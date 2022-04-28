#include "CameraComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Player.h"
#include "PlayerMove.h"
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

	PlayerMove* player = mOwner->GetGame()->GetPlayer()->GetComponent<PlayerMove>();
	if (player->GetState() != MoveState::WallRun) {
		mRollAngle -= mRollSpeed * deltaTime;
		if (mRollAngle > 0 && mRollSpeed < 0) {
			mRollAngle = 0;
		}
		if (mRollAngle < 0 && mRollSpeed > 0) {
			mRollAngle = 0;
		}
	}
	else {
		mRollAngle += mRollSpeed * deltaTime;
		if (mRollAngle < (-Math::PiOver2 / 2.0f)) {
			mRollAngle = -Math::PiOver2 / 2.0f;
		}
		if (mRollAngle > (Math::PiOver2 / 2.0f)) {
			mRollAngle = Math::PiOver2 / 2.0f;
		}
	}


	Matrix4 pitch = Matrix4::CreateRotationY(mPitchAngle);
	Matrix4 yaw = Matrix4::CreateRotationZ(mOwner->GetRotation());
	Matrix4 combined = pitch * yaw;
	Matrix4 roll;
	if (XorY) {
		roll = Matrix4::CreateRotationX(mRollAngle);
	}
	else {
		roll = Matrix4::CreateRotationY(mRollAngle);
	}
	Vector3 forward = Vector3::Transform(Vector3(1, 0, 0), combined);

	Vector3 wallRun = Vector3::Transform(Vector3(0, 0, 1), roll);
	Vector3 TargetPos = mOwner->GetPosition() + forward * 2;
	mOwner->GetGame()->GetRenderer()->SetViewMatrix(Matrix4::CreateLookAt(mOwner->GetPosition(), TargetPos, wallRun));
}

