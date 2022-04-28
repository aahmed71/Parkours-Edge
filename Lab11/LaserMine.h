#include "Actor.h"

class LaserMine : public Actor
{
public:
	LaserMine(class Game* game, Actor* parent);
private:
	class MeshComponent* mMesh;
	class LaserComponent* mLaserMesh;
};