#pragma once
#include "SDL2/SDL.h" 
#include <cmath>
#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include "Math.h"
#include "SDL2/SDL_mixer.h"

// external window size constans
extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;
extern const int GRID_SQUARE_SIZE;
// game class
class Game {
public:
	Game();
	//Initialze the game
	bool Initialize();
	// runs game loop
	void RunLoop();
	// shutdown
	void Shutdown();
	// adds given actor to the vector in Game
	void AddActor(class Actor* actor);
	// remove given actor from vector
	void RemoveActor(class Actor* actor);
	// adds sprite to vector teh sorts by draw order
	void AddSprite(class SpriteComponent* sprite);
	// removes sprite from vector
	void RemoveSprite(class SpriteComponent* sprite);
	// returns texture
	SDL_Texture* GetTexture(std::string fileName);
	// returns previous keyboard state
	std::unordered_map<SDL_Scancode, bool> getPrevState();
	void AddBlock(class Block*);
	void RemoveBlock(class Block*);
	const std::vector<class Block*>& GetBlocks();
	class Actor* GetPlayer();
	Vector2 GetCameraPos();
	void SetCameraPos(Vector2 newPos);
	void AddGoomba(class Goomba* goomba);
	void RemoveGoomba(class Goomba* goomba);
	const std::vector<class Goomba*>& GetGoombas();
	Mix_Chunk* GetSound(const std::string& filename);
	int getMusicChannel();
private:
	// member functions for game loop
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	// data function
	void LoadData();
	void UnloadData();
	
	//texture hash map
	std::unordered_map<std::string, SDL_Texture*> mTextures;
	// Actor
	std::vector<class Actor*> mActors;
	// sprite vector
	std::vector<class SpriteComponent*> mSprites;
	// window
	SDL_Window* mWindow;
	// renderer
	SDL_Renderer* mRenderer;
	// stores previous ticks
	Uint32 mPrevious;
	// checks is program is running
	bool mIsRunning;
	// hold previous keyboard state
	std::unordered_map<SDL_Scancode, bool> mPrevKeyMap;
	// block 
	std::vector<class Block*> mBlocks;
	class Player* mPlayer;
	Vector2 mCameraPos;
	// goombas
	std::vector<class Goomba*> mGoombas;
	// sounds
	std::unordered_map<std::string, Mix_Chunk*> mChunks;
	int mMusicChan;
};
