#include "VehicleMove.h"
#include "Game.h"
#include "Actor.h"
#include "HeightMap.h"


VehicleMove::VehicleMove(Actor* owner)
	:Component(owner, 50)
{
	mOldZ = mOwner->GetPosition().z;
	std::ifstream in("Assets/HeightMap/Checkpoints.csv");
	std::string line;
	std::getline(in, line);
	while (std::getline(in, line)) {
		std::vector<std::string> stringArr = CSVHelper::Split(line);
		stringArr.erase(stringArr.begin());
		std::vector<int> checkPoint;
		for (auto str : stringArr) {
			checkPoint.push_back(std::stoi(str));
		}
		mCheckpoints.push_back(checkPoint);
	}

}

void VehicleMove::Update(float deltaTime) {
	// linear motion
	if (mPedal) {
		mAccelTimer += deltaTime;
		if (mAccelTimer > 1.0f) {
			mAccelTimer = 1.0f;
		}
		float AccelMag = Math::Lerp(mMinLinearAccel, mMaxLinearAccel, mAccelTimer / mAccelRamp);
		mVelocity += mOwner->GetForward() * AccelMag * deltaTime;
	}
	else {
		mAccelTimer = 0;
	}

	mOwner->SetPosition(mOwner->GetPosition() + mVelocity * deltaTime);
	float vehicleX = mOwner->GetPosition().x;
	float vehicleY = mOwner->GetPosition().y;
	
	if (mOwner->GetGame()->mHeightMap->IsOnTrack(vehicleX, vehicleY)) {
		float height = Math::Lerp(mOldZ, mOwner->GetGame()->mHeightMap->GetHeight(vehicleX, vehicleY), 0.1f);
		mOwner->SetPosition(Vector3(vehicleX, vehicleY, height));
	}
	mOldZ = mOwner->GetPosition().z;

	if (mPedal) {
		mVelocity *= mLinearDrag;

	}
	else {
		mVelocity *= mLinearDragNoPedal;
	}
	
	// turning
	float turnFactor = 0.0f;
	switch (mDir) {
	case Direction::Left:
		turnFactor = -1.0f;
		break;
	case Direction::Right:
		turnFactor = 1.0f;
		break;
	}
	if (mDir != Direction::NotTurning) {
		mAngVelocity += mAngAccel * turnFactor * deltaTime;
	}
	mOwner->SetRotation(mOwner->GetRotation() + mAngVelocity * deltaTime);
	mAngVelocity *= mAngDrag;

	// next check point
	if (mLastCheck >= mCheckpoints.size() - 1) {
		mLastCheck = -1;
	}

	if (CrossedCheckpoint(mCheckpoints[mLastCheck + 1])) {
		mLastCheck++;
		if (mLastCheck == 0) {
			SDL_Log("lapped");
			mLap++;
			OnLapChange(mLap);
		}
	}
}

int VehicleMove::GetLap() {
	return mLap;
}

int VehicleMove::GetCheckpoint() {
	return mLastCheck;
}

float VehicleMove::CheckpointDist(){
	std::vector<int> checkPoint = mCheckpoints[(mLastCheck + 1) % 8];
	Vector3 minCell = mOwner->GetGame()->mHeightMap->CellToWorld(checkPoint[0], checkPoint[2]);
	return (minCell - mOwner->GetPosition()).Length();
}

bool VehicleMove::CrossedCheckpoint(std::vector<int> checkPoint){
	Vector2 minCell = Vector2(checkPoint[0], checkPoint[2]);
	Vector2 maxCell = Vector2(checkPoint[1], checkPoint[3]);
	int x = mOwner->GetPosition().x;
	int y = mOwner->GetPosition().y;
	Vector2 playerCell = mOwner->GetGame()->mHeightMap->WorldToCell(x, y);

	// crossed
	if(playerCell.x >= minCell.x && playerCell.y >= minCell.y && playerCell.x <= maxCell.x && playerCell.y <= maxCell.y) {
		return true;
	}

	return false;
}

bool VehicleMove::GetPedal() {
	return mPedal;
}
void VehicleMove::SetPedal(bool pedal) {
	mPedal = pedal;
}

VehicleMove::Direction VehicleMove::GetDir() {
	return mDir;
}
void VehicleMove::SetDir(VehicleMove::Direction dir) {
	mDir = dir;
}