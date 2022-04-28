#pragma once
#include "SDL2/SDL.h" 
#include <cmath>
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
private:
	// member functions for game loop
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	// window
	SDL_Window* mWindow;
	// renderer
	SDL_Renderer* mRenderer;
	// paddle position
	SDL_Point mPadPosition;
	// paddle direction
	int mPadDirection;
	// ball position
	SDL_Point mBallPosition;
	// ball velocity
	SDL_Point mBallVelocity;
	// stores previous ticks
	Uint32 mPrevious;
	// checks is program is running
	bool mIsRunning;
};