#include "SideBlock.h"
#include "Game.h"
#include "MeshComponent.h"
#include "Player.h"
#include "PlayerMove.h"
#include "Renderer.h"

SideBlock::SideBlock(Game* game, int index)
	:Actor(game)
{
	mScale = 500;
	mMesh = new MeshComponent(this);
	mMesh->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Cube.gpmesh"));
	mMesh->SetTextureIndex(index);
}

void SideBlock::OnUpdate(float deltaTime) {
	// too far, destroy
	if (mGame->GetPlayer()->GetPosition().x - GetPosition().x > 2000) {
		SetState(ActorState::Destroy);
	}
}