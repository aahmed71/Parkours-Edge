#include "Actor.h"

enum class CameraState
{
	GoingRight, 
	PauseOnRight, 
	GoingLeft, 
	PauseOnLeft,
};

class SecurityCamera : public Actor
{
public:
	SecurityCamera(class Game* game, class Actor* player);
	void SetStartQ(Quaternion quat) { mStartQ = quat; }
	void SetEndQ(Quaternion quat) { mEndQ = quat; }
	void Init() { SetQuat(mStartQ); }
	void SetInterpTime(float time) { mInterpTime = time; }
	void SetPauseTime(float time) { mPauseTime = time; }
	void OnUpdate(float deltaTime) override;
	void SetAlert(bool alert) { mAlert = alert; }
private:
	class MeshComponent* mMesh;
	class SecurityCone* mCone;
	Quaternion mStartQ;
	Quaternion mEndQ;
	float mInterpTime;
	float mPauseTime;
	float mTimer = 0.0f;
	CameraState mState = CameraState::GoingRight;
	bool mAlert = false;
	int mCameraSFX = -1;
};