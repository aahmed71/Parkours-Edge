#include "Player.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "PlayerMove.h"
#include "CollisionComponent.h"
#include "Arrow.h"
#include "CameraComponent.h"
#include "HUD.h"

Player::Player(class Game* owner, Actor* parent)
	:Actor(owner, parent) {
	mMove = new PlayerMove(this);
	mCamera = new CameraComponent(this);
	mCollision = new CollisionComponent(this);
	mCollision->SetSize(50, 175, 50);
	mHUD = new HUD(this);
}