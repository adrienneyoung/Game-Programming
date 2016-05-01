/*
Adrienne Young and Ivy Cheung
CS-3113 Intro to Game Programming
Final Project
Spring 2016

Controls:
	Move using left and right arrow keys 
	Jump using up arrow key
	Shoot bullets using space
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
