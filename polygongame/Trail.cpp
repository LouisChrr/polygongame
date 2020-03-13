#include "Trail.h"
#include "Agent.h"
#include "Physic.h"

void updateTrail(Agent* agent) {

	sf::CircleShape shape = agent->shape;
	shape.setPointCount(20);
	
	shape.setRadius(shape.getRadius());
	//shape.setOrigin(shape.getRadius(), shape.getRadius());

	

	if (agent->type == PLAYER) {
		shape.setRadius(agent->convexShape.getPoint(2).x);
		//shape.setOrigin(agent->convexShape.getPoint(2).x / 2, agent->convexShape.getPoint(2).y / 2);
		//shape.setOrigin(0, agent->convexShape.getPoint(2).y/2);
		shape.setPosition(agent->convexShape.getPosition().x - agent->convexShape.getPoint(2).y / 2 * cos(DegToRad(agent->convexShape.getRotation() - 90)), agent->convexShape.getPosition().y + (agent->convexShape.getPoint(2).y / 2) - agent->convexShape.getPoint(2).y / 2 *sin(DegToRad(agent->convexShape.getRotation() - 90))- agent->convexShape.getPoint(2).y / 2);
		//shape.setPosition(agent->convexShape.getPosition().x agent->convexShape.getPosition().y + (agent->convexShape.getPoint(2).y / 2));


		}

	shape.setOutlineThickness(0);

	agent->trail.shapes.push_back(shape);

	int multplier = 3;

	if (agent->type == PLAYER)
		multplier = 1;

	while (agent->trail.shapes.size() > multplier * (5+agent->score))
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

		if (alpha < 85) {
			alpha = 85;
		}

		(*it).setRadius((shapeIndex / agent->trail.shapes.size()) * agent->shape.getRadius() * 0.6f);
		(*it).setOrigin((*it).getRadius(), (*it).getRadius()*0.5f);

		shapeIndex++;

		

		if (agent->type == PLAYER)
			(*it).setFillColor(sf::Color(0, alpha, 255, alpha));

		else {
			//alpha /= 100;
			if (agent->r == 0) {
				(*it).setFillColor(sf::Color(alpha, agent->trailColor.g - (alpha/5), agent->trailColor.b, alpha));
			}
			else if (agent->g == 0) {
				(*it).setFillColor(sf::Color(agent->trailColor.r, alpha, agent->trailColor.b - (alpha / 5), alpha));
			}
			else {
				(*it).setFillColor(sf::Color(agent->trailColor.r - (alpha / 5), agent->trailColor.g, alpha, alpha));
			}
		}

		/*	if (alpha <= 0) {
			it = agent->trail.shapes.erase(it);
			it++;
			return;
		}*/

		window->draw(*it);
		it++;

	}
}

bool CheckTrailDamage(Agent* agent1, Agent* agent2) {
	if (agent1->type == PLAYER) {
		std::list<sf::CircleShape>::iterator shape = agent2->trail.shapes.begin();

		while (shape != agent2->trail.shapes.end()) {

			if (VectorMagnitude((*shape).getPosition() - agent1->convexShape.getPosition()) <= (*shape).getRadius() + agent1->convexShape.getPoint(2).x) {

				//Respawn(agent1);

				return true;
			}
			shape++;
		}

		return false;
	}
	else {
		std::list<sf::CircleShape>::iterator shape = agent2->trail.shapes.begin();

		while (shape != agent2->trail.shapes.end()) {

			if (VectorMagnitude((*shape).getPosition() - agent1->shape.getPosition()) <= (*shape).getRadius() + agent1->shape.getRadius()) {

				//Respawn(agent1);

				return true;
			}
			shape++;
		}

		return false;
	}
	

}