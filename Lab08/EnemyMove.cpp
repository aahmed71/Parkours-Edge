#include "EnemyMove.h"
#include "Game.h"
#include "Renderer.h"
#include "Actor.h"
#include "HeightMap.h"

EnemyMove::EnemyMove(Actor* owner) 
	:VehicleMove(owner)
{
	std::ifstream in("Assets/HeightMap/Path.csv");
	std::string line;
	std::getline(in, line);
	while (std::getline(in, line)) {
		std::vector<std::string> stringArr = CSVHelper::Split(line);
		int row = std::stoi(stringArr[1]);
		int col = std::stoi(stringArr[2]);
		mPoints.push_back(mOwner->GetGame()->mHeightMap->CellToWorld(row, col));
	}
	mOwner->SetPosition(mPoints[0]);
}
void EnemyMove::Update(float deltaTime) {
	Vector3 toTarget = mPoints[mPathIndex] - mOwner->GetPosition();
	// reached next node
	if (toTarget.Length() < 20.0f) {
		mPathIndex = (mPathIndex + 1) % mPoints.size();
	}
	Vector3 toTargetNorm = Vector3::Normalize(mPoints[mPathIndex] - mOwner->GetPosition());
	float targetDot = Vector3::Dot(mOwner->GetForward(), toTargetNorm);
	if (targetDot > 0.8f) {
		SetPedal(true);
	}
	else {
		SetPedal(false);
	}
	Vector3 targetCross = Vector3::Cross(mOwner->GetForward(), toTargetNorm);
	if (targetCross.z < 0.0f) {
		SetDir(Direction::Left);
	}
	else {
		SetDir(Direction::Right);
	}
	VehicleMove::Update(deltaTime);
}