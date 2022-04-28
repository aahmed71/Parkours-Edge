#include "Bullet.h"
#include "MeshComponent.h"
#include "MoveComponent.h"
#include "CollisionComponent.h"
#include "Block.h"
#include "Game.h"
#
#include "Renderer.h"

Bullet::Bullet(Game* game, float speed)
	:Actor(game) {
	mScale = 0.5f;
	mMesh = new MeshComponent(this);
	mMesh->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Sphere.gpmesh"));
	mMove = new MoveComponent(this);
	mMove->SetForwardSpeed(speed);
	mCollision = new CollisionComponent(this);
	mCollision->SetSize(10, 10, 10);
}

void Bullet::OnUpdate(float deltaTime) {
	timer += deltaTime;

	for (auto block : mGame->GetBlocks()) {
		if (mCollision->Intersect(block->GetComponent<CollisionComponent>())) {
			if (block->GetExplosion()) {
				Mix_PlayChannel(-1, GetGame()->GetSound("Assets/Sounds/BlockExplode.wav"), 0);
				block->Explode();
				block->SetState(ActorState::Destroy);

			}
			SetState(ActorState::Destroy);
		}
	}

	if(timer > 1) {
		SetState(ActorState::Destroy);
	}
}

