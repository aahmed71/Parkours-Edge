#pragma once
#include "SDL2/SDL.h" 
#include <cmath>
#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>

// external window size constans
extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;
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
	void AddAsteroid(class Asteroid* asteroid);
	void RemoveAsteroid(class Asteroid* asteroid);
	const std::vector<class Asteroid*>& GetAsteroid();
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
	// asteroid vector
	std::vector<class Asteroid*> mAsteroids;
};
