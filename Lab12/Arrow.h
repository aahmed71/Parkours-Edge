#include "Actor.h"

class Arrow : public Actor 
{
public:
	Arrow(Game* game, Actor* parent);
	void OnUpdate(float deltaTime) override;
private:
	class MeshComponent* mMesh;

};