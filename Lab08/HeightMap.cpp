#include "HeightMap.h"

HeightMap::HeightMap() {
	std::ifstream in("Assets/HeightMap/HeightMap.csv");
	std::string line;
	while (std::getline(in, line)) {
		std::vector<std::string> stringArr = CSVHelper::Split(line);
		std::vector<int> intLine;
		for (auto str : stringArr) {
			intLine.push_back(std::stoi(str));
		}
		mInts.push_back(intLine);
	}
}

bool HeightMap::IsCellOnTrack(int row, int col) {
	if (mInts.size() < row + 1 || mInts[row].size() < col + 1 || mInts[row][col] == -1) {
		return false;
	}
	return true;
}

float HeightMap::GetHeightFromCell(int row, int col) {
	return -40.0f + mInts[row][col] * 5.0f;
}

Vector3 HeightMap::CellToWorld(int row, int col) {
	if (!IsCellOnTrack(row, col)) {
		return Vector3::Zero;
	}
	float x = GRID_TOP - CELL_SIZE * row;
	float y = GRID_LEFT + CELL_SIZE * col;
	float z = GetHeightFromCell(row, col);
	return Vector3(x, y, z);
}

Vector2 HeightMap::WorldToCell(int x, int y) {
	int row = (Math::Abs(x - GRID_TOP) + CELL_SIZE / 2.0f) / CELL_SIZE;
	int col = (Math::Abs(y - GRID_LEFT) + CELL_SIZE / 2.0f) / CELL_SIZE;
	return Vector2(row, col);
}

bool HeightMap::IsOnTrack(int x, int y) {
	Vector2 cell = WorldToCell(x, y);
	if (IsCellOnTrack(cell.x, cell.y)) {
		return true;
	}
	return false;
}

float HeightMap::GetHeight(int x, int y){
	if (!IsOnTrack(x, y)) {
		return -1000.0f;
	}
	Vector2 cell = WorldToCell(x, y);
	return GetHeightFromCell(cell.x, cell.y);
}