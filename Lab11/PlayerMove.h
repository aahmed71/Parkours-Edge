#include "MoveComponent.h"
#include "Math.h"
#include <stdlib.h>
#include "CollisionComponent.h"
#include <queue>

enum class MoveState
{
	OnGround,
	Jump,
	Falling,
	WallClimb,
	WallRun
};

class PlayerMove : public MoveComponent
{
public:

	PlayerMove(class Actor* owner);
	virtual ~PlayerMove();
	void Update(float deltaTime) override;
	void ProcessInput(const Uint8* keyState) override;
	void ChangeState(MoveState state) { mCurrentState = state; };
	MoveState GetState() { return mCurrentState; }
	void PhysicsUpdate(float deltaTime);
	void AddForce(const Vector3& force) { mPendingForces += force; };
	void AddCheckpoint(class Checkpoint* checkpoint) { mCheckpoints.push(checkpoint); }
	void Respawn();
protected:
	void UpdateOnGround(float deltaTime);
	void UpdateJump(float deltaTime);
	void UpdateFalling(float deltaTime);
	void UpdateWallClimb(float deltaTime);
	void UpdateWallRun(float deltaTime);
	bool CanWallClimb(CollSide side);
	bool CanWallRun(CollSide side);
	void FixXYVelocity();
	CollSide FixCollision(class CollisionComponent* self, class CollisionComponent* block);
private:
	bool mSpacePressed = false;
	MoveState mCurrentState;
	Vector3 mVelocity;
	Vector3 mAcceleration;
	Vector3 mPendingForces;
	float mMass = 1.0f;
	Vector3 mGravity = Vector3(0.0f, 0.0, -980.0f);
	Vector3 mJumpForce = Vector3(0.0f, 0.0f, 35000.0f);
	Vector3 mClimbForce = Vector3(0.0f, 0.0f, 1800.0f);
	Vector3 mWallRunForce = Vector3(0.0f, 0.0f, 1200.0f);
	float mWallClimbTimer;
	float mWallRunTimer;
	std::queue<class Checkpoint*> mCheckpoints;
	int mRunningSFX;
};