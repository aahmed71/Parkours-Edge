#include "SecretBlock.h"
#include "CollisionComponent.h"
#include "Door.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "Collider.h"

SecretBlock::SecretBlock(class Game* game)
	:Actor(game)
{
	mSprite = new SpriteComponent(this);
	mSprite->SetTexture(mGame->GetTexture("Assets/SecretBlock.png"));
	mCollComp = new CollisionComponent(this);
	mCollComp->SetSize(32, 32);
}

bool SecretBlock::DoorOpen() {
	for (auto collider : mGame->GetColliders()) {
		Vector2 offset;
		CollSide side = mCollComp->GetMinOverlap(collider->GetCollComp(), offset);
		if (side == CollSide::Bottom) {
			SetPosition(GetPosition() + offset);
			std::vector<Door*> doors;
			std::unordered_map<std::string, std::vector<class Door*>> temp = mGame->GetDoorMap();
			doors = temp[mGame->GetCurrentRoom()];
			for (auto door : doors) {
				if (door->GetDoorState() == State::Closed) {
					door->SetDoorState(State::Open);
					door->UpdateTexture();
				}
			}
			return true;
		}
	}
	return false;
}

CollisionComponent* SecretBlock::GetCollComp() {
	return mCollComp;
}

