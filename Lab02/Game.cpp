//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"
#include "Actor.h"
#include "Ship.h"
#include "Asteroid.h"
#include "SpriteComponent.h"
#include "Random.h"
#include <SDL2/SDL_image.h>

// settings constans
const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768;


Game::Game() {
	// game is running
	mIsRunning = true;
}

bool Game::Initialize() {
	// init random
	Random::Init();

	// initialize SDL
	int sdlResult = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	if (sdlResult != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	//create window
	mWindow = SDL_CreateWindow("PONG!", 100, 50, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	if (!mWindow) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	// initialize SDL renderer
	mRenderer = SDL_CreateRenderer(mWindow, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!mRenderer) {
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}
	
	// initialize SDL image
	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG){
		SDL_Log("Unable to initialize SDL_image: %s", SDL_GetError());
		return false;
	}

	// call load data
	LoadData();

	// init previous tick 
	mPrevious = SDL_GetTicks();

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
	UnloadData();
	IMG_Quit();
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Game::ProcessInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		// if user exits, quit
		if (event.type == SDL_QUIT) {
			mIsRunning = false;
			break;
		}
	}

	// get state of kb
	const Uint8* state = SDL_GetKeyboardState(NULL);
	// escape key ends loop
	if (state[SDL_SCANCODE_ESCAPE]) {
		mIsRunning = false;
	}

	// process input on all actors
	std::vector <class Actor*> temp = mActors;
	// iterate through vector and update
	for (class Actor* act : temp) {
		act->ProcessInput(state);
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
	// save ticks for next update
	mPrevious = SDL_GetTicks();

	// updating actors
	// make copy of actor vector
	std::vector <class Actor*> temp = mActors;
	// iterate through vector and update
	for (class Actor* act : temp) {
		act->Update(deltaTime);
	}
	// vector for destroying
	std::vector <class Actor*> toDestroy;
	for (class Actor* act : temp) {
		if (act->GetState() == ActorState::Destroy) {
			toDestroy.push_back(act);
		}
	}
	// destroy
	for (auto act : toDestroy){
		delete act;
	}

}

void Game::GenerateOutput() {
	// set color
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 0);
	//clear back buffer
	SDL_RenderClear(mRenderer);

	// draw sprites
	for (SpriteComponent* sprite : mSprites){
		if (sprite->IsVisible()) {
			sprite->Draw(mRenderer);
		}
	}
	// render
	SDL_RenderPresent(mRenderer);
}

void Game::AddActor(class Actor* actor) {
	mActors.push_back(actor);
}

void Game::RemoveActor(class Actor* actor) {
	// find actor
	std::vector<class Actor*>::iterator it;
	it = std::find(mActors.begin(), mActors.end(), actor);
	// erase
	mActors.erase(it);
}

void Game::LoadData() {
	// background
	Actor* background = new Actor(this);
	SpriteComponent* sc = new SpriteComponent(background, 50);
	sc->SetTexture(GetTexture("Assets/Stars.png"));
	background->SetPosition(Vector2(512, 384));

	// the ship
	Ship* test = new Ship(this);
	test->SetPosition(Vector2(512, 384));

	// 10 asteroids
	for (int i = 0; i < 10; i++) {
		Asteroid* ast = new Asteroid(this);
	}
	

}

void Game::UnloadData() {
	// keep deleting until vector empty
	while (!mActors.empty()) {
		// delete actor
		delete mActors.back();
	}

	// delete textures
	for (auto tex : mTextures) {
		SDL_DestroyTexture(tex.second);
	}
	mTextures.clear();
}

SDL_Texture* Game::GetTexture(std::string fileName) {
	if (mTextures.find(fileName) != mTextures.end()) {
		return mTextures[fileName];
	}
	SDL_Surface* image = IMG_Load(fileName.c_str());
	// check for failure
	if(!image){
		SDL_Log("Failed to load texture file %s", fileName.c_str());
		return nullptr;
	}
	// convert surface to texture
	SDL_Texture* texture = SDL_CreateTextureFromSurface(mRenderer, image);
	SDL_FreeSurface(image);
	mTextures.insert(std::pair< std::string, SDL_Texture*>(fileName, texture));
	return texture;
}

void Game::AddSprite(SpriteComponent* sprite){
	mSprites.push_back(sprite);
	std::sort(mSprites.begin(), mSprites.end(),
		[](SpriteComponent* a, SpriteComponent* b) {
			return a->GetDrawOrder() < b->GetDrawOrder();
		});

}

void Game::RemoveSprite(SpriteComponent* sprite){
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}

void Game::AddAsteroid(class Asteroid* asteroid) {
	mAsteroids.push_back(asteroid);
}

void Game::RemoveAsteroid(class Asteroid* asteroid) {
	// find asteroid
	std::vector<class Asteroid*>::iterator it;
	it = std::find(mAsteroids.begin(), mAsteroids.end(), asteroid);
	// erase
	mAsteroids.erase(it);
}

const std::vector<class Asteroid*>& Game::GetAsteroid() {
	return mAsteroids;
}