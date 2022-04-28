#include <vector>
#include <iostream>
#include <fstream>
#include "CSVHelper.h"
#include "Math.h"

class HeightMap 
{
public:
	HeightMap();
	Vector3 CellToWorld(int row, int col);
	Vector2 WorldToCell(int x, int y);
	bool IsOnTrack(int x, int y);
	float GetHeight(int x, int y);
private:
	bool IsCellOnTrack(int row, int col);
	float GetHeightFromCell(int row, int col);
	std::vector<std::vector<int>> mInts;
	const float CELL_SIZE = 40.55f;
	const float GRID_TOP = 1280.0f;
	const float GRID_LEFT = -1641.0f;
};