#include "MoveComponent.h"
#include "Math.h"
#include <stdlib.h>
#include "CollisionComponent.h"

class PlayerMove : public MoveComponent
{
public:
	enum MoveState
	{
		OnGround,
		Jump,
		Falling
	};
	PlayerMove(class Actor* owner);
	void Update(float deltaTime) override;
	void ProcessInput(const Uint8* keyState) override;
	void ChangeState(MoveState state) { mCurrentState = state; };
	void PhysicsUpdate(float deltaTime);
	void AddForce(const Vector3& force) { mPendingForces += force; };
protected:
	void UpdateOnGround(float deltaTime);
	void UpdateJump(float deltaTime);
	void UpdateFalling(float deltaTime);
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
};