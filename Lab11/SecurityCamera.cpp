#include "SecurityCamera.h"
#include "Game.h" 
#include "Renderer.h"
#include "MeshComponent.h"
#include "SecurityCone.h"
#include "Player.h"

SecurityCamera::SecurityCamera(Game* game, Actor* player) 
	:Actor(game, player)
{
	SetQuat(mStartQ);
	mMesh = new MeshComponent(this);
	mMesh->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Camera.gpmesh"));
	mCone = new SecurityCone(game, (Actor*)this);
	mCameraSFX = Mix_PlayChannel(Mix_GroupAvailable(1), mGame->GetSound("Assets/Sounds/CameraMotor.wav"), 0);
	Mix_Pause(mCameraSFX);
}

void SecurityCamera::OnUpdate(float deltaTime) {
	Player* player = GetGame()->GetPlayer();
	Vector3 camToPlayer = player->GetPosition() - GetWorldPosition();
	float distance = camToPlayer.Length();
	if (distance > 1500.0f) {
		Mix_Volume(mCameraSFX, 0);
	}
	else if (1500.0f >= distance && distance >= 500.0f) {
		Mix_Volume(mCameraSFX, static_cast<int>((1500 - distance) * 0.128));
	}
	else {
		Mix_Volume(mCameraSFX, 128);
	}
	if (!mAlert) {
		Mix_Resume(mCameraSFX);
		mTimer += deltaTime;
		switch (mState) {
		case CameraState::GoingRight:
			if (mTimer > mInterpTime) {
				Mix_PlayChannel(mCameraSFX, mGame->GetSound("Assets/Sounds/CameraMotorStop.wav"), 0);
				mState = CameraState::PauseOnRight;
				mTimer = 0.0f;
				SetQuat(Quaternion::Slerp(GetQuat(), mEndQ, 0.01f));
				break;
			}
			SetQuat(Quaternion::Slerp(GetQuat(), mEndQ, 0.01f));
			break;
		case CameraState::GoingLeft:
			if (mTimer > mInterpTime) {
				Mix_PlayChannel(mCameraSFX, mGame->GetSound("Assets/Sounds/CameraMotorStop.wav"), 0);
				mState = CameraState::PauseOnLeft;
				mTimer = 0.0f;
				break;
			}
			SetQuat(Quaternion::Slerp(GetQuat(), mStartQ, 0.01f));
			break;
		case CameraState::PauseOnRight:			
			if (mTimer > mPauseTime) {
				mState = CameraState::GoingLeft;
				Mix_PlayChannel(mCameraSFX, mGame->GetSound("Assets/Sounds/CameraMotor.wav"), 0);
				mTimer = 0.0f;
			}
			break;
		case CameraState::PauseOnLeft:
			if (mTimer > mPauseTime) {
				mState = CameraState::GoingRight;
				Mix_PlayChannel(mCameraSFX, mGame->GetSound("Assets/Sounds/CameraMotor.wav"), 0);
				mTimer = 0.0f;
			}
			break;
		}
	}
	else {
		Mix_Pause(mCameraSFX);
	}

}