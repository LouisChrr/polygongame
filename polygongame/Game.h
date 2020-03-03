#pragma once
#include "Bullet.h"
#include "list"
#include "Ball.h"

struct Game {

	std::list<Bullet*> bullets;
	std::list<Ball*> balls;
	int maxBallCount = 30;
	int score = 0;

};