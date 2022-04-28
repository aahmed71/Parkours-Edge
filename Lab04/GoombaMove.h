#pragma once
#include "MoveComponent.h"


class GoombaMove : public MoveComponent
{
public:
	GoombaMove(class Actor* owner, class CollisionComponent* collComp);
	// Update the move component
	void Update(float deltaTime) override;
	void SetDeath(bool death);
	bool GetDeath();
private:
	float mYSpeed;
	class CollisionComponent* mCollComp;
	bool dead;
	float lifetime;
};
