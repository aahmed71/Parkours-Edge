//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"
// Constants
const int WALL_THICKNESS = 15;
const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768;
const int PADDLE_HEIGHT = 100;
// space between paddle and left side
const int PADDLE_BUFFER = 15;

Game::Game() {
	// game is running
	mIsRunning = true;
}

bool Game::Initialize() {
	// initialize SDL
	int sdlResult = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	if (sdlResult != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	//create window
	mWindow = SDL_CreateWindow("PONG!", 300, 100, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	if (!mWindow) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	// initialize SDL renderer
	mRenderer = SDL_CreateRenderer(mWindow,-1,		 
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!mRenderer){
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	// initial paddle and ball position
	mPadPosition.x = PADDLE_BUFFER;
	mPadPosition.y = WINDOW_HEIGHT / 2;
	mBallPosition.x = WINDOW_WIDTH / 2;
	mBallPosition.y = WINDOW_HEIGHT / 2;
	mBallVelocity.x = -300;
	mBallVelocity.y = -300;
	// init previous tick 
	mPrevious = 0;

	return true;
}

void Game::RunLoop() {
	// keep looping program until it stops running
	while (mIsRunning) {
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::Shutdown() {
	// turns game off
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Game::ProcessInput() {
	// poll for events
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		// if user exits, quit
		if (event.type == SDL_QUIT) {
			mIsRunning = false;
			break;
		}
	}

	// get state of kb
	const Uint8* keyState = SDL_GetKeyboardState(NULL);
	// escape key ends loop
	if (keyState[SDL_SCANCODE_ESCAPE]){
		mIsRunning = false;
	}
	// update paddle direction from user controls
	mPadDirection = 0;
	// up arrow
	if (keyState[SDL_SCANCODE_UP]) {
		mPadDirection = -1;
	}
	// down arrow
	if (keyState[SDL_SCANCODE_DOWN]) {
		mPadDirection = 1;
	}
}

void Game::UpdateGame() {
	// frame limiting 16 ms
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mPrevious + 16));
	// calculating delta time
	float deltaTime = (SDL_GetTicks() - mPrevious) / 1000.0f;
	// deltaTime cap
	if (deltaTime > 0.033f) {
		deltaTime = 0.033f;
	}
	// print deltaTime
	SDL_Log("%f", deltaTime);
	// save ticks for next update
	mPrevious = SDL_GetTicks();

	// move paddle
	if (mPadDirection != 0){
		mPadPosition.y += static_cast<int> (deltaTime * mPadDirection * 600.0f);
		// set paddle bounds
		if (mPadPosition.y < (WALL_THICKNESS + PADDLE_HEIGHT / 2)){
			mPadPosition.y = WALL_THICKNESS + PADDLE_HEIGHT / 2;
		}
		else if (mPadPosition.y > (WINDOW_HEIGHT - WALL_THICKNESS - PADDLE_HEIGHT / 2)){
			mPadPosition.y = WINDOW_HEIGHT - WALL_THICKNESS - PADDLE_HEIGHT / 2;
		}
	}
	
	// move ball
	mBallPosition.x += static_cast<int> (deltaTime * mBallVelocity.x);
	mBallPosition.y += static_cast<int> (deltaTime * mBallVelocity.y);

	// calculate bounces
	// top wall
	if (mBallPosition.y <= WALL_THICKNESS * 3 / 2 && mBallVelocity.y < 0.0f) {
		mBallVelocity.y *= -1;
	}
	// bottom
	if (mBallPosition.y >= WINDOW_HEIGHT - WALL_THICKNESS * 3 / 2 && mBallVelocity.y > 0.0f) {
		mBallVelocity.y *= -1;
	}
	// right wall
	if (mBallPosition.x > WINDOW_WIDTH - WALL_THICKNESS * 3 / 2 && mBallVelocity.x > 0.0f) {
		mBallVelocity.x *= -1;
	}
	// check paddle collision
	int ballPaddYDiff = std::abs(mBallPosition.y - mPadPosition.y);
	if (mBallVelocity.x < 0.0f && mBallPosition.x <= PADDLE_BUFFER + WALL_THICKNESS 
		&& mBallPosition.x >= PADDLE_BUFFER && ballPaddYDiff <= PADDLE_HEIGHT / 2.0f) {
		mBallVelocity.x *=  -1;
	}

	// ball went off left side, you lost
	if (mBallPosition.x <= 0.0f) {
		mIsRunning = false;
	}
}

void Game::GenerateOutput() {
	// set color
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 255, 255);

	//clear back buffer
	SDL_RenderClear(mRenderer);

	//create walls
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	// top wall
	SDL_Rect horizontalWall{0, 0, WINDOW_WIDTH, WALL_THICKNESS};
	SDL_RenderFillRect(mRenderer, &horizontalWall);
	// bottom wall
	horizontalWall.y = WINDOW_HEIGHT - WALL_THICKNESS;
	SDL_RenderFillRect(mRenderer, &horizontalWall);
	// side wall
	SDL_Rect verticalWall{WINDOW_WIDTH - WALL_THICKNESS, 0, WALL_THICKNESS, WINDOW_HEIGHT };
	SDL_RenderFillRect(mRenderer, &verticalWall);

	// paddle
	SDL_Rect paddle{mPadPosition.x - WALL_THICKNESS / 2, mPadPosition.y - PADDLE_HEIGHT / 2, WALL_THICKNESS, PADDLE_HEIGHT};
	SDL_RenderFillRect(mRenderer, &paddle);

	// ball
	SDL_Rect ball{mBallPosition.x - WALL_THICKNESS / 2, mBallPosition.y - WALL_THICKNESS / 2, WALL_THICKNESS, WALL_THICKNESS};
	SDL_RenderFillRect(mRenderer, &ball);
	SDL_RenderPresent(mRenderer);
}