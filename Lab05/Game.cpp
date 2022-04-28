//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"
#include "Actor.h"
#include "Player.h"
#include "SpriteComponent.h"
#include "Random.h"
#include "Spawner.h"
#include "TiledBGComponent.h"
#include "SecretBlock.h"
#include "Collider.h"
#include <SDL2/SDL_image.h>
#include "Door.h"
#include <sstream>

// settings constans
const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 448;
const int GRID_SQUARE_SIZE = 32;

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
	mWindow = SDL_CreateWindow("Mario", 100, 50, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
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
	//SDL_RenderSetLogicalSize(mRenderer, WINDOW_WIDTH, WINDOW_HEIGHT);

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

	mCameraPos = Vector2(0, 0);
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

	// space
	if (state[SDL_SCANCODE_SPACE]) {
		mPrevKeyMap[SDL_SCANCODE_SPACE] = true;
	}
	else {
		mPrevKeyMap[SDL_SCANCODE_SPACE] = false;
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
	Actor* test1 = new Actor(this);
	TiledBGComponent* test1Tile = new TiledBGComponent(test1);
	test1Tile->LoadTileCSV("Assets/Dungeon/DungeonMapBG.csv", 32, 32);
	test1Tile->SetTexture(GetTexture("Assets/Dungeon/DungeonTiles.png"));

	// load room 1
	for (int i = 1; i < 8; i++) {
		std::string roomFile = "Assets/Dungeon/Room";
		std::string room = "Room" + std::to_string(i);
		std::ifstream in(roomFile + std::to_string(i) + ".csv");
		std::string line;
		std::getline(in, line);
		while (std::getline(in, line)) {
			std::vector<std::string> stringArr = CSVHelper::Split(line);

			// load in player
			if (stringArr[0] == "Player") {
				mPlayer = new Player(this);
				mPlayer->SetPosition(Vector2(std::stoi(stringArr[1]), std::stoi(stringArr[2])));
			}

			else if (stringArr[0] == "Collider") {	
				int width = std::stoi(stringArr[3]);
				int height = std::stoi(stringArr[4]);
				int x = std::stoi(stringArr[1]) + width / 2;
				int y = std::stoi(stringArr[2]) + height / 2;
				Collider* temp = new Collider(this, width, height);
				temp->SetPosition(Vector2(x, y));
				mColliders.push_back(temp);
			}
			else if (stringArr[0] == "Door") {
				int width = std::stoi(stringArr[3]);
				int height = std::stoi(stringArr[4]);
				int x = std::stoi(stringArr[1]) + width / 2;
				int y = std::stoi(stringArr[2]) + height / 2;
				Door* temp = new Door(this, stringArr[5], stringArr[6], stringArr[7]);
				if (mDoorMap.count(room)) {
					mDoorMap[room].push_back(temp);
				}
				else {
					std::vector<Door*> tempVec;
					tempVec.push_back(temp);
					mDoorMap.insert(std::pair(room, tempVec));
				}
				temp->SetPosition(Vector2(x, y));
			}
			else if (stringArr[0] == "SecretBlock") {
				int width = std::stoi(stringArr[3]);
				int height = std::stoi(stringArr[4]);
				int x = std::stoi(stringArr[1]) + width / 2;
				int y = std::stoi(stringArr[2]) + height / 2;
				SecretBlock* temp = new SecretBlock(this);		
				temp->SetPosition(Vector2(x, y));
				mSecretBlocks.insert(std::pair(room, temp));
			}
			else {
				int width = std::stoi(stringArr[3]);
				int height = std::stoi(stringArr[4]);
				int x = std::stoi(stringArr[1]) + width / 2;
				int y = std::stoi(stringArr[2]) + height / 2;
				Spawner* temp = new Spawner(this, stringArr[0]);
				if (mSpawnerMap.count(room)) {
					mSpawnerMap[room].push_back(temp);
				}
				else {
					std::vector<Spawner*> tempVec;
					tempVec.push_back(temp);
					mSpawnerMap.insert(std::pair(room, tempVec));
				}
				temp->SetPosition(Vector2(x, y));
			}
		}
		mCurrRoom = "Room1";
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


class Player* Game::GetPlayer() {
	return mPlayer;
}

Vector2 Game::GetCameraPos() {
	return mCameraPos;
}

void Game::SetCameraPos(Vector2 newPos) {
		mCameraPos = newPos;
}

const std::vector<class Collider*>& Game::GetColliders() {
	return mColliders;
}

const std::unordered_map<std::string, std::vector<class Door*>>& Game::GetDoorMap() {
	return mDoorMap;
}

std::string Game::GetCurrentRoom() {
	return mCurrRoom;
}

void Game::SetCurrentRoom(std::string room) {
	mCurrRoom = room;
}

class SecretBlock* Game::GetSecretBlock(std::string room) {
	if (mDoorMap.count(room)) {
		return mSecretBlocks[room];
	}
	return nullptr;
}

const std::unordered_map<std::string, std::vector<class Spawner*>>& Game::GetSpawnerMap() {
	return mSpawnerMap;
}

const std::unordered_map<std::string, std::vector<class Actor*>>& Game::GetEnemyMap() {
	return mEnemyMap;
}

void Game::AddEnemy(Actor* enemy, std::string room) {
	if (mEnemyMap.count(room)) {
		mEnemyMap[room].push_back(enemy);
	}
	else {
		std::vector<Actor*> tempVec;
		tempVec.push_back(enemy);
		mEnemyMap.insert(std::pair(room, tempVec));
	}
}
void Game::RemoveEnemy(Actor* enemy, std::string room) {
	if (mEnemyMap.count(room)) {
		std::vector<class Actor*>::iterator it;
		it = std::find(mEnemyMap[room].begin(), mEnemyMap[room].end(), enemy);
		// erase
		mEnemyMap[room].erase(it);
	}
}