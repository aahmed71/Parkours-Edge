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
	Vector3 forwardVel = mOwner->GetForward() * mForwardSpeed;
	mOwner->SetPosition(mOwner->GetPosition() + forwardVel * deltaTime);
	Vector3 strafeVel = mOwner->GetRight() * mStrafeSpeed;
	mOwner->SetPosition(mOwner->GetPosition() + strafeVel * deltaTime);
}
