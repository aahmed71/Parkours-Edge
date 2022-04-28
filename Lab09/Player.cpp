#include "Player.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "PlayerMove.h"
#include "CollisionComponent.h"
#include "CameraComponent.h"


Player::Player(class Game* owner)
	:Actor(owner) {
	mMove = new PlayerMove(this);
	mCamera = new CameraComponent(this);
	mCollision = new CollisionComponent(this);
	mCollision->SetSize(50, 175, 50);
}