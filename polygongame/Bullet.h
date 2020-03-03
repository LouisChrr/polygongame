#pragma once
#include "Agent.h"
class Game;

enum bulletType { FRIENDLY, HOSTILE };

struct Bullet {

	bulletType type;
	int damage = 50;
	float speedMultiplier = 1000.0f;
	sf::CircleShape shape;
	sf::Vector2f direction;

};

void InstantiateBullet(Agent* shooter, Game* game);
void UpdatePosition(Bullet* bullet, float deltaTime);
bool outOfBounds(Bullet* bullet, Agent* player);
bool CheckDamage(Bullet* bullet, Agent* agent);