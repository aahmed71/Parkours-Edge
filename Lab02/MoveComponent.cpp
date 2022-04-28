#include "MoveComponent.h"
#include "Actor.h"

MoveComponent::MoveComponent(class Actor* owner)
:Component(owner, 50)
,mAngularSpeed(0.0f)
,mForwardSpeed(0.0f)
{
	
}

void MoveComponent::Update(float deltaTime)
{
	mOwner->SetRotation(mOwner->GetRotation() + (mAngularSpeed * deltaTime));
	Vector2 velocity = mOwner->GetForward() * mForwardSpeed;
	mOwner->SetPosition(mOwner->GetPosition() + velocity * deltaTime);
}