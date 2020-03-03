#include <SFML/Graphics.hpp>
#include <math.h>
#include "Agent.h"
#include "Physic.h"

#define M_PI 3.14159265

void AddForce(Agent* agent, sf::Vector2f force, float deltaTime) {

	agent->movingForce += force * deltaTime * agent->moveSpeed;
	if (agent->type == ENEMY) {
		//printf("ENEMY AGENT MOVING FORCE BEFORE CLAMP: %f\n", VectorMagnitude(agent->movingForce));

		if (VectorMagnitude(agent->movingForce) > 200) {
			agent->movingForce /= VectorMagnitude(agent->movingForce);
			agent->movingForce /= 1.0f / 200;
		}
	}
	else if (agent->type == PLAYER) {
		if (VectorMagnitude(agent->movingForce) > 400) {
			agent->movingForce /= VectorMagnitude(agent->movingForce);
			agent->movingForce /= 1.0f / 200;
		}
	}
	

}

float DegToRad(float deg) {

	return (2 * M_PI * (deg / 360));

}

float VectorMagnitude(sf::Vector2f vector) {

	return sqrt(vector.x * vector.x + vector.y * vector.y);

}

sf::Vector2f ClampForce(sf::Vector2f force) {

	return sf::Vector2f(0,0);

}