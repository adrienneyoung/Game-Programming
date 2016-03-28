/*
Adrienne Young
CS-3113 Intro to Game Programming
HW #5 Scrolling Platformer
Spring 2016

Make a simple scrolling platformer game demo.
- It must use a tilemap or static/dynamic entities.
- It must scroll.
- It must be either procedurally generated or load levels from a file.

Controls:
	Move using left, right, up, and down arrow keys 
	Jump using space
*/

#include "Platformer.h"

int main(int argc, char *argv[]) {
	Platformer game;

	while (!game.Run()) {
		//update and render the game
	}

	SDL_Quit();
	return 0;
}
