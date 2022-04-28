#include "Actor.h"
#include <vector>

class Block : public Actor
{
public:
	Block(class Game* game, Actor* parent);
	virtual ~Block();
	bool GetMirror() { return mMirror; }
	void SetMirror(bool mirror) { mMirror = mirror; }
	bool GetRotating() { return mRotating; }
	void SetRotating(bool rotating) { mRotating = rotating; }
	void OnUpdate(float deltaTime) override;
private:
	class MeshComponent* mMesh;
	class CollisionComponent* mCollision;
	bool mMirror = false;
	bool mRotating = false;
};