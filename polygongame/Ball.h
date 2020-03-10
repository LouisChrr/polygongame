#pragma once
#include "Agent.h"

struct Game;

struct Ball {
	
	sf::CircleShape shape;
	sf::Vector2f position;
	int size;

};

void CreateBall(Game* game, int size);
bool CheckCollision(Ball* ball, Agent* agent);
void SpawnBalls(Game* game, int totalSize, Agent* agent);