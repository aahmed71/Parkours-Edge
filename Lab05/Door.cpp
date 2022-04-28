#include "Door.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "Game.h"

Door::Door(class Game* game, std::string dest, std::string dir, std::string state)
	:Actor(game)
{
	mDest = dest;
	std::string texture = "Assets/Door/" + dir + state + ".png";
	mSprite = new SpriteComponent(this);
	mSprite->SetTexture(mGame->GetTexture(texture));
	mCollComp = new CollisionComponent(this);
	if (dir == "Up") {
		mDir = Direction::Up;
		mCollComp->SetSize(30.0f, 64.0f);
	}
	else if (dir == "Down") {
		mDir = Direction::Down;
		mCollComp->SetSize(30.0f, 64.0f);
	}
	else if (dir == "Left") {
		mDir = Direction::Left;
		mCollComp->SetSize(64.0f, 30.0f);
	}
	else if (dir == "Right") {
		mDir = Direction::Right;
		mCollComp->SetSize(64.0f, 30.0f);
	}

	if (state == "Open") {
		mState = State::Open;
	}
	else if (state == "Closed") {
		mState = State::Closed;
	}
	else if (state == "Locked") {
		mState = State::Locked;
	}

}

void Door::UpdateTexture() {
	std::string tex = "Assets/Door/";

	// update dir
	switch (mDir) {
	case Direction::Up:
		tex += "Up";
		break;
	case Direction::Down:
		tex += "Down";
		break;
	case Direction::Left:
		tex += "Left";
		break;
	case Direction::Right:
		tex += "Right";
		break;
	}

	// update state
	switch (mState) {
		case State::Open:
			tex += "Open.png";
			break;
		case State::Closed:
			tex += "Closed.png";
			break;
		case State::Locked:
			tex += "Locked.png";
			break;
	}

	mSprite->SetTexture(mGame->GetTexture(tex));
}

std::string Door::GetDestination() {
	return mDest;
}

CollisionComponent* Door::GetCollComp() {
	return mCollComp;
}

Direction Door::GetDirection() {
	return mDir;
}

State Door::GetDoorState() {
	return mState;
}

void Door::SetDoorState(State newState) {
	mState = newState;
}