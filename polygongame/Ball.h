#pragma once
#include "Agent.h"

struct Game;

struct Ball {
	
	sf::CircleShape shape;
	sf::Vector2f position;

};

void CreateBall(Game* game);