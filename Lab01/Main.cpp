//
//  Main.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"
#include <cmath> 

int main(int argc, char** argv)
{
	// driver code
	Game game;
	bool initSuccess = game.Initialize();
	if (initSuccess) {
		game.RunLoop();
	}
	game.Shutdown();
	return 0;
}