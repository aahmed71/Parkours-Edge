#include "Actor.h"

class Player : public Actor 
{
public:
	Player(class Game* owner);
private:
	class CollisionComponent* mCollision;
	class PlayerMove* mMove;
	class CameraComponent* mCamera;
};