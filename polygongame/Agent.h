#pragma once
#include <SFML/Graphics.hpp>
#include "Trail.h"

struct Game;

enum Type { PLAYER, ENEMY };

struct Agent {

	Trail trail;
	Type type;
	float turnSpeed = 360.0f;
	float moveSpeed = 35.0f;
	float acceleration;
	float damper = 0.98f;
	sf::Vector2f movingForce;
	int health = 100;
	int energy = 100;
	sf::CircleShape shape;
	int tries;
	float rotation;
	float distance;
	float lastRotation;
	float score = 0;

	sf::Color trailColor;
};

Agent* CreatePlayer();
Agent* CreateEnemy();

void Rotate(Agent* player, float direction, float deltaTime);
void UpdateEnemyRotation(Agent* player, Agent* enemy, float deltaTime);
void MoveAgent(Agent* player, float deltaTime);
sf::Vector2f moveDir(Agent* agent, int direction);
void Respawn(Agent* agent, Game* game);