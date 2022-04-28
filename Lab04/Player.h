#pragma once
#include "Actor.h"
class Player : public Actor
{
public:
	Player(class Game* game);
	class CollisionComponent* GetCollComp();
private:
	class AnimatedSprite* mSprite;
	class CollisionComponent* mCollComp;
	class PlayerMove* mPlayerMove;
};