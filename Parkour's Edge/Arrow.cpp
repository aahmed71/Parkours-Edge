#include "Arrow.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "Checkpoint.h"
#include "Player.h"

Arrow::Arrow(Game* game, Actor* parent)
	:Actor(game, parent)
{
	mMesh = new MeshComponent(this);
	mMesh->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Arrow.gpmesh"));
	SetScale(0.15f);
}

void Arrow::OnUpdate(float deltaTime) {
	Quaternion quat;
	if (!mGame->GetCheckpoints().empty()) {
		Vector3 toCheckpoint = Vector3::Normalize(mGame->GetCheckpoints().front()->GetPosition() - mGame->GetPlayer()->GetPosition());
		float toCheckpointDot = Vector3::Dot(Vector3(1, 0, 0), toCheckpoint);
		float ang = Math::Acos(toCheckpointDot);
		Vector3 axis = Vector3::Normalize(Vector3::Cross(Vector3(1, 0, 0), toCheckpoint));
		if (toCheckpointDot == 1.0f) {
			quat = Quaternion::Identity;
		}
		else if (toCheckpointDot == -1.0f) {
			quat = Quaternion(Vector3::UnitZ, Math::Pi);
		}
		else {
			quat = Quaternion(axis, ang);
		}
	}
	else {
		quat = Quaternion::Identity;
	}
	SetQuat(quat);
	SetPosition(mGame->GetRenderer()->Unproject(Vector3(0.0f, 250.0f, 0.1f)));
}
