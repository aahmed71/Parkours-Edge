#include "Actor.h"

class Player : public Actor 
{
public:
	Player(class Game* owner);
private:
	class MeshComponent* mMesh;
	class CollisionComponent* mCollision;
	class PlayerMove* mMove;
};