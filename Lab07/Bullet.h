#include "Actor.h"

class Bullet : public Actor
{
public:
	Bullet(class Game* game, float speed);
	void OnUpdate(float deltaTime) override;
private:
	class MeshComponent* mMesh;
	class MoveComponent* mMove;
	class CollisionComponent* mCollision;
	float timer = 0.0f;
};