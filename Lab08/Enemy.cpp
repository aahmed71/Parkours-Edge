#include "Enemy.h"
#include "Game.h"
#include "MeshComponent.h"
#include "EnemyMove.h"
#include "Renderer.h"

Enemy::Enemy(Game* game) 
	:Actor(game)
{
	mScale = 0.75;
	mMesh = new MeshComponent(this);
	mMesh->SetMesh(game->GetRenderer()->GetMesh("Assets/Kart.gpmesh"));
	mMesh->SetTextureIndex(6);
	mMove = new EnemyMove(this);
}

