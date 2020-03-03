#pragma once
#include "Agent.h"

class Game;

struct Ball {
	
	sf::CircleShape shape;
	sf::Vector2f position;

};

void CreateBall(Game* game);
void BallSpawner(sf::RenderWindow* window, Game* game, float deltaTime);