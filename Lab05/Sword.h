#include "Actor.h"

class Sword : public Actor
{
public:
	Sword(Game* game);
	class SpriteComponent* GetSprite();
	class CollisionComponent* GetCollComp();
private:
	class SpriteComponent* mSprite;
	class CollisionComponent* mCollComp;
};