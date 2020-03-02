#include "Agent.h"
#include "Bullet.h"
#include "Game.h"
#include "Physic.h"
#include "Collision.h"

void InstantiateBullet(Agent* shooter, Game* game) {

	Bullet* bullet = new Bullet;
	bullet->type = FRIENDLY;
	bullet->shape = sf::CircleShape(2);
	bullet->shape.setFillColor(sf::Color::White);
	bullet->shape.setPosition(shooter->shape.getPosition());
	bullet->shape.setOrigin(2,2);
	bullet->direction = moveDir(shooter, 1) * (1 + VectorMagnitude(shooter->movingForce));
	game->bullets.push_back(bullet);

}

void UpdatePosition(Bullet* bullet, float deltaTime) {

	bullet->shape.move(bullet->direction / VectorMagnitude(bullet->direction) * deltaTime * bullet->speedMultiplier);

}

bool outOfBounds(Bullet* bullet) {

	return !(bullet->shape.getPosition().x < 1590 && bullet->shape.getPosition().x > 10
		&& bullet->shape.getPosition().y < 890 && bullet->shape.getPosition().y > 10);

}

bool CheckDamage(Bullet* bullet, Agent* agent) {
	
	if (VectorMagnitude(bullet->shape.getPosition() - agent->shape.getPosition()) <= 10 && agent->type != PLAYER){
	
		agent->health -= bullet->damage;
	
		if (agent->health <= 0) {
			agent->health = 100;
			Teleport(agent);
		}

		return true;
	}

	return false;

}