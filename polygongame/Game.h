#pragma once
#include "Bullet.h"
#include "list"
#include "Agent.h"
#include "Ball.h"

struct Game {

	Agent* player;
	std::list<Bullet*> bullets;
	std::list<Agent*> ennemies;
	std::list<Ball*> balls;
	int maxBallCount = 30;

};

Game* CreateGame(Agent* player);
void UpdateGame(float deltatime, Game* game, sf::RenderWindow* window);
void UpdateTrails(Game* game);
void UpdateBalls(sf::RenderWindow* window, Game* game, float deltaTime);
void CheckHeadDamage(Agent* agent, Game* game);