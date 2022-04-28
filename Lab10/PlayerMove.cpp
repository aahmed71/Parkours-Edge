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
	case MoveState::WallClimb:
		UpdateWallClimb(deltaTime);
		break;
	case MoveState::WallRun:
		UpdateWallRun(deltaTime);
		break;
	}
}

void PlayerMove::UpdateOnGround(float deltaTime) {
	PhysicsUpdate(deltaTime);
	bool onTop = false;
	CollisionComponent* owner = mOwner->GetComponent<CollisionComponent>();
	for (auto block : mOwner->GetGame()->GetBlocks()) {
		if (FixCollision(owner, block->GetComponent<CollisionComponent>()) == CollSide::Top) {
			onTop = true;
		}
	}
	if (!onTop) {
		ChangeState(MoveState::Falling);
	}
	// wall climb
	for (auto block : mOwner->GetGame()->GetBlocks()) {
		CollSide coll = FixCollision(mOwner->GetComponent<CollisionComponent>(), block->GetComponent<CollisionComponent>());
		if (coll == CollSide::SideMinX || coll == CollSide::SideMaxX || coll == CollSide::SideMinY || coll == CollSide::SideMaxY) {
			if(CanWallClimb(coll)) {
				mWallClimbTimer = 0.0f;
				ChangeState(MoveState::WallClimb);
				return;
			}
		}
	}
	
}

void PlayerMove::UpdateJump(float deltaTime) {
	AddForce(mGravity);
	PhysicsUpdate(deltaTime);
	CollisionComponent* owner = mOwner->GetComponent<CollisionComponent>();
	for (auto block : mOwner->GetGame()->GetBlocks()) {
		if (FixCollision(owner, block->GetComponent<CollisionComponent>()) == CollSide::Bottom) {
			mVelocity.z = 0.0f;
			ChangeState(MoveState::OnGround);
		}
	}
	if (mVelocity.z <= 0.0f) {
		ChangeState(MoveState::Falling);
	}

	// wall climb
	for (auto block : mOwner->GetGame()->GetBlocks()) {
		CollSide coll = FixCollision(mOwner->GetComponent<CollisionComponent>(), block->GetComponent<CollisionComponent>());
		if (coll == CollSide::SideMinX || coll == CollSide::SideMaxX || coll == CollSide::SideMinY || coll == CollSide::SideMaxY) {
			if (CanWallClimb(coll)) {
				mWallClimbTimer = 0.0f;
				ChangeState(MoveState::WallClimb);
				return;
			}
			else if (CanWallRun(coll)) {
				mWallRunTimer = 0.0f;
   				SDL_Log("Wall Run");
   				ChangeState(MoveState::WallRun);
				return;
			}
		}
	}
}

bool PlayerMove::CanWallClimb(CollSide side) {
	float targetDot;
	Vector3 xyNorm = Vector3::Normalize(Vector3(mVelocity.x, mVelocity.y, 0));
	switch (side) {
	case CollSide::SideMinX:
		 targetDot = Vector3::Dot(mOwner->GetForward(), Vector3(1, 0, 0));
		if (targetDot < 0.8f) {
			return false;
		}
		targetDot = Vector3::Dot(xyNorm, Vector3(1, 0, 0));
		if (targetDot < 0.8f) {
			return false;
		}
		break;

	case CollSide::SideMaxX:
		targetDot = Vector3::Dot(mOwner->GetForward(), Vector3(-1, 0, 0));
		if (targetDot < 0.8f) {
			return false;
		}
		targetDot = Vector3::Dot(xyNorm, Vector3(-1, 0, 0));
		if (targetDot < 0.8f) {
			return false;
		}
		break;

	case CollSide::SideMinY:
		targetDot = Vector3::Dot(mOwner->GetForward(), Vector3(0, 1, 0));
		if (targetDot < 0.8f) {
			return false;
		}
		targetDot = Vector3::Dot(xyNorm, Vector3(0, 1, 0));
		if (targetDot < 0.8f) {
			return false;
		}
		break;

	case CollSide::SideMaxY:
		targetDot = Vector3::Dot(mOwner->GetForward(), Vector3(0, -1, 0));
		if (targetDot < 0.8f) {
			return false;
		}
		targetDot = Vector3::Dot(xyNorm, Vector3(0, -1, 0));
		if (targetDot < 0.8f) {
			return false;
		}
		break;
	}
	float speed = Vector3(mVelocity.x, mVelocity.y, 0).Length();
	if (speed < 350.0f) {
		return false;
	}
	return true;
}

