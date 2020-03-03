#pragma once
#include "Bullet.h"
#include "list"
<<<<<<< HEAD
#include "Agent.h"
=======
#include "Ball.h"
>>>>>>> develop-jerome

struct Game {

	Agent* player;
	std::list<Bullet*> bullets;
<<<<<<< HEAD
	std::list<Agent*> ennemies;
=======
	std::list<Ball*> balls;
	int maxBallCount = 30;
	int score = 0;
>>>>>>> develop-jerome

};


Game* CreateGame(Agent* player);
void UpdateGame(float deltatime, Game* game, sf::RenderWindow* window);
void UpdateTrails(Game* game);