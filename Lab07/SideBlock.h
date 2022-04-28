#include "Actor.h"

class SideBlock : public Actor 
{
public:
	SideBlock(class Game* game, int index);
	void OnUpdate(float deltaTime) override;
private:
	class MeshComponent* mMesh;
};