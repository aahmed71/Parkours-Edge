#include "Actor.h"

class Enemy : public Actor
{
public:
	Enemy(Game* game);
private:
	class MeshComponent* mMesh;
	class EnemyMove* mMove;
};