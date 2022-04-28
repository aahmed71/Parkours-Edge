#include "TiledBGComponent.h"
#include "Actor.h"
#include "Game.h"

TiledBGComponent::TiledBGComponent(Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder)
{
}

void TiledBGComponent::Draw(SDL_Renderer* renderer) {
	int rowTiles = GetTexWidth() / mTileWidth;
	int colTiles = GetTexHeight() / mTileHeight;
	for (int row = 0; row < csvFileInts.size(); row++) {
		for (int col = 0; col < csvFileInts[row].size(); col++) {
			int csvInt = csvFileInts[row][col];
			if (mTexture && (csvInt != -1))
			{
				SDL_Rect r;
				r.w = mTileWidth;
				r.h = mTileHeight;
				// Center the rectangle around the position of the owner
				r.x = col * 32;
				r.y = row * 32;

				r.x -= mOwner->GetGame()->GetCameraPos().x;
				r.y -= mOwner->GetGame()->GetCameraPos().y;

				SDL_Rect srcRect;
				srcRect.w = mTileWidth;
				srcRect.h = mTileHeight;
				// Center the rectangle around the position of the owne				
				srcRect.y = (csvInt / rowTiles) * 32;
				srcRect.x = (csvInt % rowTiles) * 32;

				// Draw (have to convert angle from radians to degrees, and clockwise to counter)
				SDL_RenderCopyEx(renderer,
					mTexture,
					&srcRect,
					&r,
					0.0,
					nullptr,
					SDL_FLIP_NONE);
			}
		}
	}
	
}

void TiledBGComponent::LoadTileCSV(const std::string& fileName, int tileWidth, int tileHeight) {
	mTileWidth = tileWidth;
	mTileHeight = tileHeight;
	std::ifstream in(fileName);
	std::string line;
	while (std::getline(in, line)) {
		std::vector<std::string> stringArr = CSVHelper::Split(line);
		std::vector<int> intLine;
		for (auto str : stringArr) {
			intLine.push_back(std::stoi(str));
		}
		csvFileInts.push_back(intLine);
	}
}
