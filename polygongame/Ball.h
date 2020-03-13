#pragma once
#include "Agent.h"

struct Game;

struct Ball {
	
	sf::CircleShape shape;
	sf::Color originalColor;
	sf::Vector2f position;
	int size;
	bool twinkle;
	sf::Color tempColor;

};

void CreateBall(Game* game, int size);
bool CheckCollision(Ball* ball, Agent* agent);
void SpawnBalls(Game* game, int totalSize, Agent* agent);
void Twinkle(Game* game, float timer);