#include "Asteroid.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Random.h"

Asteroid::Asteroid(Game* game)
	:Actor(game)
{
	mSprite = new SpriteComponent(this);
	mSprite->SetTexture(mGame->GetTexture("Assets/Asteroid.png"));
	mMoveComp = new MoveComponent(this);
	mMoveComp->SetForwardSpeed(150.0f);
	mRotation = Random::GetFloatRange(0.0f, Math::TwoPi);
	mPosition = Random::GetVector(Vector2(0, 0), Vector2(1024, 768));
	mGame->AddAsteroid(this);
}

Asteroid::~Asteroid()
{
	mGame->RemoveAsteroid(this);
}


void Asteroid::OnUpdate(float deltaTime) {
	//check x values
	if (mPosition.x > 1024) {
		mPosition.x = 2;
	}
	else if (mPosition.x < 0) {
		mPosition.x = 1022;
	}

	//check y values
	if (mPosition.y > 768) {
		mPosition.y = 2;
	}
	else if (mPosition.y < 0) {
		mPosition.y = 766;
	}
}
