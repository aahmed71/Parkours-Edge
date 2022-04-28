#include "Actor.h"

class Block : public Actor 
{
public:
	Block(class Game* game, bool exploding);
	virtual ~Block();
	void OnUpdate(float deltaTime) override;
	bool GetExplosion();
	bool GetExploded();
	void Explode();
private:
	class MeshComponent* mMesh;
	class CollisionComponent* mCollision;
	bool mExploding;
	bool mExploded = false;
};