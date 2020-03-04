#include "Agent.h"
#include "Bullet.h"
#include "Game.h"
#include "Physic.h"


void InstantiateBullet(Agent* shooter, Game* game) {

	Bullet* bullet = new Bullet;
	bullet->type = FRIENDLY;
	bullet->shape = sf::CircleShape(5);
	bullet->shape.setFillColor(sf::Color::White);
	bullet->shape.setPosition(shooter->shape.getPosition());
	bullet->shape.setOrigin(2,2);
	bullet->direction = moveDir(shooter, 1) * (1 + VectorMagnitude(shooter->movingForce));
	game->bullets.push_back(bullet);

	UpdateScore(shooter, -1);

}

void UpdatePosition(Bullet* bullet, float deltaTime) {

	bullet->shape.move(bullet->direction / VectorMagnitude(bullet->direction) * deltaTime * bullet->speedMultiplier);

}

bool outOfBounds(Bullet* bullet, Agent* player) {

	return !(bullet->shape.getPosition().x < player->shape.getPosition().x + 800 && bullet->shape.getPosition().x > player->shape.getPosition().x - 800
		&& bullet->shape.getPosition().y < player->shape.getPosition().y + 450 && bullet->shape.getPosition().y > player->shape.getPosition().y - 450);

}

bool CheckDamage(Bullet* bullet, Agent* agent) {
	
	if (VectorMagnitude(bullet->shape.getPosition() - agent->shape.getPosition()) <= 40 && agent->type != PLAYER){
		return true;
	}

	return false;

}