#include "Component.h"
#include "CollisionComponent.h"
#include "Math.h"

class CameraComponent : public Component
{
public:
	CameraComponent(Actor* mOwner);
	void Update(float deltaTime) override;
	float GetPitchAngle() { return mPitchAngle; }
	float GetPitchSpeed() { return mPitchSpeed; }
	void SetPitchAngle(float angle) { mPitchAngle = angle; }
	void SetPitchSpeed(float speed) { mPitchSpeed = speed; }
	float GetRollAngle() { return mRollAngle; }
	float GetRollSpeed() { return mRollSpeed; }
	void SetRollAngle(float angle) { mRollAngle = angle; }
	void SetRollSpeed(float speed) { mRollSpeed = speed; }
	// false = x, true = y
	bool XorY = false;
private:
	float mPitchAngle = 0.0f;
	float mPitchSpeed = 0.0f;
	float mRollAngle = 0.0f;
	float mRollSpeed = 0.0f;
};