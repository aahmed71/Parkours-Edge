#pragma once
#include "Actor.h"
class Ship : public Actor
{
public:
	Ship(class Game* game);
	void OnProcessInput(const Uint8* keyState);
	void OnUpdate(float deltaTime);
private:
	class SpriteComponent* mSprite;
	class MoveComponent* mMoveComp;
	float lastLaserTime;
};