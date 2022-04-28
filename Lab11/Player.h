#include "Actor.h"

class Player : public Actor 
{
public:
	Player(class Game* owner, Actor* parent);
	void SetRespawnPos(Vector3 pos) { mRespawn = pos; }
	Vector3 GetRespawnPos() { return mRespawn; }
private:
	class CollisionComponent* mCollision;
	class PlayerMove* mMove;
	class CameraComponent* mCamera;
	Vector3 mRespawn;
};