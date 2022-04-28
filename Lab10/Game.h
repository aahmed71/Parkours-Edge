#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include <unordered_map>
#include <string>
#include <vector>
#include "Math.h"
#include <string>       
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
	

	
};
