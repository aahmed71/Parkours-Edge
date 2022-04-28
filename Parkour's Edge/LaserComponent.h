#include "MeshComponent.h"
#include "SegmentCast.h"

class LaserComponent : public MeshComponent
{
public:
	LaserComponent(class Actor* owner);
	void Update(float deltaTime) override;
	void Draw(class Shader* shader) override;
private:
	Matrix4 LineToMatrix(LineSegment line);
	std::vector<LineSegment> mLineSegs;
};