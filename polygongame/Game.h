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
	int maxBallCount = 50;

	std::list<Agent*> winner;

	float targetZoom;
	sf::Vector2f targetPos;

	int xMax = 8000;
	int yMax = 8000;

	int frame = 0;

	sf::Text* scoreText;
};

Game* CreateGame(Agent* player);
void UpdateGame(float deltatime, Game* game, sf::RenderWindow* window);
void UpdateTrails(Game* game);
void UpdateBalls(sf::RenderWindow* window, Game* game, float deltaTime);

void UpdateScore(Agent* agent, int add);

void CheckHeadDamage(Agent* agent, Game* game);

void LerpPosition(sf::RenderWindow* window, float deltaTime, Game* game);