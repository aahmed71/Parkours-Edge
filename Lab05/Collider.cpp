#include "Collider.h"
#include "CollisionComponent.h"
Collider::Collider(class Game* game, int width, int height)
	:Actor(game)
{
	mCollComp = new CollisionComponent(this);
	mCollComp->SetSize(width, height);
}

void Collider::SetCollComp(int width, int height) {
	mCollComp->SetSize(width, height);
}

CollisionComponent* Collider::GetCollComp() {
	return mCollComp;
}