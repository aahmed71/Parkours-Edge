#include "Fireball.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "Player.h"
#include "MoveComponent.h"

Fireball::Fireball(Game* game)
	:Actor(game)
{
	mSprite = new SpriteComponent(this);
	mSprite->SetTexture(mGame->GetTexture("Assets/FireballRed.png"));
	mMoveComp = new MoveComponent(this);
	mMoveComp->SetForwardSpeed(200.0f);
	mCollComp = new CollisionComponent(this);
	mCollComp->SetSize(16, 16);
	lifetime = 0;
}

void Fireball::OnUpdate(float deltaTime) {
	lifetime += deltaTime;
	if (lifetime >= 2) {
		SetState(ActorState::Destroy);
	}

	if (mCollComp->Intersect(mGame->GetPlayer()->GetCollComp())) {
		mGame->GetPlayer()->TakeDamage(1);
		SDL_Log("hit");
		SetState(ActorState::Destroy);
	}
}