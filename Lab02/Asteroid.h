#pragma once
#include "Actor.h"
class Asteroid : public Actor
{
public:
	Asteroid(class Game* game);
	virtual ~Asteroid();
	void OnUpdate(float deltaTime);
private:
	class SpriteComponent* mSprite;
	class MoveComponent* mMoveComp;
};