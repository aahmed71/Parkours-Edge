#include "LaserComponent.h"
#include "Actor.h"
#include "Shader.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
#include "PlayerMove.h"
#include "Player.h"
#include "Mesh.h"
#include "Block.h"

LaserComponent::LaserComponent(Actor* owner)
	:MeshComponent(owner, true)
{
	SetMesh(owner->GetGame()->GetRenderer()->GetMesh("Assets/Laser.gpmesh"));
}

void LaserComponent::Update(float deltaTime) {
	
	mLineSegs.clear();
	bool mirror = false;
	LineSegment lineSeg;
	lineSeg.mStart = mOwner->GetWorldPosition();
	lineSeg.mEnd = mOwner->GetWorldPosition() + mOwner->GetWorldForward() * 500.0f;
	CastInfo info;
	Actor* skip = mOwner->GetParent();
	do {
		mirror = false;
		Player* player = mOwner->GetGame()->GetPlayer();
		if (SegmentCast((Actor*)player, lineSeg, info)) {
			Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Laser.wav"), 0);
			player->GetComponent<PlayerMove>()->Respawn();
			lineSeg.mEnd = info.mPoint;
			mLineSegs.push_back(lineSeg);
		
		}
		if (SegmentCast(mOwner->GetGame()->GetBlocks(), lineSeg, info, skip)) {
			lineSeg.mEnd = info.mPoint;
			mLineSegs.push_back(lineSeg);
			Block* block = (Block*)info.mActor;
			if (block->GetMirror()) {
				mirror = true;
				Vector3 laserDir = Vector3::Normalize(lineSeg.mEnd - lineSeg.mStart);
				lineSeg.mStart = info.mPoint;
				lineSeg.mEnd = lineSeg.mStart + Vector3::Reflect(laserDir * 500.0f, info.mNormal);
				skip = block;
			}
		}
	} while (mirror);
	mLineSegs.push_back(lineSeg);
}

void LaserComponent::Draw(Shader* shader) {
	if (mMesh)
	{
		// Set the world transform
		for (auto lineSeg : mLineSegs) {
			shader->SetMatrixUniform("uWorldTransform", LineToMatrix(lineSeg));
			// Set the active texture
			Texture* t = mMesh->GetTexture(mTextureIndex);
			if (t)
			{
				t->SetActive();
			}
			// Set the mesh's vertex array as active
			VertexArray* va = mMesh->GetVertexArray();
			va->SetActive();
			// Draw
			glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
		}

	}
}

Matrix4 LaserComponent::LineToMatrix(LineSegment line) {
	Matrix4 scale = Matrix4::CreateScale(line.Length(), 1.0f, 1.0f);
	Vector3 oldVec = Vector3(1, 0, 0);
	Vector3 newVec = Vector3::Normalize(line.mEnd - line.mStart);
	float oldNewDot = Vector3::Dot(oldVec, newVec);
	float angle = Math::Acos(oldNewDot);
	Vector3 axis = Vector3::Normalize(Vector3::Cross(oldVec, newVec));
	Quaternion quat;
	if (oldNewDot == 1.0f) {
		quat = Quaternion::Identity;
	}
	else if (oldNewDot == -1.0f) {
		quat = Quaternion(Vector3::UnitZ, Math::Pi);
	}
	else {
		quat = Quaternion(axis, angle);
	}
	Matrix4 rotation = Matrix4::CreateFromQuaternion(quat);
	Matrix4 translation = Matrix4::CreateTranslation(line.PointOnSegment(0.5f));
	return scale * rotation * translation;
;}