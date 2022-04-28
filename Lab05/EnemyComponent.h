#pragma once
#include "Component.h"
#include <string>
#include <functional>

class EnemyComponent : public Component
{
	public:
		EnemyComponent(Actor* owner, std::string currRoom);
		virtual ~EnemyComponent();
		void SetOnDeath(std::function<void()> death);
		void SetOnTakeDamage(std::function<void()> damage);
		void TakeDamage(int amount);
	private:
		std::string mRoom;
		int mHP;
		int mDmgTaken;
		std::function<void()> mOnDeath;
		std::function<void()> mOnTakeDamage;
};