#include "DeadFrog.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "CollisionComponent.h"
#include "Vehicle.h"
#include "Game.h"

DeadFrog::DeadFrog(Game* game, Vector2 pos)
	:Actor(game)
{
	SetPosition(pos);
	mSprite = new SpriteComponent(this);
	mSprite->SetTexture(mGame->GetTexture("Assets/Dead.png"));
	lifetime = 0;
	
}

void DeadFrog::OnUpdate(float deltaTime) {
	lifetime += deltaTime;
	if (lifetime >= 0.5) {
		mState = ActorState::Destroy;
	}
}
