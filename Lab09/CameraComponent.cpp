#include "CameraComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"

CameraComponent::CameraComponent(Actor* owner)
	:Component(owner) {


}

void CameraComponent::Update(float deltaTime) {
	mPitchAngle += mPitchSpeed * deltaTime;
	if (mPitchAngle < (-Math::PiOver2 / 2.0f)) {
		mPitchAngle = -Math::PiOver2 / 2.0f;
	}
	if (mPitchAngle > (Math::PiOver2 / 2.0f)) {
		mPitchAngle = Math::PiOver2 / 2.0f;
	}
	Matrix4 pitch = Matrix4::CreateRotationY(mPitchAngle);
	Matrix4 yaw = Matrix4::CreateRotationZ(mOwner->GetRotation());
	Matrix4 combined = pitch * yaw;
	Vector3 forward = Vector3::Transform(Vector3(1, 0, 0), combined);
	Vector3 TargetPos = mOwner->GetPosition() + forward * 2;
	mOwner->GetGame()->GetRenderer()->SetViewMatrix(Matrix4::CreateLookAt(mOwner->GetPosition(), TargetPos, Vector3::UnitZ));
}

