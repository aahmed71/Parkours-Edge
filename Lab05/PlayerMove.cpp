#include "PlayerMove.h"
#include "Actor.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "Door.h"
#include "Collider.h"
#include "SecretBlock.h"
#include "Player.h"
#include "AnimatedSprite.h"
#include "Spawner.h"
#include "Sword.h"
#include "EnemyComponent.h"

PlayerMove::PlayerMove(class Player* owner, class Actor* actor, class CollisionComponent* collComp, class AnimatedSprite* sprite)
	:MoveComponent(actor)
{
	mYSpeed = 0;
	mCollComp = collComp;
	mSprite = sprite;
	mSpacePressed = false;
	mInAir = false;
	dead = false;
	mDirection = Vector2(0, -1);
	mPlayer = owner;
	mSword = new Sword(mOwner->GetGame());
	timer = 0;
}

void PlayerMove::ProcessInput(const Uint8* keyState){

	if (keyState[SDL_SCANCODE_LEFT]) {
		mDirection = Vector2(-1, 0);
		mDirFlag = Direction::Left;
		SetForwardSpeed(130.0f);
	}
	else if (keyState[SDL_SCANCODE_RIGHT]) {
		mDirection = Vector2(1, 0);
		mDirFlag = Direction::Right;
		SetForwardSpeed(130.0f);
	}
	else if (keyState[SDL_SCANCODE_UP]){
		mDirection = Vector2(0, -1);
		mDirFlag = Direction::Up;
		SetForwardSpeed(130.0f);
	}
	else if (keyState[SDL_SCANCODE_DOWN]) {
		mDirection = Vector2(0, 1);
		mDirFlag = Direction::Down;
		SetForwardSpeed(130.0f);
	}
	else {
		SetForwardSpeed(0);
	}

	if (keyState[SDL_SCANCODE_SPACE] && !mOwner->GetGame()->getPrevState()[SDL_SCANCODE_SPACE] && timer <= 0) {
		timer = 0.25f;
		UpdateSword();
	}
}

