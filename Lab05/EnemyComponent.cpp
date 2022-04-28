#include "EnemyComponent.h"
#include "Game.h"
#include "Actor.h"

EnemyComponent::EnemyComponent(Actor* owner, std::string currRoom)
	:Component(owner)
{
	mOwner->GetGame()->AddEnemy(mOwner, currRoom);
	mRoom = currRoom;
	mHP = 1;
	mDmgTaken = 1;
}

EnemyComponent::~EnemyComponent() {
	mOwner->GetGame()->RemoveEnemy(mOwner, mRoom);
}

void EnemyComponent::TakeDamage(int amount) {
	mHP -= amount;
	if (mHP <= 0) {
		if (mOnDeath)
		{
			mOnDeath();
		}
		mOwner->SetState(ActorState::Destroy);
	}
}