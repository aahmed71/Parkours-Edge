#pragma once
#include "Actor.h"
#include <string>
class Spawner : public Actor
{
public:
	Spawner(class Game* game, std::string type);
	void Spawn();
private:
	std::string mType;
};