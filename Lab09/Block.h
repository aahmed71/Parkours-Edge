#include "Actor.h"
#include <vector>

class Block : public Actor
{
public:
	Block(class Game* game);
	virtual ~Block();
private:
	class MeshComponent* mMesh;
	class CollisionComponent* mCollision;
};