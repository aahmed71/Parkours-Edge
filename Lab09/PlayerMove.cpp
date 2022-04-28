#include "PlayerMove.h"
#include "Player.h"
#include "Renderer.h"
#include "Game.h"
#include "CameraComponent.h"
#include "SDL2/SDL.h" 

PlayerMove::PlayerMove(Actor* owner)
	:MoveComponent(owner) {
	ChangeState(MoveState::Falling);
}


void PlayerMove::ProcessInput(const Uint8* keyState) {
	if (keyState[SDL_SCANCODE_W] && !keyState[SDL_SCANCODE_S]) {
		AddForce(mOwner->GetForward() * 700.0f);
	}
	else if (!keyState[SDL_SCANCODE_W] && keyState[SDL_SCANCODE_S]) {
		AddForce(mOwner->GetForward() * -700.0f);
	}
	else {

	}

	if (keyState[SDL_SCANCODE_A] && !keyState[SDL_SCANCODE_D]) {
		AddForce(mOwner->GetRight() * -700.0f);
	}
	else if (!keyState[SDL_SCANCODE_A] && keyState[SDL_SCANCODE_D]) {
		AddForce(mOwner->GetRight() * 700.0f);
	}
	else {
		
	}

	if (keyState[SDL_SCANCODE_SPACE] && !mSpacePressed && mCurrentState == MoveState::OnGround)
	{
		AddForce(mJumpForce);
		ChangeState(MoveState::Jump);
	}
	mSpacePressed = keyState[SDL_SCANCODE_SPACE];

	//mouse
	int x, y;
	SDL_GetRelativeMouseState(&x, &y);
	float maxX = x / 500.0f;
	float maxY = y / 500.0f;
	SetAngularSpeed(maxX * Math::Pi * 10.0f);
	mOwner->GetComponent<CameraComponent>()->SetPitchSpeed(maxY * Math::Pi * 10.0f);
}

void PlayerMove::Update(float deltaTime) {
	switch (mCurrentState) {
	case MoveState::Falling:
		UpdateFalling(deltaTime);
		break;
	case MoveState::OnGround:
		UpdateOnGround(deltaTime);
		break;
	case MoveState::Jump:
		UpdateJump(deltaTime);
		break;
	}
}

void PlayerMove::UpdateOnGround(float deltaTime) {
	PhysicsUpdate(deltaTime);
	bool onTop = false;
	CollisionComponent* owner = mOwner->GetComponent<CollisionComponent>();
	for (auto block : mOwner->GetGame()->mBlocks) {
		if (FixCollision(owner, block->GetComponent<CollisionComponent>()) == CollSide::Top) {
			onTop = true;
		}
	}
	if (!onTop) {
		ChangeState(MoveState::Falling);
	}
	
}
void PlayerMove::UpdateJump(float deltaTime) {
	AddForce(mGravity);
	PhysicsUpdate(deltaTime);
	CollisionComponent* owner = mOwner->GetComponent<CollisionComponent>();
	for (auto block : mOwner->GetGame()->mBlocks) {
		if (FixCollision(owner, block->GetComponent<CollisionComponent>()) == CollSide::Bottom) {
			mVelocity.z = 0.0f;
			ChangeState(MoveState::OnGround);
		}
	}
	if (mVelocity.z <= 0.0f) {
		ChangeState(MoveState::Falling);
	}
}

void PlayerMove::UpdateFalling(float deltaTime) {
	AddForce(mGravity);
	PhysicsUpdate(deltaTime);
	CollisionComponent* owner = mOwner->GetComponent<CollisionComponent>();
	for (auto block : mOwner->GetGame()->mBlocks) {
		if (FixCollision(owner, block->GetComponent<CollisionComponent>()) == CollSide::Top) {
			mVelocity.z = 0.0f;
			ChangeState(MoveState::OnGround);
		}
	}
}

CollSide PlayerMove::FixCollision(CollisionComponent* self, CollisionComponent* block) {
	Vector3 offset;
	CollSide coll = self->GetMinOverlap(block, offset);
	if (coll != CollSide::None) {
		mOwner->SetPosition(mOwner->GetPosition() + offset);
	}
	return coll;
}

void PlayerMove::PhysicsUpdate(float deltaTime) {
	mAcceleration = mPendingForces * (1.0f / mMass);
	mVelocity += mAcceleration * deltaTime;
	FixXYVelocity();
	mOwner->SetPosition(mOwner->GetPosition() + mVelocity * deltaTime);
	mOwner->SetRotation(mOwner->GetRotation() + GetAngularSpeed() * deltaTime);
	mPendingForces = Vector3::Zero;
}

void PlayerMove::FixXYVelocity() {
	Vector2 xyVelocity = Vector2(mVelocity.x, mVelocity.y);
	if (xyVelocity.Length() > 400.0f) {
		xyVelocity = Vector2::Normalize(xyVelocity) * 400.0f;
	}
	if (mCurrentState == MoveState::OnGround) {
		if (Math::NearZero(mAcceleration.x)) {
			xyVelocity.x *= 0.9f;
		}
		else if (signbit(mAcceleration.x) != signbit(xyVelocity.x)) {
			xyVelocity.x *= 0.9f;
		}

		if (Math::NearZero(mAcceleration.y)) {
			xyVelocity.y *= 0.9f;
		}
		else if (signbit(mAcceleration.y) != signbit(xyVelocity.y)) {
			xyVelocity.y *= 0.9f;
		}
	}
	mVelocity.x = xyVelocity.x;
	mVelocity.y = xyVelocity.y;
}