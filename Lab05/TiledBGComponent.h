#pragma once
#include "SpriteComponent.h"
#include "SDL2/SDL.h"
#include "CSVHelper.h"
#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>
#include <sstream>

class TiledBGComponent : public SpriteComponent
{
public:
	TiledBGComponent(class Actor* owner, int drawOrder = 50);
	void Draw(SDL_Renderer* renderer) override;
	void LoadTileCSV(const std::string& fileName, int tileWidth, int tileHeight);
protected:
	int mTileWidth;
	int mTileHeight;
	std::vector<std::vector<int>> csvFileInts;
};