void PlayerMove::Update(float deltaTime){
	std::string currRoom = mOwner->GetGame()->GetCurrentRoom();
	std::unordered_map roomMap = mOwner->GetGame()->GetDoorMap();
	std::unordered_map spawnMap = mOwner->GetGame()->GetSpawnerMap();
	std::unordered_map enemyMap = mOwner->GetGame()->GetEnemyMap();
	Vector2 secOffset;
	if (mOwner->GetGame()->GetSecretBlock(currRoom) != nullptr) {
		CollSide secSide = mCollComp->GetMinOverlap(mOwner->GetGame()->GetSecretBlock(currRoom)->GetCollComp(), secOffset);
		if (secSide != CollSide::None) {
			switch (secSide) {
			case CollSide::Bottom:
				if (!mOwner->GetGame()->GetSecretBlock(currRoom)->DoorOpen() && mDirection.y == -1) {
					SetForwardSpeed(GetForwardSpeed() / 2);
					mOwner->GetGame()->GetSecretBlock(currRoom)->SetPosition(mOwner->GetGame()->GetSecretBlock(currRoom)->GetPosition() + deltaTime * GetForwardSpeed() * mDirection);
				}
				else {
					mOwner->SetPosition(mOwner->GetPosition() + secOffset);
				}

				break;
			default:
				mOwner->SetPosition(mOwner->GetPosition() + secOffset);
				break;
			}

		}
	}
	if (timer > 0) {
		timer -= deltaTime;
	}
	else {
		mOwner->SetPosition(mOwner->GetPosition() + deltaTime * GetForwardSpeed() * mDirection);
	}
	
	for (auto enemy : enemyMap[currRoom]) {
		if (mSword->GetCollComp()->Intersect(enemy->GetComponent<CollisionComponent>())) {
			enemy->GetComponent<EnemyComponent>()->TakeDamage(1);
		}
	}

	Vector2 offset;
	for (auto door : roomMap[currRoom]) {
		CollSide side = mCollComp->GetMinOverlap(door->GetCollComp(), offset);
		if (side != CollSide::None && door->GetDirection() == mDirFlag && door->GetDoorState() == State::Locked && mPlayer->GetKeyCount() != 0) {
			mPlayer->SetKeyCount(-1);
			door->SetDoorState(State::Open);
			door->UpdateTexture();
		}
		if (side != CollSide::None && door->GetDirection() == mDirFlag && door->GetDoorState() == State::Open) {
			mOwner->GetGame()->SetCurrentRoom(door->GetDestination());
			switch (door->GetDirection()) {
				case Direction::Right:
					mOwner->SetPosition(mOwner->GetPosition() + Vector2(128,0));
					break;
				case Direction::Left:
					mOwner->SetPosition(mOwner->GetPosition() + Vector2(-128, 0));
					break;
				case Direction::Up:
					mOwner->SetPosition(mOwner->GetPosition() + Vector2(0, -128));
					break;
				case Direction::Down:
					mOwner->SetPosition(mOwner->GetPosition() + Vector2(0, 128));
					break;
			}
			for (auto spawner : spawnMap[door->GetDestination()]) {
				spawner->Spawn();
			}
			for (auto enemy : enemyMap[currRoom]) {
				enemy->SetState(ActorState::Paused);
			}
			for (auto enemy : enemyMap[door->GetDestination()]) {
				enemy->SetState(ActorState::Active);
			}
		}

	}

	for (auto collider : mOwner->GetGame()->GetColliders()) {
		Vector2 offset;
		CollSide side = mCollComp->GetMinOverlap(collider->GetCollComp(), offset);
		if (side != CollSide::None) {
			mOwner->SetPosition(mOwner->GetPosition() + offset);
		}
	}



	UpdateAnim();

	mOwner->GetGame()->SetCameraPos(Vector2(mOwner->GetPosition().x - WINDOW_WIDTH / 2, 
		mOwner->GetPosition().y - WINDOW_HEIGHT / 2));
	UpdateSword();
}

void PlayerMove::UpdateAnim() {
	float x = mDirection.x;
	float y = mDirection.y;

	mSprite->SetIsPaused(false);
	if (x == 1) {
		mSprite->SetAnimation("walkRight");
	}
	else if (x == -1) {
		mSprite->SetAnimation("walkLeft");
	}

	else if (y == 1) {
		mSprite->SetAnimation("walkDown");
	}
	else if (y == -1) {
		mSprite->SetAnimation("walkUp");
	}
	if (GetForwardSpeed() == 0) {
		mSprite->SetIsPaused(true);
	 }
}

void PlayerMove::UpdateSword() {
	if (timer <= 0) {
		mSword->GetSprite()->SetIsVisible(false);
	}
	else {
		mSword->GetSprite()->SetIsVisible(true);
		switch (mDirFlag) {
		case Direction::Right:
			mSword->GetSprite()->SetTexture(mOwner->GetGame()->GetTexture("Assets/Sword/Right.png"));
			mSword->SetPosition(mOwner->GetPosition() + Vector2(16, 4));
			break;
		case Direction::Left:
			mSword->GetSprite()->SetTexture(mOwner->GetGame()->GetTexture("Assets/Sword/Left.png"));
			mSword->SetPosition(mOwner->GetPosition() + Vector2(-16, 4));
			break;
		case Direction::Up:
			mSword->GetSprite()->SetTexture(mOwner->GetGame()->GetTexture("Assets/Sword/Up.png"));
			mSword->SetPosition(mOwner->GetPosition() + Vector2(-8, -16));
			break;
		case Direction::Down:
			mSword->GetSprite()->SetTexture(mOwner->GetGame()->GetTexture("Assets/Sword/Down.png"));
			mSword->SetPosition(mOwner->GetPosition() + Vector2(8, 16));
			break;
		}
	}
}