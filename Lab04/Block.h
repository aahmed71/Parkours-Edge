#pragma once
#include "Actor.h"
class Block : public Actor
{
public:
	Block(class Game* game, char blockType);
	class CollisionComponent* GetCollComp();
private:
	class SpriteComponent* mSprite;
	class CollisionComponent* mCollComp;
};