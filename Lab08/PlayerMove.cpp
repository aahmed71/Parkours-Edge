#include "PlayerMove.h"
#include "Player.h"
#include "Renderer.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "PlayerUI.h"
#include "Enemy.h"
#include "HeightMap.h"
#include "SDL2/SDL.h" 

PlayerMove::PlayerMove(Actor* owner)
	:VehicleMove(owner) {
	mVelocity = Vector3(400.0f * mSpeedMultiplier, 0.0f, 0.0f);
	mOwner->SetPosition(mOwner->GetGame()->mHeightMap->CellToWorld(39, 58));
}

void PlayerMove::Update(float deltaTime) {
	VehicleMove::Update(deltaTime);
}

void PlayerMove::OnLapChange(int newLap) {
	if (newLap == 5) {
		mOwner->SetState(ActorState::Paused);
		mOwner->GetGame()->GetEnemy()->SetState(ActorState::Paused);
		Mix_FadeOutChannel(mOwner->GetGame()->mMusicChan, 250);
		if (newLap > mOwner->GetGame()->GetEnemy()->GetComponent<VehicleMove>()->GetLap()) {
			Mix_PlayChannel(mOwner->GetGame()->mMusicChan, mOwner->GetGame()->GetSound("Assets/Sounds/Won.wav"), 0);
			mOwner->GetComponent<PlayerUI>()->SetRaceState(PlayerUI::Won);
		}
		else {
			Mix_PlayChannel(mOwner->GetGame()->mMusicChan, mOwner->GetGame()->GetSound("Assets/Sounds/Lost.wav"), 0);
			mOwner->GetComponent<PlayerUI>()->SetRaceState(PlayerUI::Lost);
		}

	}
	else {
		mOwner->GetComponent<PlayerUI>()->OnLapChange(newLap);
		if (newLap == 4) {
			Mix_FadeOutChannel(mOwner->GetGame()->mMusicChan, 250);
			mOwner->GetGame()->mMusicChan = Mix_FadeInChannel(mOwner->GetGame()->mMusicChan, mOwner->GetGame()->GetSound("Assets/Sounds/MusicFast.ogg"),0, 4000);
		}
	}
	
}

void PlayerMove::ProcessInput(const Uint8* keyState) {
	if ((keyState[SDL_SCANCODE_W] || keyState[SDL_SCANCODE_UP]) && !keyState[SDL_SCANCODE_S] && !keyState[SDL_SCANCODE_DOWN]) {
		SetPedal(true);
	}
	else {
		SetPedal(false);
	}

	if (keyState[SDL_SCANCODE_A] && !keyState[SDL_SCANCODE_D]) {
		SetDir(Direction::Left);
	}
	else if (!keyState[SDL_SCANCODE_A] && keyState[SDL_SCANCODE_D]) {
		SetDir(Direction::Right);
	}
	else {
		SetDir(Direction::NotTurning);
	}

	if (keyState[SDL_SCANCODE_SPACE] && !mSpacePressed)
	{

	}
	mSpacePressed = keyState[SDL_SCANCODE_SPACE];
}

