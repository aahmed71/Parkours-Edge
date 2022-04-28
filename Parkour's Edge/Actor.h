#pragma once
#include <vector>
#include <SDL2/SDL_stdinc.h>
#include "Math.h"

enum class ActorState
{
	Active,
	Paused,
	Destroy
};

class Actor
{
public:
	Actor(class Game* game, Actor* parent);
	virtual ~Actor();

	// Update function called from Game (not overridable)
	void Update(float deltaTime);
	// update components
	void UpdateComp(float deltaTime);
	// ProcessInput function called from Game (not overridable)
	void ProcessInput(const Uint8* keyState);
	// ProccessInput on all components
	void ProcessInputComp(const Uint8* keyState);
	void CalcWorldTransform();
	Matrix4 GetWorldRotTrans();
	Vector3 GetWorldPosition();
	Vector3 GetWorldForward();

	// Parenting
	void AddChild(Actor* actor) { mChildren.push_back(actor); }
	void RemoveChild(Actor* actor);

	// Getters/setters
	const Vector3& GetPosition() const { return mPosition; }
	void SetPosition(const Vector3& pos) { mPosition = pos; }
	float GetScale() const { return mScale; }
	void SetScale(float scale) { mScale = scale; }
	float GetRotation() const { return mRotation; }
	void SetRotation(float rotation) { mRotation = rotation; }
	const Matrix4& GetWorldTransform() const { return mWorldTransform; }
	Quaternion GetQuat() const { return mQuat; }
	void SetQuat(Quaternion quat) { mQuat = quat; }
	Vector3 GetQuatForward();
	ActorState GetState() const { return mState; }
	void SetState(ActorState state) { mState = state; }
	Actor* GetParent() { return mParent; }

	class Game* GetGame() { return mGame; }
	
	// returns forward direction vector of the actor
	Vector3 GetForward();
	Vector3 GetRight();
	// Adds component to Actor (this is automatically called
	// in the component constructor)
	void AddComponent(class Component* c);
	
	// Returns component of type T, or null if doesn't exist
	template <typename T>
	T* GetComponent() const
	{
		for (auto c : mComponents)
		{
			T* t = dynamic_cast<T*>(c);
			if (t != nullptr)
			{
				return t;
			}
		}
		
		return nullptr;
	}
protected:
	// Any actor-specific update code (overridable)
	virtual void OnUpdate(float deltaTime);
	// Any actor-specific update code (overridable)
	virtual void OnProcessInput(const Uint8* keyState);
	
	class Game* mGame;
	// Actor's state
	ActorState mState;

	// Transform
	Vector3 mPosition;
	float mScale;
	float mRotation;
	Matrix4 mWorldTransform;
	Quaternion mQuat;

	// Components
	std::vector<class Component*> mComponents;

	// parenting
	Actor* mParent;
	std::vector<Actor*> mChildren;
	bool mInheritScale = false;
};
