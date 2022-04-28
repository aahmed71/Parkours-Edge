#pragma once
#include "Actor.h"
class Goomba : public Actor
{
public:
	Goomba(class Game* game);
	virtual ~Goomba();
	class CollisionComponent* GetCollComp();
	class AnimatedSprite* GetSpriteComp();
	class GoombaMove* GetMoveComp();
private:
	class AnimatedSprite* mSprite;
	class CollisionComponent* mCollComp;
	class GoombaMove* mGoombaMove;
};

