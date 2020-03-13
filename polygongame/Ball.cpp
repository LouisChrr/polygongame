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
    if (agent->type == PLAYER) {
        if (VectorMagnitude(ball->position - agent->convexShape.getPosition()) <= ((agent->convexShape.getPoint(2).x + ball->shape.getRadius()))) {

            return true;
        }

        return false;
    }
    else {
        if (VectorMagnitude(ball->position - agent->shape.getPosition()) <= ((agent->shape.getRadius() + ball->shape.getRadius()))) {

            return true;
        }

        return false;
    }
	

}

void SpawnBalls(Game* game, int totalSize, Agent* agent) {
    int newTotal = totalSize;
    int divide = 0;
    int size = 0;
    if (totalSize == 0) return;
   // printf("TOTALSIZE: %d\n", totalSize);

    if (totalSize < 15) {
        divide = ((totalSize + 4) / 4) + (rand() % ((totalSize + 4) / 4));
    }
    else {
        divide = (totalSize / 10) + (rand() % totalSize / 2);
    }
    //printf("DIVIDE: %d\n", divide);

    for (int i = 0; i < divide; i++) {
        if (newTotal <= 0) return;
        else if (newTotal == 1) size = 1;
        else {
            size = newTotal / divide + (rand() % (divide));
        }
        newTotal -= size;
        if (size == 0) return;

        Ball* ball = new Ball;
        ball->size = size;

        sf::CircleShape shape(10 * size, 5);
        int color = 255 / size;

        ball->shape = shape;
        ball->position = sf::Vector2f(agent->shape.getPosition().x + ((rand() % totalSize) * 10), agent->shape.getPosition().y + ((rand() % totalSize) * 10));
        ball->shape.setFillColor(sf::Color(255, color, 0));
        ball->shape.setOutlineThickness(size * 2);
        ball->shape.setOutlineColor(sf::Color(rand() % 255, color, rand() % 255));
        ball->shape.setPosition(ball->position);
        ball->shape.setOrigin(shape.getRadius(), shape.getRadius());
        game->balls.push_back(ball);
    }



}