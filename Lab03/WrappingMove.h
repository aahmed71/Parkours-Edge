#pragma once
#include "MoveComponent.h"
#include "Math.h"

class WrappingMove : public MoveComponent
{
public:
	WrappingMove(class Actor* owner, Vector2 dir);
	// Update the move component
	void Update(float deltaTime) override;
	Vector2 GetOwnerDirection();
private:
	Vector2 mOwnerDirection;
};
