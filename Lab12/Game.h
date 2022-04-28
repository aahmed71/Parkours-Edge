#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include <SDL2/SDL_ttf.h>
#include <unordered_map>
#include <string>
#include <vector>
#include "Math.h"
#include <string>    
#include <queue>
#include <iostream>     
#include <sstream>

class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);


	Mix_Chunk* GetSound(const std::string& fileName);
	int GetMusicChannel();

	class Renderer* GetRenderer() {	return mRenderer; }
	void SetPlayer(class Player*);
	class Player* GetPlayer();
	int mMusicChan;

	std::vector<class Actor*>& GetBlocks() { return mBlocks; }
	void AddBlock(class Actor* block) { mBlocks.push_back(block); }
	void PopBlock() { mBlocks.pop_back(); }

	std::vector<class SecurityCamera*>& GetCameras() { return mCameras; }
	void AddCamera(class SecurityCamera* camera) { mCameras.push_back(camera); }
	void RemoveCamera(class SecurityCamera* camera);

	std::queue<class Checkpoint*>& GetCheckpoints() { return mCheckpoints; }
	void AddCheckpoint(class Checkpoint* check) { mCheckpoints.push(check); }
	void PopCheckpoint() { mCheckpoints.pop(); }

	void SetNextLevel(std::string level) { mNextLevel = level; }
	void LoadNextLevel();

	float GetTimer() { return mTimer; }
	int GetCoins() { return mCoins; }
	void AddCoin() { mCoins++; }
	void SetText(std::string text) { mText = text; }
	std::string GetText() { return mText; }

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();



	std::unordered_map<std::string, Mix_Chunk*> mSounds;

	// All the actors in the game
	std::vector<class Actor*> mActors;

	class Player* mPlayer;
	class Renderer* mRenderer = nullptr;

	Uint32 mTicksCount = 0;
	bool mIsRunning;	

	std::vector<class Actor*> mBlocks;
	std::queue<class Checkpoint*> mCheckpoints;
	std::vector<class SecurityCamera*> mCameras;

	class Arrow* mArrow;

	std::string mNextLevel;

	float mTimer = 0.0f;
	int mCoins = 0;
	std::string mText = " ";
};
