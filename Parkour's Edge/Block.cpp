#include "Block.h"
#include "MeshComponent.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "Renderer.h"

Block::Block(Game* game, Actor* parent)
	:Actor(game, parent)
{
	mScale = 64.0f;
	mMesh = new MeshComponent(this);
	mMesh->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Cube.gpmesh"));
	mCollision = new CollisionComponent(this);
	mCollision->SetSize(1, 1, 1);
	game->AddBlock(this);
}

Block::~Block() {
	auto iter = std::find(mGame->GetBlocks().begin(), mGame->GetBlocks().end(), this);
	if (iter != mGame->GetBlocks().end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		auto iter2 = mGame->GetBlocks().end() - 1;
		std::iter_swap(iter, iter2);
		mGame->PopBlock();
	}
}

void Block::OnUpdate(float deltaTime) {
	if (mRotating) {
		SetRotation(GetRotation() + Math::Pi / 4.0f * deltaTime);
	}
}