#pragma once
#include "Bullet.h"
#include "list"
#include "Agent.h"

struct Game {

	Agent* player;
	std::list<Bullet*> bullets;
	std::list<Agent*> ennemies;

};


Game* CreateGame(Agent* player);
void UpdateGame(float deltatime, Game* game, sf::RenderWindow* window);
void UpdateTrails(Game* game);