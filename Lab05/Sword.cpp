#include "Sword.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"

Sword::Sword(Game* game) 
	:Actor(game)
{
	mSprite = new SpriteComponent(this);
	mSprite->SetTexture(mGame->GetTexture("Assets/Sword/Up.png"));
	mCollComp = new CollisionComponent(this);
	mCollComp->SetSize(25, 25);
}
class SpriteComponent* Sword::GetSprite() {
	return mSprite;
}
class CollisionComponent* Sword::GetCollComp() {
	return mCollComp;
}