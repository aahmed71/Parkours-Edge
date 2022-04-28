#include "Block.h"
#include "MeshComponent.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "Renderer.h"

Block::Block(Game* game)
	:Actor(game)
{
	mScale = 64.0f;
	mMesh = new MeshComponent(this);
	mMesh->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Cube.gpmesh"));
	mCollision = new CollisionComponent(this);
	mCollision->SetSize(1, 1, 1);
	game->mBlocks.push_back(this);
}

Block::~Block() {
	auto iter = std::find(mGame->mBlocks.begin(), mGame->mBlocks.end(), this);
	if (iter != mGame->mBlocks.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		auto iter2 = mGame->mBlocks.end() - 1;
		std::iter_swap(iter, iter2);
		mGame->mBlocks.pop_back();
	}
}
