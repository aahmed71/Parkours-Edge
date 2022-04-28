#include "Coin.h"
#include "Game.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "Player.h"
#include "Renderer.h"

Coin::Coin(Game* game, Actor* parent)
	:Actor(game, parent)
{
	mMesh = new MeshComponent(this);
	mMesh->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Coin.gpmesh"));
	mCollision = new CollisionComponent(this);
	mCollision->SetSize(100.0f, 100.0f, 100.0f);
}


void Coin::OnUpdate(float deltaTime) {
	SetRotation(GetRotation() + (deltaTime * Math::Pi));
	if (mCollision->Intersect(mGame->GetPlayer()->GetComponent<CollisionComponent>())) {
		Mix_PlayChannel(-1, GetGame()->GetSound("Assets/Sounds/Coin.wav"), 0);
		SetState(ActorState::Destroy);
	}
}