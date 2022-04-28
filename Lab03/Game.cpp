//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"
#include "Actor.h"
#include "Frog.h"
#include "Log.h"
#include "Vehicle.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "Random.h"
#include <SDL2/SDL_image.h>

// settings constans
const int WINDOW_WIDTH = 896;
const int WINDOW_HEIGHT = 1024;
const int GRID_SQUARE_SIZE = 64;

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
	mWindow = SDL_CreateWindow("Frogger", 100, 50, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 0);
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
	SDL_RenderSetLogicalSize(mRenderer, WINDOW_WIDTH, WINDOW_HEIGHT);

	// initialize SDL image
	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG){
		SDL_Log("Unable to initialize SDL_image: %s", SDL_GetError());
		return false;
	}

	// call load data
	LoadData();

	// init previous tick 
	mPrevious = SDL_GetTicks();

	// set scancode to false
	mPrevKeyMap.insert(std::make_pair(SDL_SCANCODE_UP, false));
	mPrevKeyMap.insert(std::make_pair(SDL_SCANCODE_DOWN, false));
	mPrevKeyMap.insert(std::make_pair(SDL_SCANCODE_LEFT, false));
	mPrevKeyMap.insert(std::make_pair(SDL_SCANCODE_RIGHT, false));

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

	// set prev keystate map
	// up
	if (state[SDL_SCANCODE_UP]) {
		mPrevKeyMap[SDL_SCANCODE_UP] = true;
	}
	else {
		mPrevKeyMap[SDL_SCANCODE_UP] = false;
	}
	// down
	if (state[SDL_SCANCODE_DOWN]) {
		mPrevKeyMap[SDL_SCANCODE_DOWN] = true;
	}
	else {
		mPrevKeyMap[SDL_SCANCODE_DOWN] = false;
	}
	// left
	if (state[SDL_SCANCODE_LEFT]) {
		mPrevKeyMap[SDL_SCANCODE_LEFT] = true;
	}
	else {
		mPrevKeyMap[SDL_SCANCODE_LEFT] = false;
	}
	// right
	if (state[SDL_SCANCODE_RIGHT]) {
		mPrevKeyMap[SDL_SCANCODE_RIGHT] = true;
	}
	else {
		mPrevKeyMap[SDL_SCANCODE_RIGHT] = false;
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
	sc->SetTexture(GetTexture("Assets/Background.png"));
	background->SetPosition(Vector2(448, 512));

	// level file
	char ch;
	std::ifstream in("Assets/level.txt");
	std::vector<std::vector<Actor*>> levelActors;
	// fill 2d vector with all actors in level
	for (int i = 0; i < 13; i++) {
		std::vector<Actor*> currLine;
		Vector2 dir(0, 0);
		// even row
		if (i % 2 == 0) {
			dir.x = 1;
		}
		else {
			dir.x = -1;
		}
		for (int j = 0; j < 13; j++) {
			in.get(ch);
			if (ch == '\n') {
				in.get(ch);
			}
			Vector2 currPosition(static_cast<float>(j * GRID_SQUARE_SIZE + 64), static_cast<float>(i * GRID_SQUARE_SIZE + 160));
			// log
			if ('X' <= ch && ch <= 'Z') {
				currLine.push_back(new Log(this, ch, dir));
				currLine.back()->SetPosition(currPosition);
			}
			// frog
			else if (ch == 'F') {
				mFrog = new Frog(this, currPosition);
				currLine.push_back(mFrog);
				currLine.back()->SetPosition(currPosition);
				
			}
			// goal
			else if (ch == 'G') {
				SDL_Log("goal created");
				mGoal = new Actor(this);
				currLine.push_back(mGoal);
				currLine.back()->SetPosition(currPosition);
				mGoalCol = new CollisionComponent(mGoal);
				mGoalCol->SetSize(64, 64);

			}
			// nothing
			else if (ch == '.') {
				continue;
			}
			// car
			else {
				currLine.push_back(new Vehicle(this, ch, dir));
				currLine.back()->SetPosition(currPosition);
			}


		}
		levelActors.push_back(currLine);
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

std::unordered_map<SDL_Scancode, bool> Game::getPrevState(){
	return mPrevKeyMap;
}

void Game::AddVehicle(class Vehicle* vehicle) {
	mVehicles.push_back(vehicle);
}

void Game::RemoveVehicle(class Vehicle* vehicle) {
	// find asteroid
	std::vector<class Vehicle*>::iterator it;
	it = std::find(mVehicles.begin(), mVehicles.end(), vehicle);
	// erase
	mVehicles.erase(it);
}

const std::vector<class Vehicle*>& Game::GetVehicle() {
	return mVehicles;
}

void Game::AddLog(Log* log)
{
	mLogs.push_back(log);
}

void Game::RemoveLog(Log* log)
{
	// find asteroid
	std::vector<class Log*>::iterator it;
	it = std::find(mLogs.begin(), mLogs.end(), log);
	// erase
	mLogs.erase(it);
}

const std::vector<class Log*>& Game::GetLog()
{
	return mLogs;
}

Frog* Game::GetFrog() {
	return mFrog;
}

Actor* Game::GetGoal() {
	return mGoal;
}

CollisionComponent* Game::GetGoalCol(){
	return mGoalCol;
}