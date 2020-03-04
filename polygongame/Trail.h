#pragma once
#include "list"
#include <SFML/Graphics.hpp>

class Agent;

struct Trail {

	std::list<sf::CircleShape> shapes;
	int length = 0;

};

void updateTrail(Agent* agent);
void drawTrail(Agent* agent, sf::RenderWindow* window, float deltaTime);
bool CheckTrailDamage(Agent* agent1, Agent* agent2);