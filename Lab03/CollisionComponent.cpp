#include "CollisionComponent.h"
#include "Actor.h"

CollisionComponent::CollisionComponent(class Actor* owner)
:Component(owner)
,mWidth(0.0f)
,mHeight(0.0f)
{
	
}

CollisionComponent::~CollisionComponent()
{
	
}

bool CollisionComponent::Intersect(const CollisionComponent* other)
{
	bool case1 = GetMax().x < other->GetMin().x;
	bool case2 = other->GetMax().x < GetMin().x;
	bool case3 = GetMax().y < other->GetMin().y;
	bool case4 = other->GetMax().y < GetMin().y;
	return !(case1 || case2 || case3 || case4);
}

Vector2 CollisionComponent::GetMin() const
{
	Vector2 min;
	min.x = GetCenter().x - (mWidth * mOwner->GetScale()) / 2.0f;
	min.y = GetCenter().y - (mHeight * mOwner->GetScale()) / 2.0f;
	return min;
}

Vector2 CollisionComponent::GetMax() const
{
	Vector2 max;
	max.x = GetCenter().x + (mWidth * mOwner->GetScale()) / 2.0f;
	max.y = GetCenter().y + (mHeight * mOwner->GetScale()) / 2.0f;
	return max;
}

const Vector2& CollisionComponent::GetCenter() const
{
	return mOwner->GetPosition();
}

CollSide CollisionComponent::GetMinOverlap(
	const CollisionComponent* other, Vector2& offset)
{
	offset = Vector2::Zero;
	// TODO: Implement
	if (!Intersect(other)) {
		return CollSide::None;
	}

	float minYDiff = Math::Abs(other->GetMin().y - GetMax().y);
	float maxYDiff = Math::Abs(other->GetMax().y - GetMin().y);
	float minXDiff = Math::Abs(other->GetMin().x - GetMax().x);
	float maxXDiff = Math::Abs(other->GetMax().x - GetMin().x);
	float absMinYDiff = Math::Abs(other->GetMin().y - GetMax().y);
	float absMaxYDiff = Math::Abs(other->GetMax().y - GetMin().y);
	float absMinXDiff = Math::Abs(other->GetMin().x - GetMax().x);
	float absMaxXDiff = Math::Abs(other->GetMax().x - GetMin().x);
	// top
	if ( (absMinYDiff <= absMaxYDiff) && (absMinYDiff <= absMinXDiff) && (absMinYDiff <= absMaxXDiff)) {
		offset = Vector2(0, other->GetMin().y - GetMin().y);
		return CollSide::Top;
	}
	if ((absMaxYDiff <= absMinYDiff) && (absMaxYDiff <= absMinXDiff) && (absMaxYDiff <= absMaxXDiff)) {
		offset = Vector2(0, GetMax().y - other->GetMax().y);
		return CollSide::Bottom;
	}
	if ((absMinXDiff <= absMinYDiff) && (absMinXDiff <= absMaxYDiff) && (absMinXDiff <= absMaxXDiff)) {
		offset = Vector2(other->GetMin().x - GetMin().x , 0);
		return CollSide::Left;
	}
	if ((absMaxXDiff <= absMinYDiff) && (absMaxXDiff <= absMaxYDiff) && (absMaxXDiff <= minXDiff)) {
		offset = Vector2(GetMax().x - other->GetMax().x, 0);
		return CollSide::Right;
	}
	return CollSide::None;
}