bool PlayerMove::CanWallRun(CollSide side) {
	float targetDot;
	Vector3 xyNorm = Vector3::Normalize(Vector3(mVelocity.x, mVelocity.y, 0));
	Vector3 forward = mOwner->GetForward();
	CameraComponent* cam = mOwner->GetComponent<CameraComponent>();
	switch (side) {
	case CollSide::SideMinX:
		targetDot = Vector3::Dot(forward, Vector3(1, 0, 0));
		if (targetDot > 0.7f) {
			return false;
		}
		targetDot = Vector3::Dot(xyNorm, forward);
		if (targetDot < 0.8f) {
			return false;
		}
		cam->SetRollSpeed(-1);
		cam->XorY = false;
		break;

	case CollSide::SideMaxX:
		targetDot = Vector3::Dot(forward, Vector3(-1, 0, 0));
		if (targetDot > 0.7f) {
			return false;
		}
		targetDot = Vector3::Dot(xyNorm, forward);
		if (targetDot < 0.8f) {
			return false;
		}
		cam->SetRollSpeed(1);
		cam->XorY = false;
		break;

	case CollSide::SideMinY:
 		targetDot = Vector3::Dot(forward, Vector3(0, 1, 0));
		if (targetDot > 0.7f) {
			return false;
		}
		targetDot = Vector3::Dot(xyNorm, forward);
		if (targetDot < 0.8f) {
			return false;
		}
		cam->SetRollSpeed(1);
		cam->XorY = true;
		break;

	case CollSide::SideMaxY:
		targetDot = Vector3::Dot(forward, Vector3(0, -1, 0));
		if (targetDot > 0.7f) {
			return false;
		}
		targetDot = Vector3::Dot(xyNorm, forward);
		if (targetDot < 0.8f) {
			return false;
		}
		cam->SetRollSpeed(-1);
		cam->XorY = true;
		break;
	}
	float speed = Vector3(mVelocity.x, mVelocity.y, 0).Length();
	if (speed < 350.0f) {
		return false;
	}
	return true;
}

void PlayerMove::UpdateFalling(float deltaTime) {
	AddForce(mGravity);
	PhysicsUpdate(deltaTime);
	CollisionComponent* owner = mOwner->GetComponent<CollisionComponent>();
	for (auto block : mOwner->GetGame()->GetBlocks()) {
		if (FixCollision(owner, block->GetComponent<CollisionComponent>()) == CollSide::Top) {
			mVelocity.z = 0.0f;
			ChangeState(MoveState::OnGround);
		}
	}
}

void PlayerMove::UpdateWallClimb(float deltaTime) {
	mWallClimbTimer += deltaTime;
	AddForce(mGravity);
	if (mWallClimbTimer < 0.4f) {
		AddForce(mClimbForce);
	}
	PhysicsUpdate(deltaTime);
	CollisionComponent* owner = mOwner->GetComponent<CollisionComponent>();
	bool falling = true;
	for (auto block : mOwner->GetGame()->GetBlocks()) {
		if (FixCollision(owner, block->GetComponent<CollisionComponent>()) != CollSide::None) {
			falling = false;
		}
	}
	if (falling || mVelocity.z <= 0.0f) {
		mVelocity.z = 0.0f;
		ChangeState(MoveState::Falling);
	}
}

void PlayerMove::UpdateWallRun(float deltaTime) {
	mWallRunTimer += deltaTime;
	AddForce(mGravity);
	if (mWallRunTimer < 0.4f) {
		AddForce(mWallRunForce);
	}
	PhysicsUpdate(deltaTime);
	CollisionComponent* owner = mOwner->GetComponent<CollisionComponent>();
	for (auto block : mOwner->GetGame()->GetBlocks()) {
		FixCollision(owner, block->GetComponent<CollisionComponent>());
	}
	if (mVelocity.z <= 0.0f) {
		mVelocity.z = 0.0f;
		//mOwner->GetComponent<CameraComponent>()->SetRollAngle(0);
		ChangeState(MoveState::Falling);
	}
}

CollSide PlayerMove::FixCollision(CollisionComponent* self, CollisionComponent* block) {
	Vector3 offset;
	CollSide coll = self->GetMinOverlap(block, offset);
	if (coll != CollSide::None) {
		mOwner->SetPosition(mOwner->GetPosition() + offset);
		switch (coll) {
		case CollSide::SideMinX:
			AddForce(Vector3(-700.0f, 0.0f, 0.0f));
			break;
		case CollSide::SideMaxX:
			AddForce(Vector3(700.0f, 0.0f, 0.0f));
			break;
		case CollSide::SideMinY:
			AddForce(Vector3(0.0f, -700.0f, 0.0f));
			break;
		case CollSide::SideMaxY:
			AddForce(Vector3(0.0f, 700.0f, 0.0f));
			break;
		}
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
	if (mCurrentState == MoveState::OnGround || mCurrentState == MoveState::WallClimb) {
		if (Math::NearZero(mAcceleration.x)) {
			xyVelocity.x *= 0.9f;
		}
		if (Math::NearZero(mAcceleration.y)) {
			xyVelocity.y *= 0.9f;
		}
		if (signbit(mAcceleration.x) != signbit(xyVelocity.x)) {
			xyVelocity.x *= 0.9f;
		}
		if (signbit(mAcceleration.y) != signbit(xyVelocity.y)) {
			xyVelocity.y *= 0.9f;
		}
	}
	mVelocity.x = xyVelocity.x;
	mVelocity.y = xyVelocity.y;
}