#pragma once
#include "Actor.h"
class Frog : public Actor
{
public:
	Frog(class Game* game, Vector2 start);
	void OnProcessInput(const Uint8* keyState);
	void OnUpdate(float deltaTime) override;
private:
	// sprite component
	class SpriteComponent* mSprite;
	// movement componeont
	class WrappingMove* mMoveComp;
	// collision component
	class CollisionComponent* mCollComp;
	// starting position
	Vector2 mStart;
};