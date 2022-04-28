#include "WrappingMove.h"
#include "Actor.h"
#include "Game.h"

WrappingMove::WrappingMove(class Actor* owner, Vector2 dir)
	:MoveComponent(owner)
{
	mOwnerDirection = dir;
}

void WrappingMove::Update(float deltaTime)
{
	Vector2 newPos = mOwner->GetPosition() + mOwnerDirection * GetForwardSpeed() * deltaTime;
	// goes off left
	if (newPos.x < 0) {
		newPos.x = static_cast<float>(WINDOW_WIDTH);
	}
	// goes off right
	if (newPos.x > WINDOW_WIDTH) {
		newPos.x = 0;
	}
	mOwner->SetPosition(newPos);
}

Vector2 WrappingMove::GetOwnerDirection() {
	return mOwnerDirection;
}