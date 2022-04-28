#pragma once
#include <string>
#include <queue>

class LevelLoader
{
public:
	static bool Load(class Game* game, const std::string& fileName);
};
