#pragma once
#include <SFML/Graphics.hpp>
#include "Trail.h"

enum Type { PLAYER, ENEMY };

struct Agent {

	Trail trail;
	Type type;
	float turnSpeed = 360.0f;
	float moveSpeed = 35.0f;
	float acceleration;
	float damper = 0.996f;
	sf::Vector2f movingForce;
	int health = 100;
	int energy = 100;
	sf::CircleShape shape;

};

Agent* CreatePlayer();
Agent* CreateEnemy();

void Rotate(Agent* player, int direction, float deltaTime);
void UpdateEnemyRotation(Agent* player, Agent* enemy, float deltaTime);
void MoveAgent(Agent* player, float deltaTime);
sf::Vector2f moveDir(Agent* agent, int direction);
void Teleport(Agent* agent);