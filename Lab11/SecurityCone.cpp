#include "SecurityCone.h"
#include "Game.h"
#include "Renderer.h"
#include "Player.h"
#include "SecurityCamera.h"
#include "PlayerMove.h"
#include "MeshComponent.h"

SecurityCone::SecurityCone(Game* game, Actor* actor)
	:Actor(game, actor)
{
	mMesh = new MeshComponent(this, true);
	mMesh->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Cone.gpmesh"));
	SetPosition(Vector3(30.0f, 0.0f, 0.0f));
	SetScale(3.0f);
	mCamera = (SecurityCamera*)mParent;
	mDetectedSFX = Mix_PlayChannel(Mix_GroupAvailable(1), mGame->GetSound("Assets/Sounds/SecurityDetected.wav"), 0);
	Mix_Pause(mDetectedSFX);
}

void SecurityCone::OnUpdate(float deltaTime) {
	Player* player = GetGame()->GetPlayer();
	float height = 100.0f * GetScale();
	Vector3 coneToPlayer = player->GetPosition() - GetWorldPosition();
	float ang = Math::Acos(Vector3::Dot(Vector3::Normalize(coneToPlayer), GetWorldForward()));
	if (ang < (Math::Pi / 6.0f) && coneToPlayer.Length() < height && mLightTimer > 2.0f) {		
		mCamera->SetAlert(true);
		mMesh->SetTextureIndex(2);
		mLightTimer += deltaTime;
		if (mLightTimer > 2.5f) {
			player->GetComponent<PlayerMove>()->Respawn();
		}
	}
	else if (ang < (Math::Pi / 6.0f) && coneToPlayer.Length() < height) {
		if (!mDetected) {
			Mix_Resume(mDetectedSFX);
			Mix_PlayChannel(mDetectedSFX, mGame->GetSound("Assets/Sounds/SecurityDetected.wav"), 0);
		}
		mDetected = true;
		mCamera->SetAlert(true);
		mMesh->SetTextureIndex(1);
		mLightTimer += deltaTime;
	}
	else {
		mDetected = false;
		Mix_Pause(mDetectedSFX);
		mCamera->SetAlert(false);
		mMesh->SetTextureIndex(0);
		mLightTimer = 0.0f;
	}
}