#include "Trail.h"
#include "Agent.h"

void updateTrail(Agent* agent) {

	sf::CircleShape shape = agent->shape;
	shape.setPointCount(100);
	shape.setRadius(shape.getRadius());
	shape.setOrigin(shape.getRadius(), shape.getRadius());
	shape.setOutlineThickness(0);

	agent->trail.shapes.push_back(shape);

	if (agent->trail.shapes.size() > 10 * (1+agent->score))
		agent->trail.shapes.pop_front();

}

void drawTrail(Agent* agent, sf::RenderWindow* window, float deltaTime) {

	std::list<sf::CircleShape>::iterator it = agent->trail.shapes.begin();

	float alpha = 0;

	float shapeIndex = 0.0f;

	while (it != agent->trail.shapes.end()) {

		/*alpha = (*it).getFillColor().a;
		alpha -= (deltaTime / agent->trail.shapes.size());

		if (alpha <= 0)
			alpha = 0;
		float alpha = (*it).getFillColor().a;
		alpha -= (deltaTime);*/

		alpha = (shapeIndex / agent->trail.shapes.size()) * 255.0f;

		(*it).setRadius((shapeIndex / agent->trail.shapes.size()) * agent->shape.getRadius() * 0.6f);
		(*it).setOrigin((*it).getRadius(), (*it).getRadius());

		shapeIndex++;

		if (agent->type == PLAYER)
			(*it).setFillColor(sf::Color(0, alpha, 255, alpha));

		else
			(*it).setFillColor(sf::Color(alpha, 255 , 0, alpha));

	/*	if (alpha <= 0) {
			it = agent->trail.shapes.erase(it);
			it++;
			return;
		}*/

		window->draw(*it);
		it++;

	}
}