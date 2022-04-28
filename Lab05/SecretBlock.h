#pragma once
#include "Actor.h"
#include "Door.h"

class SecretBlock : public Actor 
{
	public:
		SecretBlock(class Game* game);
		class CollisionComponent* GetCollComp();
		bool DoorOpen();
	private:
		class CollisionComponent* mCollComp;
		class SpriteComponent* mSprite;
};