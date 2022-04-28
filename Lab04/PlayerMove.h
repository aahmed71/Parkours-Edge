#pragma once
#include "MoveComponent.h"
#include <string>

class PlayerMove : public MoveComponent
{
public:
	PlayerMove(class Actor* owner, class CollisionComponent* collComp, class AnimatedSprite* sprite);
	// Update the move component
	void Update(float deltaTime) override;
	void ProcessInput(const Uint8* keyState) override;
	void UpdateAnim();
private:
	float mYSpeed;
	class CollisionComponent* mCollComp;
	class AnimatedSprite* mSprite;
	bool mSpacePressed;
	bool mInAir;
	bool dead;
};
