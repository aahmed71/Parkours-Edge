#pragma once
#include "Actor.h"
class Log : public Actor
{
public:
	Log(class Game* game, char logType, Vector2 dir);
	//void OnUpdate(float deltaTime) override;
	class CollisionComponent* GetCollComp();
	class WrappingMove* GetMoveComp();
private:
	class SpriteComponent* mSprite;
	class WrappingMove* mMoveComp;
	class CollisionComponent* mCollComp;
};