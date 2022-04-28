//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"
#include <algorithm>
#include "Actor.h"
#include <fstream>
#include "Renderer.h"
#include "Random.h"
#include "Player.h"
#include "SideBlock.h"
#include "Block.h"

const float WINDOW_WIDTH = 1024.0f;
const float WINDOW_HEIGHT = 768.0f;

Game::Game()
:mIsRunning(true)
{
	
}

bool Game::Initialize()
{
	Random::Init();

	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	// TODO: Create renderer
	mRenderer = new Renderer(this);
	if (!mRenderer->Initialize(1024.0f, 768.0f))
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	LoadData();

	mTicksCount = SDL_GetTicks();
	
	return true;
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				mIsRunning = false;
				break;
		}
	}
	
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	for (auto actor : mActors)
	{
		actor->ProcessInput(state);
	}
}

void Game::UpdateGame()
{
	// Compute delta time
	// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	if (deltaTime > 0.01667f)
	{
		deltaTime = 0.01667f;
	}
	mTicksCount = SDL_GetTicks();
	
	// Make copy of actor vector
	// (iterate over this in case new actors are created)
	std::vector<Actor*> copy = mActors;
	// Update all actors
	for (auto actor : copy)
	{
		actor->Update(deltaTime);
	}

	// Add any actors to destroy to a temp vector
	std::vector<Actor*> destroyActors;
	for (auto actor : mActors)
	{
		if (actor->GetState() == ActorState::Destroy)
		{
			destroyActors.emplace_back(actor);
		}
	}

	// Delete the destroyed actors (which will
	// remove them from mActors)
	for (auto actor : destroyActors)
	{
		delete actor;
	}
}

void Game::GenerateOutput()
{
	mRenderer->Draw();
}

void Game::LoadData()
{
	mPlayer = new Player(this);
	
	for (int i = 0; i < 6; i++) {
		int index = 1;
		if (i % 2 == 0) {
			index = 3;
		}
		SideBlock* temp1 = new SideBlock(this, index);
		temp1->SetPosition(Vector3(i * 500.0f, 500.0f, 0.0f));
		SideBlock* temp2 = new SideBlock(this, index);
		temp2->SetPosition(Vector3(i * 500.0f, -500.0f, 0.0f));
	}

	LoadBlocks("Assets/Blocks/1.txt", 1000);
	LoadBlocks("Assets/Blocks/2.txt", 2000);

	Matrix4 projMatrix = Matrix4::CreatePerspectiveFOV(1.22f, WINDOW_WIDTH, WINDOW_HEIGHT, 10.0f, 10000.0f);
	mRenderer->SetProjectionMatrix(projMatrix);

	Matrix4 viewMatrix = Matrix4::CreateLookAt(Vector3(-300, 0, 100), Vector3(20, 0, 0), Vector3::UnitZ);
	mRenderer->SetViewMatrix(viewMatrix);

	mMusicChan = Mix_PlayChannel(-1, GetSound("Assets/Sounds/ShipLoop.wav"), -1);
}

void Game::UnloadData()
{
	// Delete actors
	// Because ~Actor calls RemoveActor, have to use a different style loop
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	// Destroy sounds
	for (auto s : mSounds)
	{
		Mix_FreeChunk(s.second);
	}
	mSounds.clear();
}

Mix_Chunk* Game::GetSound(const std::string& fileName)
{
	Mix_Chunk* chunk = nullptr;
	auto iter = mSounds.find(fileName);
	if (iter != mSounds.end())
	{
		chunk = iter->second;
	}
	else
	{
		chunk = Mix_LoadWAV(fileName.c_str());
		if (!chunk)
		{
			SDL_Log("Failed to load sound file %s", fileName.c_str());
			return nullptr;
		}

		mSounds.emplace(fileName, chunk);
	}
	return chunk;
}

int Game::GetMusicChannel() {
	return mMusicChan;
}

void Game::Shutdown()
{
	UnloadData();
	Mix_CloseAudio();
	mRenderer->Shutdown();
	delete mRenderer;
	SDL_Quit();
}

void Game::AddActor(Actor* actor)
{
	mActors.emplace_back(actor);
}

void Game::RemoveActor(Actor* actor)
{
	auto iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		auto iter2 = mActors.end() - 1;
		std::iter_swap(iter, iter2);
		mActors.pop_back();
	}
}

Player* Game::GetPlayer() {
	return mPlayer;
}

void Game::AddBlock(Block* block) {
	mBlocks.push_back(block);
}

void Game::RemoveBlock(Block* block) {
	// find block
	std::vector<class Block*>::iterator it;
	it = std::find(mBlocks.begin(), mBlocks.end(), block);
	mBlocks.erase(it);
}

const std::vector<class Block*>& Game::GetBlocks() {
	return mBlocks;
}

void Game::LoadBlocks(std::string filename, float x) {
	std::ifstream in(filename);

	std::vector<std::vector<Actor*>> levelActors;
	// fill 2d vector with all actors in level
	std::string line;
	float row = 0;;
	while (std::getline(in, line)) {
		std::istringstream iss(line);
		float col = 0;
		char c;
		while (iss >> c) {
			Vector3 currPos = Vector3(x, -237.5f + (col * 25), 237.5f - (row * 25));
			// normal block
			if (c == 'A') {
				Block* block = new Block(this, false);
				block->SetPosition(currPos);
			}
			// exploding block
			if (c == 'B') {
				Block* block = new Block(this, true);
				block->SetPosition(currPos);
			}
			col++;
		}
		row++;
	}
}