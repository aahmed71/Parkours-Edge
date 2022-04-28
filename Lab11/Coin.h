#include "Actor.h"

class Coin : public Actor
{
public:
	Coin(class Game* game, class Actor* owner);
	void OnUpdate(float deltaTime) override;
private:
	class MeshComponent* mMesh;
	class CollisionComponent* mCollision;
};