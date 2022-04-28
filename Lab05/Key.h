#pragma once
#include "Actor.h"

class Key : public Actor
{
	public:
		Key(class Game* game);
		class SpriteComponent* GetSprite();
		class CollisionComponent* GetCollComp();
	private:
		class SpriteComponent* mSprite;
		class CollisionComponent* mCollComp;
};