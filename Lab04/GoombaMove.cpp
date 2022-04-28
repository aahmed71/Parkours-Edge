#include "GoombaMove.h"
#include "Goomba.h"
#include "Actor.h"
#include "Game.h"
#include "Math.h"
#include "CollisionComponent.h"
#include "Block.h"

GoombaMove::GoombaMove(class Actor* owner, class CollisionComponent* collComp)
	:MoveComponent(owner)
{
	mYSpeed = 0;
	mCollComp = collComp;
	dead = false;
	lifetime = 0;
	SetForwardSpeed(-100.0f);
}

void GoombaMove::SetDeath(bool death) {
	dead = death;
}

bool GoombaMove::GetDeath() {
	return dead;
}

void GoombaMove::Update(float deltaTime) {
	if (!dead) {
		mOwner->SetPosition(mOwner->GetPosition() + deltaTime * GetForwardSpeed() * mOwner->GetForward());

		mOwner->SetPosition(mOwner->GetPosition() + deltaTime * mYSpeed * Vector2(0, 1));

		if (mOwner->GetPosition().y > WINDOW_HEIGHT) {
			mOwner->SetState(ActorState::Destroy);
		}

		float grav = 2000.0f;

		for (auto block : mOwner->GetGame()->GetBlocks()) {
			Vector2 offset;
			CollSide side = mCollComp->GetMinOverlap(block->GetCollComp(), offset);
			if (side == CollSide::Top) {
				if (mYSpeed > 0) {
					mYSpeed = 0;
					mOwner->SetPosition(mOwner->GetPosition() + offset);
				}
			}
		}
		for (auto block : mOwner->GetGame()->GetBlocks()) {
			Vector2 offset;
			CollSide side = mCollComp->GetMinOverlap(block->GetCollComp(), offset);
			float diff = block->GetPosition().y - mOwner->GetPosition().y;
			if (side == CollSide::Left && (Math::Abs(diff) < 1)) {
				mOwner->SetPosition(mOwner->GetPosition() + offset);
				SetForwardSpeed(-100);
			}
			else if (side == CollSide::Right && (Math::Abs(diff) < 1)) {
				mOwner->SetPosition(mOwner->GetPosition() + offset);
				SetForwardSpeed(100);
			}
		}

		for (auto goomba : mOwner->GetGame()->GetGoombas()) {
			if (goomba != mOwner) {
				Vector2 offset;
				CollSide side = mCollComp->GetMinOverlap(goomba->GetCollComp(), offset);
				switch (side) {
				case CollSide::Left:
					mOwner->SetPosition(mOwner->GetPosition() + offset);
					SetForwardSpeed(-100);
					break;
				case CollSide::Right:
					mOwner->SetPosition(mOwner->GetPosition() + offset);
					SetForwardSpeed(100);
					break;

				}
			}
		}
		mYSpeed += grav * deltaTime;
	}
	else {
		lifetime += deltaTime;
		if (lifetime > 0.25) {
			mOwner->SetState(ActorState::Destroy);
		}
	}
	if (mOwner->GetPosition().x < mOwner->GetGame()->GetCameraPos().x - 50) {
		mOwner->SetState(ActorState::Destroy);
	}
	
}

