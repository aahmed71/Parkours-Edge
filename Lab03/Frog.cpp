#include "Frog.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "WrappingMove.h"
#include "CollisionComponent.h"
#include "Vehicle.h"
#include "Log.h"
#include "DeadFrog.h"
#include "Game.h"

Frog::Frog(Game* game, Vector2 start)
	:Actor(game)
{
	mSprite = new SpriteComponent(this);
	mSprite->SetTexture(mGame->GetTexture("Assets/Frog.png"));
	mCollComp = new CollisionComponent(this);
	mCollComp->SetSize(50, 50);
	mStart = start;
}

void Frog::OnProcessInput(const Uint8* keyState) {
	// froggy moves forward
	if (keyState[SDL_SCANCODE_UP] && !mGame->getPrevState()[SDL_SCANCODE_UP] && mPosition.y > (GRID_SQUARE_SIZE * 3)) {
		mPosition += Vector2(0, -64);
	}
	// froggy moves down
	if (keyState[SDL_SCANCODE_DOWN] && !mGame->getPrevState()[SDL_SCANCODE_DOWN] && mPosition.y < (WINDOW_HEIGHT - GRID_SQUARE_SIZE * 2)) {
		mPosition += Vector2(0, 64);
	}
	// froggy moves left
	if (keyState[SDL_SCANCODE_LEFT] && !mGame->getPrevState()[SDL_SCANCODE_LEFT] && mPosition.x > GRID_SQUARE_SIZE) {
		mPosition += Vector2(-64, 0);
	}
	if (keyState[SDL_SCANCODE_RIGHT] && !mGame->getPrevState()[SDL_SCANCODE_RIGHT] && mPosition.x < (WINDOW_WIDTH - GRID_SQUARE_SIZE)) {
		mPosition += Vector2(64, 0);
	}
}

void Frog::OnUpdate(float deltaTime) {
	for (auto v : mGame->GetVehicle()) {
		// dead froggy :(
		if (mCollComp->Intersect(v->GetCollComp())) {
			DeadFrog* death = new DeadFrog(mGame, mPosition);
			mPosition = mStart;
		}
	}
	// water section
	bool onLog = false;
	for (auto l : mGame->GetLog()) {
		Vector2 offset;
		if (mCollComp->GetMinOverlap(l->GetCollComp(), offset) != CollSide::None) {
			onLog = true;
			mPosition += l->GetMoveComp()->GetForwardSpeed() * l->GetMoveComp()->GetOwnerDirection() * deltaTime;
			if (mCollComp->GetMinOverlap(l->GetCollComp(), offset) == CollSide::Left) {
				mPosition += offset;
			}
			else if (mCollComp->GetMinOverlap(l->GetCollComp(), offset) == CollSide::Right) {
				mPosition -= offset;
			}
		}
	}
	// check for drowing
	if (mPosition.y >= 180 && mPosition.y <= 510 && !onLog) {
		DeadFrog* death = new DeadFrog(mGame, mPosition);
		mPosition = mStart;
	}

	// goal
	if (mPosition.y < 180) {
		if (mCollComp->Intersect(mGame->GetGoalCol())) {
			mPosition = mGame->GetGoal()->GetPosition();
			SetState(ActorState::Paused);
		}
		else {
			DeadFrog* death = new DeadFrog(mGame, mPosition);
			mPosition = mStart;
		}

	}
}