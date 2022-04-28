#include "VehicleMove.h"
#include "Math.h"
#include "CSVHelper.h"
#include <iostream>
#include <fstream>
#include <vector>

class EnemyMove : public VehicleMove
{
public:
	EnemyMove(Actor* owner);
	void Update(float deltaTime) override;
private:
	std::vector<Vector3> mPoints;
	int mPathIndex = 1;
};