#include "Key.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "Collectible.h"
#include "Player.h"
#include "Game.h"

Key::Key(class Game* game)
	:Actor(game)
{
	mSprite = new SpriteComponent(this);
	mSprite->SetTexture(mGame->GetTexture("Assets/Key.png"));
	mCollComp = new CollisionComponent(this);
	mCollComp->SetSize(16, 32);
	Collectible* collectible = new Collectible(this, mCollComp);
	collectible->SetOnCollect([this] {
		mGame->GetPlayer()->SetKeyCount(1);
	});

}

CollisionComponent* Key::GetCollComp() {
	return mCollComp;
}

SpriteComponent* Key::GetSprite() {
	return mSprite;
}
