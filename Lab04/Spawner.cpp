#include "Spawner.h"
#include "Math.h"
#include "Game.h"
#include "Goomba.h"


Spawner::Spawner(Game* game)
	:Actor(game)
{

}

void Spawner::OnUpdate(float deltaTime) {
	if ( (Math::Abs(GetPosition().x - mGame->GetPlayer()->GetPosition().x)) < 600) {
		Goomba* temp = new Goomba(mGame);
		temp->SetPosition(GetPosition());
		SetState(ActorState::Destroy);
	}

}
