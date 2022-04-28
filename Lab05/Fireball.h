#include "Actor.h"

class Fireball : public Actor
{
public:
	Fireball(class Game* game);
	void OnUpdate(float deltaTime) override;
private:
	class SpriteComponent* mSprite;
	class CollisionComponent* mCollComp;
	class MoveComponent* mMoveComp;
	float lifetime;
};