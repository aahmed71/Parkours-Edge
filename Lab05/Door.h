#pragma once
#include "Actor.h"
#include <string>
enum class Direction
{
	Up,
	Down,
	Left,
	Right
};

enum class State
{
	Open,
	Closed,
	Locked
};

class Door : public Actor 
{
public:
	Door(class Game* game, std::string dest, std::string dir, std::string state);
	class CollisionComponent* GetCollComp();
	void UpdateTexture();
	std::string GetDestination();
	void SetDoorState(State);
	State GetDoorState();
	Direction GetDirection();
private:
	class SpriteComponent* mSprite;
	class CollisionComponent* mCollComp;
	std::string mDest;
	Direction mDir;
	State mState;
};