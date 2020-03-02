#pragma once
#include "list"
#include <SFML/Graphics.hpp>

class Agent;

struct Trail {

	std::list<sf::CircleShape> shapes;

};

void updateTrail(Agent* agent);
void drawTrail(Agent* agent, sf::RenderWindow* window);