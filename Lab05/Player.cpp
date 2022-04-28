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
	// choose texture 
	std::vector<SDL_Texture*> walkUp{
	GetGame()->GetTexture("Assets/Link/WalkUp0.png"),
	GetGame()->GetTexture("Assets/Link/WalkUp1.png"),
	};
	mSprite->AddAnimation("walkUp", walkUp);
	mSprite->SetAnimation("walkUp");

	std::vector<SDL_Texture*> walkDown{
	GetGame()->GetTexture("Assets/Link/WalkDown0.png"),
	GetGame()->GetTexture("Assets/Link/WalkDown1.png"),
	};
	mSprite->AddAnimation("walkDown", walkDown);

	std::vector<SDL_Texture*> walkLeft{
	GetGame()->GetTexture("Assets/Link/WalkLeft0.png"),
	GetGame()->GetTexture("Assets/Link/WalkLeft1.png"),
	};
	mSprite->AddAnimation("walkLeft", walkLeft);

	std::vector<SDL_Texture*> walkRight{
	GetGame()->GetTexture("Assets/Link/WalkRight0.png"),
	GetGame()->GetTexture("Assets/Link/WalkRight1.png"),
	};
	mSprite->AddAnimation("walkRight", walkRight);

	mCollComp = new CollisionComponent(this);
	mCollComp->SetSize(20, 20);
	mPlayerMove = new PlayerMove(this, this, mCollComp, mSprite);
	mKeyCount = 0;
	mHP = 10;
}


CollisionComponent* Player::GetCollComp() {
	return mCollComp;
}

int Player::GetKeyCount() {
	return mKeyCount;
}

void Player::SetKeyCount(int key) {
	mKeyCount += key;
}	

void Player::TakeDamage(int amount) {
	mHP -= amount;
	if (mHP <= 0) {
		SetState(ActorState::Paused);
	}
}