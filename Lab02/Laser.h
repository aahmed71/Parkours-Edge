#pragma once
#include "Actor.h"
class Laser : public Actor
{
public:
	Laser(class Game* game);
	void OnUpdate(float deltaTime);
private:
	class SpriteComponent* mSprite;
	class MoveComponent* mMoveComp;
	float lifetime;
};