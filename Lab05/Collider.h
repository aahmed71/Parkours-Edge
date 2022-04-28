#pragma once
#include "Actor.h"
class Collider : public Actor
{
public:
	Collider(class Game* game, int width, int height);
	void SetCollComp(int width, int height);
	class CollisionComponent* GetCollComp();
private:
	class CollisionComponent* mCollComp;
};