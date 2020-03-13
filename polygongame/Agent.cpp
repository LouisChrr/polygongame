#include <SFML/Graphics.hpp>
#include <cstdlib>
#include "Agent.h"
#include "Physic.h"
#include <math.h>
#include "Game.h"
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include <math.h>

Agent* CreatePlayer() {

	Agent* player = new Agent;
	player->type = PLAYER;
	player->shape = sf::CircleShape(40, 3);
	player->shape.setFillColor(sf::Color::Green);
	player->shape.setPosition(4000,4000);
	player->shape.setOrigin(player->shape.getRadius(), player->shape.getRadius());
	player->shape.setOutlineThickness(1);
	player->shape.setOutlineColor(sf::Color(0,100,0));

	return player;

}

Agent* CreateEnemy() {

	Agent* enemy = new Agent;
	enemy->type = ENEMY;
	//enemy->damper = 0.8f;
	enemy->shape = sf::CircleShape(40, 8);
	enemy->shape.setFillColor(sf::Color(255, rand() % 255, rand() % 255));

	enemy->shape.setPosition(sf::Vector2f(rand() % 8000, rand() % 8000));

	enemy->trailColor = sf::Color(rand() % 255, rand() % 255, rand() % 255);


	enemy->score = 10;
	
	int randomx = 0;
	int randomy = 0;
	int randombool = (rand() % 100);
	if (randombool < 50) {
		 randomx = (rand() % 800) + 800;
		 randomy = -(rand() % 550) + 550;
	}
	else {
		 randomx = -(rand() % 800) + 800;
		 randomy = (rand() % 550) + 550;
	}

	enemy->shape.setPosition(randomx, randomy);

	enemy->shape.setOrigin(enemy->shape.getRadius(), enemy->shape.getRadius());
	enemy->shape.setOutlineThickness(1);
	enemy->shape.setOutlineColor(sf::Color(100, 0, 0));

	// IA ENNEMI
	enemy->tries = 0;
	enemy->rotation = 1;
	enemy->lastRotation = 1;
	enemy->distance = 0;

	return enemy;
}

void Rotate(Agent* player, float direction, float deltaTime) {

	player->shape.rotate(1 * direction * player->turnSpeed * deltaTime);

}

void MoveAgent(Agent* agent, float deltaTime) {

	agent->movingForce *= agent->damper;
	agent->shape.move(agent->movingForce * deltaTime);

	if (agent->type == PLAYER) {

		if (agent->shape.getPosition().x < 0 + agent->shape.getRadius() || agent->shape.getPosition().x > 8000 - agent->shape.getRadius())
			agent->movingForce.x = -agent->movingForce.x;

		if (agent->shape.getPosition().y < 0 + agent->shape.getRadius() || agent->shape.getPosition().y > 8000 - agent->shape.getRadius())
			agent->movingForce.y = -agent->movingForce.y;
	}
}

sf::Vector2f moveDir(Agent* agent, int direction) {

	return(sf::Vector2f(agent->moveSpeed * direction * std::cos(DegToRad(agent->shape.getRotation() - 90)), agent->moveSpeed * direction * std::sin(DegToRad(agent->shape.getRotation() - 90))));

}

void UpdateEnemyRotation(Agent* player, Agent* enemy, float deltaTime) {

	sf::Vector2f playerPos = player->shape.getPosition();
	sf::Vector2f enemyPos = enemy->shape.getPosition();


	sf::Vector2f lookDir = playerPos - enemyPos;
	sf::Vector2f currentDir = moveDir(enemy, 1);

	//printf("Vector lookdir before: %f\n", VectorMagnitude(lookDir));

	if (VectorMagnitude(lookDir) > 100) {
		int surplus = VectorMagnitude(lookDir) - 100;
		//lookDir.x -= surplus;
		//lookDir.y -= surplus;
	}


	//lookDir.x *= 10;
	//lookDir.y *= 10;


	//printf("Vector lookdir after: %f\n", VectorMagnitude(lookDir));

	//AddForce(enemy,lookDir, deltaTime);

	//float angle = std::acos((lookDir.x * currentDir.x + lookDir.y * currentDir.y) / VectorMagnitude(lookDir) * VectorMagnitude(currentDir));S
	
	//printf("distance X: %f,%f\n", distance.x);
	//printf("distance Y: %f,%f\n", distance.y);
	sf::Vector2f distanceVector = playerPos - enemyPos;
	float distance = VectorMagnitude(distanceVector);
	//printf("Vector magnitude: %f\n", VectorMagnitude(distance));
	
	// "IA" PART /////////////////////////////////////////////////////////////////////////////////////


	enemy->tries++;

	int tries = (rand() % 50)+250;
	tries = 50;

	//printf("TRIES:%d\n", tries);


	//tries *= 100000000000;
	
	
	if (enemy->tries >= tries) {
		//printf("ON TRY TRIES ENNEMI TRIES %d\n", tries);
		

		if (enemy->rotation != 0) {
			enemy->distance = distance;
			enemy->rotation = 0;
			//printf("New enemy rotation: %d\n", enemy->rotation);
			return;
		}

		if (distance > enemy->distance) { // SI LENNEMI SEST ELOIGNER
			int random = rand() % 101;
			if (random < 50) {
				if (enemy->lastRotation > 0) {
					enemy->lastRotation = 0;
					return;
				}

				float random2 = rand() % 100;
				if (random2 < 25) random2 += 25;
				enemy->rotation = -(random2/100);
			}
			else {
				if (enemy->lastRotation < 0) {
					enemy->lastRotation = 0;
					return;
				}

				float random2 = rand() % 100;
				if (random2 < 25) random2 += 25;

				enemy->rotation = (random2 / 100);
				
			}

			enemy->lastRotation = enemy->rotation;
			//printf("Lennemi SELOIGNE\n");
			//printf("New enemy rotation: %f\n", enemy->rotation);

		}
		else {
			enemy->rotation = 0;
			//printf("Lennemi se rapproche oklm\n");
			//printf("Ennemy lastdistance: %f\n", enemy->distance);
			//printf("Ennemy distance: %f\n", distance);

		}

		enemy->tries = 0;
	}

	enemy->distance = distance;
	Rotate(enemy, enemy->rotation, deltaTime);

	if (player->score > 100)
		AddForce(enemy, lookDir, deltaTime);

}

void Respawn(Agent* agent, Game* game) {

	SpawnBalls(game,agent->score, agent);

	agent->health = 100;
	UpdateScore(agent, -agent->score);

	agent->shape.setPosition(sf::Vector2f(rand() % 8000, rand()%8000));
	agent->shape.setRadius(40);

	agent->shape.setOrigin(40,40);

	agent->acceleration = 0;
	agent->movingForce = sf::Vector2f(0, 0);

	agent->trail.shapes.clear();

}