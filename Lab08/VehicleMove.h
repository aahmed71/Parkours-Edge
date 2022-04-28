#include "Component.h"
#include "Math.h"
#include <fstream>
#include <vector>

class VehicleMove : public Component
{
public:
	VehicleMove(class Actor* owner);
	void Update(float deltaTime) override;
	bool GetPedal();
	void SetPedal(bool pedal);
	virtual void OnLapChange(int newLap) {}
	enum Direction
	{
		Left,
		Right,
		NotTurning
	};
	Direction GetDir();
	void SetDir(Direction dir);
	float CheckpointDist();
	int GetLap();
	int GetCheckpoint();
private:
	bool CrossedCheckpoint(std::vector<int> checkPoint);
	bool mPedal = false;
	Direction mDir = NotTurning;
	Vector3 mVelocity = Vector3(0,0,0);
	float mAngVelocity = 0.0f;
	float mAccelTimer = 0.0f;
	float mMinLinearAccel = 1000.0f;
	float mMaxLinearAccel = 2500.0f;
	float mAccelRamp = 2.0f;
	float mAngAccel = 5 * Math::Pi;
	float mLinearDrag = 0.9f;
	float mLinearDragNoPedal = 0.975f;
	float mAngDrag = 0.9f;
	float mOldZ;
	std::vector<std::vector<int>> mCheckpoints;
	int mLap = 0;
	int mLastCheck = -1;
};