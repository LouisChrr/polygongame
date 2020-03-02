#include "Game.h"

void CreateGame() {

	Game* game = new Game;
	game->bullets = std::list<Bullet*>();

}