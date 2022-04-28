#include "HUD.h"
#include "Game.h"
#include "Actor.h"
#include "Shader.h"
#include "Texture.h"
#include "Font.h"
#include "Renderer.h"
#include "SecurityCamera.h"


HUD::HUD(Actor* owner)
	:UIComponent(owner)
{
	mFont = new Font();
	mFont->Load("Assets/Inconsolata-Regular.ttf");
	mTimerText = mFont->RenderText("00:00.00");
	mCoinText = mFont->RenderText(std::to_string(mOwner->GetGame()->GetCoins()) + "/55");
	mCheckpointText = mFont->RenderText(mOwner->GetGame()->GetText());
	Renderer* renderer = mOwner->GetGame()->GetRenderer();
	mRadar = renderer->GetTexture("Assets/Radar.png");
	mRadarArrow = renderer->GetTexture("Assets/RadarArrow.png");
	mBlip = renderer->GetTexture("Assets/Blip.png");
}

HUD::~HUD() {
	mFont->Unload();
	delete mFont;
}

void HUD::Update(float deltaTime) {
	// timer
	mTextTimer += deltaTime;
	if (mTextTimer > 5.0f) {
		RenderCheckpoint(" ");
		mTextTimer = 0.0f;
	}
	UpdateTime();
}

void HUD::Draw(Shader* shader) {
	DrawTexture(shader, mTimerText, mTimerPos);
	DrawTexture(shader, mCoinText, mCoinPos);
	DrawTexture(shader, mCheckpointText, mCheckpointPos);
	DrawTexture(shader, mRadar, mRadarPos);
	DrawTexture(shader, mRadarArrow, mRadarPos);
	DrawBlips(shader);
}

void HUD::UpdateTime() {
	float timer = mOwner->GetGame()->GetTimer();
	mTimerText->Unload();
	delete mTimerText;
	std::stringstream ss1;
	int min = static_cast<int>(timer / 60);
	ss1 << std::setw(2) << std::setfill('0') << min;
	std::string minString = ss1.str();

	float second = timer - (min * 60);
	std::stringstream ss2;
	ss2 << std::setw(5) << std::setfill('0') << std::fixed << std::setprecision(2) << second;
	std::string time = minString + ":" + ss2.str();
	mTimerText = mFont->RenderText(time);
}

void HUD::AddCoin() {
	mOwner->GetGame()->AddCoin();
	mCoinText->Unload();
	delete mCoinText;
	mCoinText = mFont->RenderText(std::to_string(mOwner->GetGame()->GetCoins()) + "/55");
}

void HUD::RenderCheckpoint(std::string text) {
	mTextTimer = 0.0f;
	mCheckpointText->Unload();
	delete mCheckpointText;
	mCheckpointText = mFont->RenderText(text);
}



void HUD::DrawBlips(Shader* shader) {
	for (SecurityCamera* cam : mOwner->GetGame()->GetCameras()) {
		Vector3 playerPos = mOwner->GetWorldPosition();
		Vector2 player2D = Vector2(playerPos.y, playerPos.x);
		Vector3 cameraPos = cam->GetWorldPosition();
		Vector2 camera2D = Vector2(cameraPos.y, cameraPos.x);
		Vector2 playerToCamera = camera2D - player2D;
		if (playerToCamera.Length() <= mRadarRange) {
			Vector3 playerForward = mOwner->GetForward();
			float ang = atan2(playerForward.y, playerForward.x);
			Matrix3 rotation = Matrix3::CreateRotation(ang);
			playerToCamera.x /= mRadarRange;
			playerToCamera.y /= mRadarRange;
			playerToCamera *= mRadarRadius;
			playerToCamera = Vector2::Transform(playerToCamera, rotation);
			DrawTexture(shader, mBlip, playerToCamera + mRadarPos, 1.0f, ang);
		}	
	}
}