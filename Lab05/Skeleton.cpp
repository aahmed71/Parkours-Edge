#include "Skeleton.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "EnemyComponent.h"
#include "AnimatedSprite.h"
#include "Fireball.h"
#include "Player.h"
#include "Random.h"

Skeleton::Skeleton(class Game* game, std::string room)
	:Actor(game)
{
	mSprite = new AnimatedSprite(this);
	std::vector<SDL_Texture*> skeleton{
	GetGame()->GetTexture("Assets/Skeleton0.png"),
	GetGame()->GetTexture("Assets/Skeleton1.png"),
	};
	mSprite->AddAnimation("skeleton", skeleton);
	mSprite->SetAnimation("skeleton");
	mCollComp = new CollisionComponent(this);
	mCollComp->SetSize(25, 25);
	mEnemyComp = new EnemyComponent(this, room);
	timer = Random::GetFloatRange(1.0f, 3.0f);
}

void Skeleton::OnUpdate(float deltaTime) {
	timer += deltaTime;
	if (timer >= 4) {
		Fireball* temp = new Fireball(mGame);
		temp->SetPosition(GetPosition());
		timer = 0;
		Vector2 sp = mGame->GetPlayer()->GetPosition() - GetPosition();
		float angle = Math::Atan2(-sp.y, sp.x);
		temp->SetRotation(temp->GetRotation() + angle);
	}

}