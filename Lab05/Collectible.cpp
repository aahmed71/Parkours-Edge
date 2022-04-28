#include "Collectible.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "Actor.h"
#include "Player.h"

Collectible::Collectible(class Actor* owner, class CollisionComponent* collComp)
	:Component(owner) 
{
	mCollComp = collComp;
}

void Collectible::Update(float deltaTime) {
	if (mCollComp->Intersect(mOwner->GetGame()->GetPlayer()->GetCollComp())) {
		mOwner->SetState(ActorState::Destroy);
		if (mOnCollect)
		{
			mOnCollect();
		}
	}
}

void Collectible::SetOnCollect(std::function<void()> newColl) {
	mOnCollect = newColl;
}