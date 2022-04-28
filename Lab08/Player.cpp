#include "Player.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "PlayerMove.h"
#include "CollisionComponent.h"
#include "CameraComponent.h"
#include "PlayerUI.h"

Player::Player(class Game* owner)
	:Actor(owner) {
	mMesh = new MeshComponent(this);
	mMesh->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Kart.gpmesh"));
	mMove = new PlayerMove(this);
	mCamera = new CameraComponent(this);
	mUI = new PlayerUI(this);
	mScale = 0.75f;

}