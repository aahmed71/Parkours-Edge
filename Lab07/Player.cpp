#include "Player.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "PlayerMove.h"
#include "CollisionComponent.h"

Player::Player(class Game* owner)
	:Actor(owner) {
	mMesh = new MeshComponent(this);
	mMesh->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Ship.gpmesh"));
	mCollision = new CollisionComponent(this);
	mCollision->SetSize(100, 40, 60);
	mMove = new PlayerMove(this);

}