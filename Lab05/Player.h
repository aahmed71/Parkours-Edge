#pragma once
#include "Actor.h"
class Player : public Actor
{
public:
	Player(class Game* game);
	class CollisionComponent* GetCollComp();
	int GetKeyCount();
	void SetKeyCount(int key);
	void TakeDamage(int amount);
private:
	class AnimatedSprite* mSprite;
	class CollisionComponent* mCollComp;
	class PlayerMove* mPlayerMove;
	int mKeyCount;
	int mHP;
};