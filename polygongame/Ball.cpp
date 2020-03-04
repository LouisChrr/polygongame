#include "Agent.h"
#include "Ball.h"
#include "Game.h"
#include <cstdlib>
#include "Physic.h"

void CreateBall(Game* game) {

	sf::CircleShape shape(5,5);

	Ball* ball = new Ball;
	ball->shape = shape;
	ball->position = sf::Vector2f(rand() % 1600, rand() % 900);
	ball->shape.setFillColor(sf::Color::Yellow);
	ball->shape.setPosition(ball->position);
	ball->shape.setOrigin(2.5f, 2.5f);
	game->balls.push_back(ball);

}

bool CheckCollision(Ball* ball, Agent* agent) {

	if (VectorMagnitude(ball->position - agent->shape.getPosition()) <= agent->shape.getRadius() && agent->type == PLAYER) {
		return true;
	}

	return false;

}