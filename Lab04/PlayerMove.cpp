#include "PlayerMove.h"
#include "Actor.h"
#include "Game.h"
#include "Math.h"
#include "Goomba.h"
#include "CollisionComponent.h"
#include "AnimatedSprite.h"
#include "GoombaMove.h"
#include "Block.h"

PlayerMove::PlayerMove(class Actor* owner, class CollisionComponent* collComp, class AnimatedSprite* sprite)
	:MoveComponent(owner)
{
	mYSpeed = 0;
	mCollComp = collComp;
	mSprite = sprite;
	mSpacePressed = false;
	mInAir = false;
	dead = false;
}

void PlayerMove::ProcessInput(const Uint8* keyState){
	// move left and right
	if (keyState[SDL_SCANCODE_LEFT] && keyState[SDL_SCANCODE_RIGHT]) {
		SetForwardSpeed(0);
	}
	else if (keyState[SDL_SCANCODE_LEFT]) {
		SetForwardSpeed(-300.0f);
	}
	else if (keyState[SDL_SCANCODE_RIGHT]) {
		SetForwardSpeed(300.0f);
	}
	else {
		SetForwardSpeed(0);
	}

	// jump
	if (keyState[SDL_SCANCODE_SPACE] && !mOwner->GetGame()->getPrevState()[SDL_SCANCODE_SPACE] && !mInAir) {
		Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Jump.wav"), 0);
		mYSpeed = -700.0f;
		mInAir = true;
	}


}

void PlayerMove::Update(float deltaTime){
	mOwner->SetPosition(mOwner->GetPosition() + deltaTime * GetForwardSpeed() * mOwner->GetForward());
	if (mOwner->GetPosition().x <= mOwner->GetGame()->GetCameraPos().x) {
		mOwner->SetPosition(Vector2(mOwner->GetGame()->GetCameraPos().x, mOwner->GetPosition().y));
	}

	mOwner->SetPosition(mOwner->GetPosition() + deltaTime * mYSpeed * Vector2(0, 1));

	if (mOwner->GetPosition().y > 550.0f) {
		mInAir = false;
		dead = true;
		mOwner->SetState(ActorState::Paused);
	}
	// win
	if (mOwner->GetPosition().x > 6368) {
		Mix_HaltChannel(mOwner->GetGame()->getMusicChannel());
		Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/StageClear.wav"), 0);
		mOwner->SetState(ActorState::Paused);
	}

	float grav = 2000.0f;
	bool collision = false;
	for (auto block : mOwner->GetGame()->GetBlocks()) {
		Vector2 offset;
		CollSide side = mCollComp->GetMinOverlap(block->GetCollComp(), offset);
		switch(side) {
		case CollSide::None:
			break;
		case CollSide::Top:
			collision = true;
			if (mYSpeed > 0) {
				mYSpeed = 0;
				mOwner->SetPosition(mOwner->GetPosition() + offset);
				mInAir = false;
			}
			break;
		case CollSide::Bottom:
			if (mYSpeed < 0) {
				Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Bump.wav"), 0);
				mYSpeed = 0;
			}
		default:
			collision = true;
			mOwner->SetPosition(mOwner->GetPosition() + offset);
			break;
		}
		
	}
	if (!collision) {
		mInAir = true;
	}
	for (auto goomba : mOwner->GetGame()->GetGoombas()) {
			Vector2 offset;
			if (!goomba->GetMoveComp()->GetDeath()) {
				CollSide side = mCollComp->GetMinOverlap(goomba->GetCollComp(), offset);
				bool con1 = side == CollSide::Top;
				bool con2 = (side == CollSide::Left || side == CollSide::Right) && (mInAir);
				// stomped
				if (con1 || con2) {
					Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Stomp.wav"), 0);
					goomba->GetSpriteComp()->SetAnimation("dead");
					mYSpeed = -350.0f;
					mInAir = true;
					goomba->GetMoveComp()->SetDeath(true);
				}
				else if (mCollComp->Intersect(goomba->GetCollComp())) {
					dead = true;
					mOwner->SetState(ActorState::Paused);
				}
			}
	}
	UpdateAnim();
	mYSpeed += grav * deltaTime;

	mOwner->GetGame()->SetCameraPos(Vector2(mOwner->GetPosition().x - WINDOW_WIDTH / 2, 
		mOwner->GetGame()->GetCameraPos().y));
}

void PlayerMove::UpdateAnim() {
	if (dead) {
		mSprite->SetAnimation("dead");
		Mix_HaltChannel(mOwner->GetGame()->getMusicChannel());
		Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Dead.wav"), 0);
	}
	else if (!mInAir) {
		if (GetForwardSpeed() < 0) {
			mSprite->SetAnimation("runLeft");
		}
		else if(GetForwardSpeed() > 0){
			mSprite->SetAnimation("runRight");
		}
		else {
			mSprite->SetAnimation("idle");
		}
	}
	else {
		if (GetForwardSpeed() < 0) {
			mSprite->SetAnimation("jumpLeft");
		}
		else if (GetForwardSpeed() > 0) {
			mSprite->SetAnimation("jumpRight");
		}
		else {
			std::string currAnim = mSprite->GetAnimName();
			if (currAnim == "runRight" || currAnim == "jumpRight" || currAnim == "idle") {
				mSprite->SetAnimation("jumpRight");
			}
			else {
				mSprite->SetAnimation("jumpLeft");
			}

		}
	}
}