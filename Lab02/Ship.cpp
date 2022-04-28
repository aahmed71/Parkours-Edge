#include "Ship.h"
#include "Game.h"
#include "Laser.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"

Ship::Ship(Game* game)
	:Actor(game)
{
	mSprite = new SpriteComponent(this);
	mSprite->SetTexture(mGame->GetTexture("Assets/Ship.png"));
	mMoveComp = new MoveComponent(this);
	lastLaserTime = 1;
}

void Ship::OnProcessInput(const Uint8* keyState) {
	// ship when no keys are pressed
	mMoveComp->SetAngularSpeed(0);
	mMoveComp->SetForwardSpeed(0);
	mSprite->SetTexture(mGame->GetTexture("Assets/Ship.png"));

	//stop ship if both forward and back pressed
	if (keyState[SDL_SCANCODE_UP] && keyState[SDL_SCANCODE_DOWN]) {
		mMoveComp->SetForwardSpeed(0);
	}
	// move forward
	else if (keyState[SDL_SCANCODE_UP]) {
		mMoveComp->SetForwardSpeed(200);
		mSprite->SetTexture(mGame->GetTexture("Assets/ShipThrust.png"));
	}
	// move back
	else if (keyState[SDL_SCANCODE_DOWN]) {
		mMoveComp->SetForwardSpeed(-200);
		mSprite->SetTexture(mGame->GetTexture("Assets/ShipThrust.png"));
	}

	// stop ship if both pressed
	if (keyState[SDL_SCANCODE_LEFT] && keyState[SDL_SCANCODE_RIGHT]) {
		mMoveComp->SetAngularSpeed(0);
	}

	// turn right
	else if (keyState[SDL_SCANCODE_RIGHT]) {
		mMoveComp->SetAngularSpeed(-5);
	}
	// turn left
	else if (keyState[SDL_SCANCODE_LEFT]) {
		mMoveComp->SetAngularSpeed(5);
	}

	//shoot lasers
	if (keyState[SDL_SCANCODE_SPACE]) {
		if (lastLaserTime >= 1.0f) {
			Laser* temp = new Laser(mGame);
			temp->SetPosition(mPosition);
			temp->SetRotation(mRotation);
			lastLaserTime = 0;
		}

	}
}

void Ship::OnUpdate(float deltaTime) {
	lastLaserTime += deltaTime;
}

