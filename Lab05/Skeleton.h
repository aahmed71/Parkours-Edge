#include "Actor.h"
#include "Math.h"
#include <string>
class Skeleton : public Actor
{
public:
	Skeleton(class Game* game, std::string room);
	void OnUpdate(float deltaTime) override;
private:
	class AnimatedSprite* mSprite;
	class CollisionComponent* mCollComp;
	class EnemyComponent* mEnemyComp;
	float timer;
};