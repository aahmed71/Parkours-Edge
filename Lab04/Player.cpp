#include "Player.h"
#include "AnimatedSprite.h"
#include "CollisionComponent.h"
#include "Math.h"
#include "Game.h"
#include "PlayerMove.h"

Player::Player(Game* game)
	:Actor(game)
{
	mSprite = new AnimatedSprite(this, 200);
	// choose texture for type of block
	std::vector<SDL_Texture*> idle{
	GetGame()->GetTexture("Assets/Mario/Idle.png"),
	};
	mSprite->AddAnimation("idle", idle);
	mSprite->SetAnimation("idle");

	std::vector<SDL_Texture*> dead{
	GetGame()->GetTexture("Assets/Mario/Dead.png"),
	};
	mSprite->AddAnimation("dead", dead);

	std::vector<SDL_Texture*> jumpLeft{
	GetGame()->GetTexture("Assets/Mario/JumpLeft.png"),
	};
	mSprite->AddAnimation("jumpLeft", jumpLeft);

	std::vector<SDL_Texture*> jumpRight{
	GetGame()->GetTexture("Assets/Mario/JumpRight.png"),
	};
	mSprite->AddAnimation("jumpRight", jumpRight);

	std::vector<SDL_Texture*> runLeft{
	GetGame()->GetTexture("Assets/Mario/RunLeft0.png"),
	GetGame()->GetTexture("Assets/Mario/RunLeft1.png"),
	GetGame()->GetTexture("Assets/Mario/RunLeft2.png"),
	};
	mSprite->AddAnimation("runLeft", runLeft);

	std::vector<SDL_Texture*> runRight{
	GetGame()->GetTexture("Assets/Mario/RunRight0.png"),
	GetGame()->GetTexture("Assets/Mario/RunRight1.png"),
	GetGame()->GetTexture("Assets/Mario/RunRight2.png"),
	};
	mSprite->AddAnimation("runRight", runRight);

	mCollComp = new CollisionComponent(this);
	mCollComp->SetSize(32, 32);
	mPlayerMove = new PlayerMove(this, mCollComp, mSprite);
}


CollisionComponent* Player::GetCollComp() {
	return mCollComp;
}
