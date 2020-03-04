#include "Agent.h"
#include "Ball.h"
#include "Game.h"
#include <cstdlib>
#include "Physic.h"

void CreateBall(Game* game, int size) {
	Ball* ball = new Ball;
	ball->size = size;
		
	sf::CircleShape shape(10 * size,4 + size);
	int color = 255 / size;
	
	ball->shape = shape;
	ball->position = sf::Vector2f(rand() % game->xMax, rand() % game->yMax);
	ball->shape.setFillColor(sf::Color(255, color, 0));
	ball->shape.setPosition(ball->position);
	ball->shape.setOrigin(shape.getRadius(), shape.getRadius());
	game->balls.push_back(ball);
}

bool CheckCollision(Ball* ball, Agent* agent) {

	if (VectorMagnitude(ball->position - agent->shape.getPosition()) <= ((agent->shape.getRadius() + ball->shape.getRadius())*1.25f)) {

		return true;
	}

	return false;

}