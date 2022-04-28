//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"
#include "Actor.h"
#include "Block.h"
#include "Player.h"
#include "SpriteComponent.h"
#include "Random.h"
#include "Goomba.h"
#include "Spawner.h"
#include <SDL2/SDL_image.h>
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

	// load sound
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

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
	Mix_CloseAudio();
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
	Actor* background = new Actor(this);
	SpriteComponent* sc = new SpriteComponent(background, 50);
	sc->SetTexture(GetTexture("Assets/Background.png"));
	background->SetPosition(Vector2(3392, 224));

	// level file
	std::ifstream in("Assets/level1.txt");

	std::vector<std::vector<Actor*>> levelActors;
	// fill 2d vector with all actors in level
	std::string line;
	float row = 0;;
	while (std::getline(in, line)) {
		std::istringstream iss(line);
		float col = 0;
		std::vector<Actor*> currLine;
		char c;
		while (iss >> c) {
			Vector2 currPos = Vector2(16 + col * 32, 16 + row * 32);
			// block
			if (c >= 'A' && c <= 'I') {
				Block* block = new Block(this, c);
				block->SetPosition(currPos);
			}
			// player
			if (c == 'P') {
				mPlayer = new Player(this);
				mPlayer->SetPosition(currPos);
			}
			// spawner
			if (c == 'Y') {
				Spawner* spawner = new Spawner(this);
				spawner->SetPosition(currPos);
			}
			col++;
		}
		levelActors.push_back(currLine);
		row++;
	}

	mMusicChan = Mix_PlayChannel(-1, GetSound("Assets/Sounds/Music.ogg"), -1);
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

	// delete chunks
	for (auto chunk : mChunks) {
		Mix_FreeChunk(chunk.second);
	}
	mChunks.clear();
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

void Game::AddBlock(Block* block) {
	mBlocks.push_back(block);
}

void Game::RemoveBlock(Block* block) {
	// find block
	std::vector<class Block*>::iterator it;
	it = std::find(mBlocks.begin(), mBlocks.end(), block);
	// erase
	mBlocks.erase(it);
}

const std::vector<class Block*>& Game::GetBlocks() {
	return mBlocks;
}

void Game::AddGoomba(class Goomba* goomba) {
	mGoombas.push_back(goomba);
}
void Game::RemoveGoomba(class Goomba* goomba) {
	// find goomba
	std::vector<class Goomba*>::iterator it;
	it = std::find(mGoombas.begin(), mGoombas.end(), goomba);
	// erase
	mGoombas.erase(it);
}
const std::vector<class Goomba*>& Game::GetGoombas() {
	return mGoombas;
}

class Actor* Game::GetPlayer() {
	return mPlayer;
}

Vector2 Game::GetCameraPos() {
	return mCameraPos;
}

void Game::SetCameraPos(Vector2 newPos) {
	if (newPos.x >= mCameraPos.x) {
		mCameraPos = newPos;
		if (mCameraPos.x < 0) {
			mCameraPos.x = 0;
		}
	}

	
}

Mix_Chunk* Game::GetSound(const std::string & filename) {
	if (mChunks.find(filename) != mChunks.end()) {
		return mChunks[filename];
	}
	Mix_Chunk* chunk = Mix_LoadWAV(filename.c_str());
	// check for failure
	if (!chunk) {
		SDL_Log("Failed to load chunk file %s", filename.c_str());
		return nullptr;
	}
	mChunks.insert(std::pair< std::string, Mix_Chunk*>(filename, chunk));
	return chunk;
}

int Game::getMusicChannel() {
	return mMusicChan;
}