#pragma once
#include "Component.h"
#include <functional>
class Collectible : public Component 
{
	public:
		Collectible(class Actor* owner, class CollisionComponent* collComp);
		void Update(float deltaTime) override;
		void SetOnCollect(std::function<void()> newColl);
	private:
		std::function<void()> mOnCollect;
		class CollisionComponent* mCollComp;

};