#include "Actor.h"

class SecurityCone : public Actor
{
public:
	SecurityCone(class Game* game, class Actor* actor);
	void OnUpdate(float deltaTime) override;
private:
	class MeshComponent* mMesh;
	float mLightTimer = 0.0f;
	class SecurityCamera* mCamera;
	bool mDetected = false;
	int mDetectedSFX = -1;
};