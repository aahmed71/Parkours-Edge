#include "Vehicle.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "Frog.h"
#include "WrappingMove.h"
#include "Game.h"

Vehicle::Vehicle(Game* game, char vehicleType, Vector2 dir)
	:Actor(game)
{
	mSprite = new SpriteComponent(this);
	std::string texture = "";
	// choose texture for type of car
	switch (vehicleType) {
		case 'A':
			texture = "Assets/CarA.png";
			break;
		case 'B':
			texture = "Assets/CarB.png";
			break;
		case 'C':
			texture = "Assets/CarC.png";
			break;
		case 'D':
			texture = "Assets/CarD.png";
			break;
		case 'T':
			texture = "Assets/Truck.png";
			break;
	}
	mSprite->SetTexture(mGame->GetTexture(texture));
	mGame->AddVehicle(this);
	mMoveComp = new WrappingMove(this, dir);
	mMoveComp->SetForwardSpeed(100.0f);
	mCollComp = new CollisionComponent(this);
	// truck size
	if (vehicleType == 'T') {
		mCollComp->SetSize(128, 48);
	}
	else {
		mCollComp->SetSize(64, 64);
	}
	
}

Vehicle::~Vehicle() {
	mGame->RemoveVehicle(this);
}

CollisionComponent* Vehicle::GetCollComp(){
	return mCollComp;
}

void Vehicle::OnUpdate(float deltaTime) {
	Vector2 a = mMoveComp->GetOwnerDirection();
	Vector2 b = (mGame->GetFrog()->GetPosition()) - GetPosition();
	b.Normalize();
	float dot = Vector2::Dot(a, b);
	float ang = Math::Acos(dot);
	if (ang <= (Math::Pi) / 6.0f) {
		mMoveComp->SetForwardSpeed(50.0f);
	}
	else {
		mMoveComp->SetForwardSpeed(100.0f);
	}
}

