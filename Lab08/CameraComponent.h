#include "Component.h"
#include "Math.h"

class CameraComponent : public Component
{
public:
	CameraComponent(Actor* mOwner);
	void Update(float deltaTime) override;
private:
	float mSpringConst = 256.0f;
	float mDampConst;
	Vector3 mCameraPos = Vector3(0, 0, 0);
	Vector3 mCameraVel = Vector3(0, 0, 0);
	void SnapToIdeal();
};