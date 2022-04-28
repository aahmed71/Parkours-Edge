#include "Log.h"
#include "SpriteComponent.h"
#include "WrappingMove.h"
#include "CollisionComponent.h"
#include "Math.h"
#include "Game.h"

Log::Log(Game* game, char logType, Vector2 dir)
	:Actor(game)
{
	mSprite = new SpriteComponent(this);
	std::string texture = "";
	float size;
	// choose texture for type of car
	switch (logType) {
	case 'X':
		texture = "Assets/LogX.png";
		size = 192.0f;
		break;
	case 'Y':
		texture = "Assets/LogY.png";
		size = 256.0f;
		break;
	case 'Z':
		texture = "Assets/LogZ.png";
		size = 384.0f;
		break;
	}
	mSprite->SetTexture(mGame->GetTexture(texture));
	mMoveComp = new WrappingMove(this, dir);
	mMoveComp->SetForwardSpeed(75.0f);
	mCollComp = new CollisionComponent(this);
	mCollComp->SetSize(size, 48.0f);
	mGame->AddLog(this);
}


CollisionComponent* Log::GetCollComp() {
	return mCollComp;
}

WrappingMove* Log::GetMoveComp() {
	return mMoveComp;
}
