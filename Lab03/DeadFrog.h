#pragma once
#include "Actor.h"
class DeadFrog : public Actor
{
public:
	DeadFrog(class Game* game, Vector2 pos);
	void OnUpdate(float deltaTime) override;
private:
	// sprite component
	class SpriteComponent* mSprite;
	// starting position
	Vector2 mPos;
	float lifetime;
};