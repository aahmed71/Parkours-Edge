#include "Checkpoint.h"
#include "MeshComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "Player.h"
#include "CollisionComponent.h"

Checkpoint::Checkpoint(Game* game, Actor* parent)
	:Actor(game, parent)
{
	mMesh = new MeshComponent(this);
	mMesh->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Checkpoint.gpmesh"));
	mMesh->SetTextureIndex(1);
	mCollision = new CollisionComponent(this);
	mCollision->SetSize(25.0, 25.0f, 25.0f);
}

void Checkpoint::Activate(bool active) {
	int index = !active;
	mMesh->SetTextureIndex(index);
	mActive = active;
}

void Checkpoint::OnUpdate(float deltaTime) {
	if (mCollision->Intersect(mGame->GetPlayer()->GetComponent<CollisionComponent>())) {
		Mix_PlayChannel(-1, GetGame()->GetSound("Assets/Sounds/Checkpoint.wav"), 0);
		if (!mLevelString.empty()) {
			GetGame()->SetNextLevel(mLevelString);
		}
	}
}