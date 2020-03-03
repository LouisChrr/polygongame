#include <SFML/Graphics.hpp>
#include <cstdlib>
#include "Agent.h"
#include "Physic.h"
#include <math.h>
#include "Game.h"

Agent* CreatePlayer() {

	Agent* player = new Agent;
	player->type = PLAYER;
	player->shape = sf::CircleShape(10, 3);
	player->shape.setFillColor(sf::Color::Green);
	player->shape.setPosition(400,400);
	player->shape.setOrigin(player->shape.getRadius(), player->shape.getRadius());
	player->shape.setOutlineThickness(1);
	player->shape.setOutlineColor(sf::Color(0,100,0));

	return player;

}

Agent* CreateEnemy() {

	Agent* enemy = new Agent;
	enemy->type = ENEMY;
	enemy->shape = sf::CircleShape(10, 8);
	enemy->shape.setFillColor(sf::Color::Red);
	enemy->shape.setPosition(0, 0);
	enemy->shape.setOrigin(enemy->shape.getRadius(), enemy->shape.getRadius());
	enemy->shape.setOutlineThickness(1);
	enemy->shape.setOutlineColor(sf::Color(100, 0, 0));

	return enemy;

}

void Rotate(Agent* player, int direction, float deltaTime) {

	player->shape.rotate(1 * direction * player->turnSpeed * deltaTime);

}

void MoveAgent(Agent* agent, float deltaTime) {
	
	agent->movingForce *= agent->damper;
	agent->shape.move(agent->movingForce * deltaTime);

}

sf::Vector2f moveDir(Agent* agent, int direction) {

	return(sf::Vector2f(agent->moveSpeed * direction * std::cos(DegToRad(agent->shape.getRotation() - 90)), agent->moveSpeed * direction * std::sin(DegToRad(agent->shape.getRotation() - 90))));

}

void UpdateEnemyRotation(Agent* player, Agent* enemy, float deltaTime) {

	sf::Vector2f lookDir = player->shape.getPosition() - enemy->shape.getPosition();
	sf::Vector2f currentDir = moveDir(enemy, 1);

	float angle = std::acos((lookDir.x * currentDir.x + lookDir.y * currentDir.y) / VectorMagnitude(lookDir) * VectorMagnitude(currentDir));

	Rotate(enemy, -1, deltaTime);

}

void Teleport(Agent* agent) {

	agent->shape.setPosition(sf::Vector2f(rand() % 1600, rand()%900));
	
}