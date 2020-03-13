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
	std::list<sf::CircleShape> stars;
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
void UpdateGame(float deltatime, Game* game, sf::RenderTexture* tex);
void UpdateTrails(Game* game);
void UpdateBalls(sf::RenderTexture* tex, Game* game, float deltaTime);

void UpdateScore(Agent* agent, int add);

void CheckHeadDamage(Agent* agent, Game* game);

void LerpPosition(sf::RenderTexture* tex, float deltaTime, Game* game);