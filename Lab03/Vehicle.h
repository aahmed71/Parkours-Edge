#pragma once
#include "Actor.h"
class Vehicle : public Actor
{
public:
	Vehicle(class Game* game, char vehicleType, Vector2 dir);
	virtual ~Vehicle();
	class CollisionComponent* GetCollComp();
	void OnUpdate(float deltaTime) override;
private:
	class SpriteComponent* mSprite;
	class WrappingMove* mMoveComp;
	// collision component
	class CollisionComponent* mCollComp;
};