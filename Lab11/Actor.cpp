#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>

Actor::Actor(Game* game, Actor* parent)
	:mGame(game)
	,mState(ActorState::Active)
	,mPosition(Vector3::Zero)
	,mScale(1.0f)
	,mRotation(0.0f)
	,mParent(parent)
{
	if (mParent == nullptr) {
		mGame->AddActor(this);
	}
	else {
		mParent->AddChild(this);
	}
	
}

Actor::~Actor()
{
	while (!mChildren.empty())
	{
		delete mChildren.back();
	}

	for (auto c : mComponents) {
		delete c;
	}
	mComponents.clear();

	if (mParent == nullptr) {
		mGame->RemoveActor(this);
	}
	else {
		mParent->RemoveChild(this);
	}
	
}

void Actor::Update(float deltaTime)
{
	CalcWorldTransform();
	if (mState == ActorState::Active) {
		UpdateComp(deltaTime);
		OnUpdate(deltaTime);
	}
	CalcWorldTransform();
	for (auto child : mChildren) {
		child->Update(deltaTime);
	}
}

void Actor::CalcWorldTransform() {
	mWorldTransform = Matrix4::CreateScale(mScale) * Matrix4::CreateRotationZ(mRotation) * Matrix4::CreateFromQuaternion(mQuat) * Matrix4::CreateTranslation(mPosition);
	if (mParent) {
		if (mInheritScale) {
			mWorldTransform *= mParent->GetWorldTransform();
		}
		else {
			mWorldTransform *= mParent->GetWorldRotTrans();
		}
	}

}

Matrix4 Actor::GetWorldRotTrans() {
	Matrix4 temp = Matrix4::CreateRotationZ(mRotation) * Matrix4::CreateFromQuaternion(mQuat) * Matrix4::CreateTranslation(mPosition);
	if (mParent != nullptr) {
		temp *= mParent->GetWorldTransform();
	}
	return temp;
}

Vector3 Actor::GetWorldPosition() {
	return mWorldTransform.GetTranslation();
}

Vector3 Actor::GetWorldForward() {
	return mWorldTransform.GetXAxis();
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

Vector3 Actor::GetForward() {
	return Vector3(Math::Cos(GetRotation()), Math::Sin(GetRotation()), 0.0f);
}

Vector3 Actor::GetRight() {
	return Vector3(Math::Cos(GetRotation() + Math::PiOver2), Math::Sin(GetRotation() + Math::PiOver2), 0.0f);
}

Vector3 Actor::GetQuatForward() {
	Vector3 transformed = Vector3::Transform(Vector3::UnitX, mQuat);
	return Vector3::Normalize(transformed);
}

void Actor::RemoveChild(Actor* actor) {
	auto iter = std::find(mChildren.begin(), mChildren.end(), actor);
	if (iter != mChildren.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		auto iter2 = mChildren.end() - 1;
		std::iter_swap(iter, iter2);
		mChildren.pop_back();
	}
}