#include "VehicleMove.h"
#include "Math.h"
#include <stdlib.h>

class PlayerMove : public VehicleMove 
{
public:
	PlayerMove(class Actor* owner);
	void Update(float deltaTime) override;
	void ProcessInput(const Uint8* keyState) override;
	void OnLapChange(int newLap) override;
private:
	Vector3 mVelocity;
	int mBlockX = 2000;
	int mSideBlockX = 2500;
	bool mSpacePressed = false;
	float mSpeedMultiplier = 1.0f;
	float mSpeedTimer = 0;
};