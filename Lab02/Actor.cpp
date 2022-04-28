#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>

Actor::Actor(Game* game)
	:mGame(game)
	,mState(ActorState::Active)
	,mPosition(Vector2::Zero)
	,mScale(1.0f)
	,mRotation(0.0f)
{
	mGame->AddActor(this);
}

Actor::~Actor()
{
	mGame->RemoveActor(this);
}

void Actor::Update(float deltaTime)
{
	// TODO
	if (mState == ActorState::Active) {
		UpdateComp(deltaTime);
		OnUpdate(deltaTime);
	}
}

void Actor::UpdateComp(float deltaTime) {
	for (Component* comp : mComponents) {
		comp->Update(deltaTime);
	}
}

void Actor::OnUpdate(float deltaTime)
{
}

void Actor::ProcessInput(const Uint8* keyState)
{
	if (mState == ActorState::Active) {
		ProcessInputComp(keyState);
		OnProcessInput(keyState);
	}

}

void Actor::ProcessInputComp(const Uint8* keyState) {
	for (Component* comp : mComponents) {
		comp->ProcessInput(keyState);
	}
}

void Actor::OnProcessInput(const Uint8* keyState)
{
}

void Actor::AddComponent(Component* c)
{
	mComponents.emplace_back(c);
	std::sort(mComponents.begin(), mComponents.end(), [](Component* a, Component* b) {
		return a->GetUpdateOrder() < b->GetUpdateOrder();
	});
}

Vector2 Actor::GetForward() {
	return Vector2(Math::Cos(GetRotation()), -1 * Math::Sin(GetRotation()));
}