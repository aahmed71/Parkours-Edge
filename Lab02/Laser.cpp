#include "Laser.h"
#include "Asteroid.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"

Laser::Laser(Game* game)
	:Actor(game)
{
	mSprite = new SpriteComponent(this);
	mSprite->SetTexture(mGame->GetTexture("Assets/Laser.png"));
	mMoveComp = new MoveComponent(this);
	mMoveComp->SetForwardSpeed(400.0f);
	lifetime = 0;
}

void Laser::OnUpdate(float deltaTime) {
	lifetime += deltaTime;
	if (lifetime >= 1) {
		mState = ActorState::Destroy;
	}
	// check asteroids
	else {
		for (auto act : mGame->GetAsteroid()) {
			Vector2 aToB = act->GetPosition() - mPosition;
			float distance = aToB.Length();
			// asteroid hit
			if (distance <= 70) {
				SDL_Log("asteroid destoyed");
				mState = ActorState::Destroy;
				act->SetState(ActorState::Destroy);
			}
		}
	}
}