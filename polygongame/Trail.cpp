#include "Trail.h"
#include "Agent.h"

void updateTrail(Agent* agent) {

	sf::CircleShape shape = agent->shape;
	shape.setRadius(8);
	shape.setOrigin(shape.getRadius(), shape.getRadius());
	shape.setOutlineThickness(0);

	agent->trail.shapes.push_back(shape);

	if (agent->trail.shapes.size() > 100)
	if (agent->trail.shapes.size() > 5)
		agent->trail.shapes.pop_front();

}

void drawTrail(Agent* agent, sf::RenderWindow* window) {
void drawTrail(Agent* agent, sf::RenderWindow* window, float deltaTime) {

	std::list<sf::CircleShape>::iterator it = agent->trail.shapes.begin();

	float alpha = 0;

	while (it != agent->trail.shapes.end()) {

<<<<<<< HEAD
		alpha = (*it).getFillColor().a;
		alpha *= 0.999f;

		if (alpha <= 0)
			alpha = 0;
=======
		float alpha = (*it).getFillColor().a;
		alpha -= (deltaTime);
>>>>>>> develop-jerome

		if (agent->type == PLAYER)
			(*it).setFillColor(sf::Color(0, alpha, 255, alpha));

		else

		window->draw(*it);
		if (alpha <= 0) {
			it = agent->trail.shapes.erase(it);
		}

		window->draw(*it);
		it++;

	}
}