#include "Component.h"
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
private:
	float mPitchAngle = 0.0f;
	float mPitchSpeed = 0.0f;
};