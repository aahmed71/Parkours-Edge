#include "Goomba.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "GoombaMove.h"
#include "Math.h"
#include "Game.h"
#include "AnimatedSprite.h"


Goomba::Goomba(Game* game)
	:Actor(game)
{
	mSprite = new AnimatedSprite(this, 200);
	std::vector<SDL_Texture*> walkAnim{
		GetGame()->GetTexture("Assets/Goomba/Walk0.png"),
		GetGame()->GetTexture("Assets/Goomba/Walk1.png")
	};
	std::vector<SDL_Texture*> deadAnim{
	GetGame()->GetTexture("Assets/Goomba/Dead.png"),
	};
	mSprite->AddAnimation("walk", walkAnim);
	mSprite->AddAnimation("dead", deadAnim);
	mSprite->SetAnimation("walk");


	mCollComp = new CollisionComponent(this);
	mCollComp->SetSize(32, 32);
	mGoombaMove = new GoombaMove(this, mCollComp);
	mGame->AddGoomba(this);
}

Goomba::~Goomba() {
	mGame->RemoveGoomba(this);
}

CollisionComponent* Goomba::GetCollComp() {
	return mCollComp;
}

AnimatedSprite* Goomba::GetSpriteComp(){
	return mSprite;
}

GoombaMove* Goomba::GetMoveComp() {
	return mGoombaMove;
}


