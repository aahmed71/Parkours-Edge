#include "UIComponent.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

class HUD : public UIComponent
{
public:
	HUD(class Actor* owner);
	virtual ~HUD();
	void Update(float deltaTime) override;
	void Draw(Shader* shader);
	void UpdateTime();
	void AddCoin();
	void RenderCheckpoint(std::string text);
	void DrawBlips(Shader* shader);
private:
	class Font* mFont;
	class Texture* mTimerText;
	class Texture* mCoinText;
	class Texture* mCheckpointText;
	class Texture* mRadar;
	class Texture* mRadarArrow;
	class Texture* mBlip;

	Vector2 mTimerPos = Vector2(-420.0f, -325.0f);
	Vector2 mCoinPos = Vector2(-450.0f, -290.0f);
	Vector2 mCheckpointPos = Vector2::Zero;
	Vector2 mRadarPos = Vector2(400, -280.0f);

	float mTextTimer = 0.0f;
	float mRadarRange = 1500.0f;
	float mRadarRadius = 92.0f;
};