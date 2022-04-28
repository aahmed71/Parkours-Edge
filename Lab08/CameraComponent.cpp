#include "CameraComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"

CameraComponent::CameraComponent(Actor* owner)
	:Component(owner) {
	mDampConst = 2.0f * Math::Sqrt(mSpringConst);
	SnapToIdeal();

}

void CameraComponent::Update(float deltaTime) {
	Vector3 forward = mOwner->GetForward();
	Vector3 up = Vector3::UnitZ;
	float HDist = 60.0f;
	float TargetDist = 50.0f;
	Vector3 IdealPos = mOwner->GetPosition() - (forward * HDist);
	IdealPos.z = 70.0f;
	Vector3 displacement = mCameraPos - IdealPos;
	Vector3 springAccel = (-mSpringConst * displacement) - (mDampConst * mCameraVel);
	mCameraVel += springAccel * deltaTime;
	mCameraPos += mCameraVel * deltaTime;
	Vector3 TargetPos = mOwner->GetPosition() + (forward * TargetDist);
	mOwner->GetGame()->GetRenderer()->SetViewMatrix(Matrix4::CreateLookAt(mCameraPos, TargetPos, Vector3::UnitZ));
}

void CameraComponent::SnapToIdeal() {
	Vector3 forward = mOwner->GetForward();
	Vector3 up = Vector3::UnitZ;
	float HDist = 60.0f;
	float TargetDist = 50.0f;
	Vector3 IdealPos = mOwner->GetPosition() - (forward * HDist);
	IdealPos.z = 70.0f;
	Vector3 TargetPos = mOwner->GetPosition() + (forward * TargetDist);
	mCameraPos = IdealPos;
	mOwner->GetGame()->GetRenderer()->SetViewMatrix(Matrix4::CreateLookAt(IdealPos, TargetPos, Vector3::UnitZ));
}