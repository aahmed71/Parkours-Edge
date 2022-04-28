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

	void AddBlock(class Block*);
	void RemoveBlock(class Block*);
	const std::vector<class Block*>& GetBlocks();
	void LoadBlocks(std::string filename, float x);

	Mix_Chunk* GetSound(const std::string& fileName);
	int GetMusicChannel();

	class Renderer* GetRenderer() {	return mRenderer; }

	class Player* GetPlayer();
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
	std::vector<class Block*> mBlocks;

	class Renderer* mRenderer = nullptr;

	Uint32 mTicksCount = 0;
	bool mIsRunning;

	int mMusicChan;
};
