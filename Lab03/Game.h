#pragma once
#include "SDL2/SDL.h" 
#include <cmath>
#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>

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
	// add Vehicle
	void AddVehicle(class Vehicle* vehicle);
	// remove Vehicle
	void RemoveVehicle(class Vehicle* vehicle);
	// returns Vehicle vector
	const std::vector<class Vehicle*>& GetVehicle();
	void AddLog(class Log* log);
	void RemoveLog(class Log* log);
	const std::vector<class Log*>& GetLog();
	// returns previous keyboard state
	std::unordered_map<SDL_Scancode, bool> getPrevState();
	// returns frog
	class Frog* GetFrog();
	// returns goal
	class Actor* GetGoal();
	class CollisionComponent* GetGoalCol();
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
	// vehicle vector
	std::vector<class Vehicle*> mVehicles;
	// log vector
	std::vector<class Log*> mLogs;
	// da frowg
	class Frog* mFrog;
	// goal
	class Actor* mGoal;
	class CollisionComponent* mGoalCol;
};
