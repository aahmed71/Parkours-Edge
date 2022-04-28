#pragma once
#include "MoveComponent.h"
#include "Math.h"
#include <string>
#include "Door.h"
#include <map>
#include <vector>

class PlayerMove : public MoveComponent
{
public:
	PlayerMove(class Player* owner, class Actor* actor, class CollisionComponent* collComp, class AnimatedSprite* sprite);
	// Update the move component
	void Update(float deltaTime) override;
	void ProcessInput(const Uint8* keyState) override;
	void UpdateAnim();
	void UpdateSword();
private:
	float mYSpeed;
	class CollisionComponent* mCollComp;
	class AnimatedSprite* mSprite;
	bool mSpacePressed;
	bool mInAir;
	bool dead;
	Vector2 mDirection;
	Direction mDirFlag;
	int mKeyCount;
	class Player* mPlayer;
	class Sword* mSword;
	float timer;
};
