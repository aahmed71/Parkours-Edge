#include "PlayerMove.h"
#include "Player.h"
#include "Renderer.h"
#include "Game.h"
#include "SideBlock.h"
#include "CollisionComponent.h"
#include "Bullet.h"
#include "Block.h"
#include "SDL2/SDL.h" 

PlayerMove::PlayerMove(Actor* owner)
	:MoveComponent(owner) {
	mVelocity = Vector3(400.0f * mSpeedMultiplier, 0.0f, 0.0f);
}

void PlayerMove::Update(float deltaTime) {
	// increment time if necessary
	mSpeedTimer += deltaTime;
	if (mSpeedTimer >= 10) {
		mSpeedMultiplier += 0.15f;
		mSpeedTimer = 0.0f;
	}

	// move player
	mVelocity.x = 400.0f * mSpeedMultiplier;
	Vector3 playerPos = mOwner->GetPosition();
	playerPos += mVelocity * deltaTime;
	if (playerPos.y < -195) {
		playerPos.y = -195;
	}
	else if (playerPos.y > 195) {
		playerPos.y = 195;
	}

	if (playerPos.z < -255) {
		playerPos.z = -255;
	}
	else if (playerPos.z > 255) {
		playerPos.z = 255;
	}
	mOwner->SetPosition(playerPos);
	for (auto block : mOwner->GetGame()->GetBlocks()) {
		if (mOwner->GetComponent<CollisionComponent>()->Intersect(block->GetComponent<CollisionComponent>()) && !block->GetExploded()) {
			mOwner->SetState(ActorState::Paused);
			Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/ShipDie.wav"), 0);
			Mix_HaltChannel(mOwner->GetGame()->GetMusicChannel());
		}
	}

	// add side blocks
	if (mSideBlockX - playerPos.x < 2500) {
		mSideBlockX += 500;
		int index = 1;
		if ((mSideBlockX / 100) % 2 == 0) {
			index = 3;
		}
		SideBlock* temp1 = new SideBlock(mOwner->GetGame(), index);
		temp1->SetPosition(Vector3(static_cast<float>(mSideBlockX), 500.0f, 0));
		SideBlock* temp2 = new SideBlock(mOwner->GetGame(), index);
		temp2->SetPosition(Vector3(static_cast<float>(mSideBlockX), -500.0f, 0));
	}

	// add obstacles
	if (mBlockX - playerPos.x < 2000) {
		mBlockX += 1000;
		int textFile = mBlockX / 1000;
		if (mBlockX > 20000) {
			textFile = rand() % 20 + 1;
		}
		std::string file = "Assets/Blocks/" + std::to_string(textFile) + ".txt";
		mOwner->GetGame()->LoadBlocks(file, static_cast<float>(mBlockX));
	}

	// move camera
	Vector3 forward = mOwner->GetForward();
	Vector3 up = Vector3::UnitZ;
	float HDist = 300.0f;
	float VDist = 100.0f;
	float TargetDist = 20.0f;
	Vector3 CameraPos = playerPos - (forward * HDist) + (up * VDist);
	Vector3 TargetPos = playerPos + (forward * TargetDist);
	mOwner->GetGame()->GetRenderer()->SetViewMatrix(Matrix4::CreateLookAt(CameraPos, TargetPos, Vector3::UnitZ));
}

void PlayerMove::ProcessInput(const Uint8* keyState) {
	if (keyState[SDL_SCANCODE_W] && !keyState[SDL_SCANCODE_S]) {
		mVelocity.z = 300;
	}
	else if (!keyState[SDL_SCANCODE_W] && keyState[SDL_SCANCODE_S]) {
		mVelocity.z = -300;
	}
	else {
		mVelocity.z = 0;
	}

	if (keyState[SDL_SCANCODE_A] && !keyState[SDL_SCANCODE_D]) {
		mVelocity.y = -300;
	}
	else if (!keyState[SDL_SCANCODE_A] && keyState[SDL_SCANCODE_D]) {
		mVelocity.y = 300;
	}
	else {
		mVelocity.y = 0;
	}

	// Check for attack.
	if (keyState[SDL_SCANCODE_SPACE] && !mSpacePressed)
	{
		Bullet* shot = new Bullet(mOwner->GetGame(), mVelocity.x + 500);
		shot->SetPosition(mOwner->GetPosition());
		Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Shoot.wav"), 0);
	}
	mSpacePressed = keyState[SDL_SCANCODE_SPACE];
}

