#include "LaserMine.h"
#include "Game.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "LaserComponent.h"

LaserMine::LaserMine(Game* game, Actor* parent)
	:Actor(game, parent)
{
	mMesh = new MeshComponent(this);
	mMesh->SetMesh(mGame->GetRenderer()->GetMesh("Assets/LaserMine.gpmesh"));
	mLaserMesh = new LaserComponent(this);
}