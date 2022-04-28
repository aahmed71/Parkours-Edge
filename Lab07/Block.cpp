#include "Block.h"
#include "Game.h"
#include "Renderer.h"
#include "Player.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"

Block::Block(Game* game, bool exploding)
	:Actor(game)
{
	mScale = 25.0f;
	mMesh = new MeshComponent(this);
	mMesh->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Cube.gpmesh"));
	int texture = 2;
	texture += 2 * exploding;
	mMesh->SetTextureIndex(texture);
	mCollision = new CollisionComponent(this);
	mCollision->SetSize(1, 1, 1);
	mExploding = exploding;
	mGame->AddBlock(this);
}

Block::~Block() {
	mGame->RemoveBlock(this);
}

void Block::OnUpdate(float deltaTime) {
	if (mGame->GetPlayer()->GetPosition().x - GetPosition().x > 2000) {
		SetState(ActorState::Destroy);
	}
}

bool Block::GetExplosion() {
	return mExploding;
}
bool Block::GetExploded() {
	return mExploded;
}
void Block::Explode() {
	mExploded = true;
	for (auto block : mGame->GetBlocks()) {
		if ((block->GetPosition() - GetPosition()).Length() <= 50 && block != this && !block->GetExploded()) {
			if (block->GetExplosion()) {
				block->Explode();
				block->SetState(ActorState::Destroy);
			}
			else {
				block->SetState(ActorState::Destroy);
			}
		}
	}
}
