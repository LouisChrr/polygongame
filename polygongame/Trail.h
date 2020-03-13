#pragma once
#include "list"
#include <SFML/Graphics.hpp>
#include "cstdlib"

class Agent;

struct Trail {

	std::list<sf::CircleShape> shapes;
	int length = 0;

};

void updateTrail(Agent* agent);
void drawTrail(Agent* agent, sf::RenderTexture* tex, float deltaTime);
bool CheckTrailDamage(Agent* agent1, Agent* agent2);