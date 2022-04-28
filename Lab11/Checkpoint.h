#include "Actor.h"
#include <string>

class Checkpoint : public Actor
{
public:
	Checkpoint(Game* game, Actor* parent);
	void Activate(bool active);
	void OnUpdate(float deltaTime) override;
	bool GetActive() { return mActive; }
	void SetLevel(std::string level) { mLevelString = level; }
private:
	class MeshComponent* mMesh;
	class CollisionComponent* mCollision;
	bool mActive = false;
	std::string mLevelString;
};