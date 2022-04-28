#include "Spawner.h"
#include "Math.h"
#include "Game.h"
#include "Skeleton.h"
#include "Key.h"


Spawner::Spawner(Game* game, std::string type)
	:Actor(game)
{
	mType = type;
}

void Spawner::Spawn() {
	if (mType == "Key") {
		Key* temp = new Key(mGame);
		temp->SetPosition(GetPosition());
		SetState(ActorState::Destroy);
	}
	else if (mType == "Skeleton") {
		Skeleton* temp = new Skeleton(mGame, mGame->GetCurrentRoom());
		temp->SetPosition(GetPosition());
		SetState(ActorState::Destroy);
	}
}
