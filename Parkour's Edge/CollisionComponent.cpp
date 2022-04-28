#include "CollisionComponent.h"
#include "SDL2/SDL_image.h"
#include "Actor.h"

CollisionComponent::CollisionComponent(class Actor* owner)
:Component(owner)
,mWidth(0.0f)
,mHeight(0.0f)
,mDepth(0.0f)
{
	
}

CollisionComponent::~CollisionComponent()
{
	
}

bool CollisionComponent::Intersect(const CollisionComponent* other)
{
	Vector3 aMin = GetMin();
	Vector3 aMax = GetMax();
	Vector3 bMin = other->GetMin();
	Vector3 bMax = other->GetMax();

	bool no = aMax.x < bMin.x ||
		aMax.y < bMin.y ||
		aMax.z < bMin.z ||
		bMax.x < aMin.x ||
		bMax.y < aMin.y ||
		bMax.z < aMin.z;

	return !no;
}

Vector3 CollisionComponent::GetMin() const
{
	Vector3 v = mOwner->GetPosition();
	v.x -= mDepth * mOwner->GetScale() / 2.0f;
	v.y -= mWidth * mOwner->GetScale() / 2.0f;
	v.z -= mHeight * mOwner->GetScale() / 2.0f;
	return v;
}

Vector3 CollisionComponent::GetMax() const
{
	Vector3 v = mOwner->GetPosition();
	v.x += mDepth * mOwner->GetScale() / 2.0f;
	v.y += mWidth * mOwner->GetScale() / 2.0f;
	v.z += mHeight * mOwner->GetScale() / 2.0f;
	return v;
}

const Vector3& CollisionComponent::GetCenter() const
{
	return mOwner->GetPosition();
}

CollSide CollisionComponent::GetMinOverlap(
	const CollisionComponent* other, Vector3& offset)
{
	offset = Vector3::Zero;

	if (Intersect(other))
	{
		// Get player min/max and block min/max
		Vector3 myMin = GetMin();
		Vector3 myMax = GetMax();
		Vector3 otherMin = other->GetMin();
		Vector3 otherMax = other->GetMax();

		// Figure out which side we are closest to
		float dx1 = otherMin.x - myMax.x;
		float dx2 = otherMax.x - myMin.x;
		float dy1 = otherMin.y - myMax.y;
		float dy2 = otherMax.y - myMin.y;
		float dz1 = otherMin.z - myMax.z;
		float dz2 = otherMax.z - myMin.z;

		float dx = 0.0f;
		if (Math::Abs(dx1) < Math::Abs(dx2))
		{
			dx = dx1;
		}
		else
		{
			dx = dx2;
		}

		float dy = 0.0f;
		if (Math::Abs(dy1) < Math::Abs(dy2))
		{
			dy = dy1;
		}
		else
		{
			dy = dy2;
		}

		float dz = 0.0f;
		if (Math::Abs(dz1) < Math::Abs(dz2))
		{
			dz = dz1;
		}
		else
		{
			dz = dz2;
		}

		if (Math::Abs(dz) < Math::Abs(dy) && Math::Abs(dz) < Math::Abs(dx))
		{
			offset.z = dz;
			if (dz == dz1)
			{
				//SDL_Log("bottom");
				return CollSide::Bottom;
			}
			else
			{
				//SDL_Log("top");
				return CollSide::Top;
			}
		}
		else if(Math::Abs(dy) < Math::Abs(dz) && Math::Abs(dy) < Math::Abs(dx))
		{
			offset.y = dy;
			if (dy == dy1)
			{
				//SDL_Log("min y");
				return CollSide::SideMinY;
			}
			else
			{
				//SDL_Log("max y");
				return CollSide::SideMaxY;
			}

		}
		else {
			offset.x = dx;
			if (dx == dx1) {
				//SDL_Log("min x");
				return CollSide::SideMinX;
			}
			else {
				//SDL_Log("max x");
				return CollSide::SideMaxX;
			}
		}

	}

	return CollSide::None;
}
