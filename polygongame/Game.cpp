#include "Game.h"
#include "Agent.h"
#include "Physic.h"
#include "Ball.h"
#define ennemy_nb 2

void UpdateText(Game* game, sf::RenderWindow* window) {

	sf::Font font;
	font.loadFromFile("OldeEnglish.ttf");
	sf::Text scoreText = sf::Text(std::to_string(0), font);
	scoreText.setOrigin(scoreText.getCharacterSize() / 2, scoreText.getCharacterSize() / 2);
	scoreText.setCharacterSize(60);
	scoreText.setFillColor(sf::Color::White);

	scoreText.setString(std::to_string(game->player->score));
	scoreText.setPosition(game->player->shape.getPosition().x, game->player->shape.getPosition().y - 120);

	window->draw(scoreText);

}

Game* CreateGame(Agent* player) {
	Game* game = new Game;
	game->player = player;
	game->bullets = std::list<Bullet*>();
	game->ennemies = std::list<Agent*>();
	game->balls = std::list<Ball*>();

	for (int i = 0; i < ennemy_nb; i++) {
		Agent* ennemy = CreateEnemy();
		game->ennemies.push_back(ennemy);
		//printf("Ennemy cree nb: %d\n", i);
	}

	return game;
}

void UpdateTrails(Game* game) {
	updateTrail(game->player);
	
	std::list<Agent*>::iterator it = game->ennemies.begin();
	while (it != game->ennemies.end()) {
		updateTrail(*it);
		it++;
	}
}


void CheckHeadDamage(Agent* agent, Game* game) {

	std::list<Agent*>::iterator enemy = game->ennemies.begin();

	while (enemy != game->ennemies.end()) {

		if (agent != *enemy && VectorMagnitude((*enemy)->shape.getPosition() - agent->shape.getPosition()) <= (*enemy)->shape.getRadius() + agent->shape.getRadius() * 1.1f) {

			if ((*enemy)->score > agent->score) {
				(*enemy)->score += agent->score;
				Respawn(agent);
			}
			else {
				agent->score += (*enemy)->score;
				Respawn((*enemy));
			}
		}
		enemy++;
	}

}

void UpdateBalls(sf::RenderWindow* window, Game* game, float deltaTime) {
	//printf("ON UPDATE LES BALLS\n");
	std::list<Ball*>::iterator it = game->balls.begin();
	while (it != game->balls.end()) {

		if (CheckCollision(*it, game->player)){
			it = game->balls.erase(it);
			game->player->score++;
			return;
	}

		window->draw((*it)->shape);
		it++;

	}
	//printf("FIN UPDATE BALLS\n");

}

void UpdateGame(float deltatime, Game* game, sf::RenderWindow* window) {
	// BALLS
	UpdateBalls(window, game, deltatime);


	// PLAYER
	drawTrail(game->player, window, deltatime);
	MoveAgent(game->player, deltatime);
	window->draw(game->player->shape);

	//UI
	UpdateText(game, window);


	//printf("UPDATE PLAYER FINI // \n");
	// ENNEMIES
	std::list<Agent*>::iterator it = game->ennemies.begin();
	while (it != game->ennemies.end()) {
		Agent* enemy = *it;
		UpdateEnemyRotation(game->player, enemy, deltatime);
		drawTrail(enemy, window, deltatime);
		AddForce(enemy, moveDir(enemy, 1), deltatime);
		MoveAgent(enemy, deltatime);
		window->draw(enemy->shape);
		it++;
	}

	//printf("UPDATE ENEMY FINI // \n");
	// BULLETS

	bool erase = false;

	std::list<Bullet*>::iterator bullet = game->bullets.begin();
	
	while (bullet != game->bullets.end()) {
		UpdatePosition(*bullet, deltatime);

		if (outOfBounds(*bullet, game->player) || erase == true) {
			bullet = game->bullets.erase(bullet);
			erase = false;
		}
		else {
			std::list<Agent*>::iterator enemy = game->ennemies.begin();
			while (enemy != game->ennemies.end()) {

				bool check = CheckDamage(*bullet, *enemy);
				if (check == false) {
					window->draw((*bullet)->shape);
				}
				else {
					(*enemy)->health -= (*bullet)->damage;

					if ((*enemy)->health <= 0) {
						(*enemy)->health = 100;
						Respawn((*enemy));
					}

					erase = true;
				}
				enemy++;
			}
			bullet++;
		}
	}

	//TRAILDAMAGE

	std::list<Agent*>::iterator enemy = game->ennemies.begin();

	CheckHeadDamage(game->player, game);

	while (enemy != game->ennemies.end()) {

		CheckHeadDamage(*enemy, game);

		if (CheckTrailDamage((*enemy), game->player)) {
			Respawn(*enemy);
		}

		if (CheckTrailDamage(game->player, (*enemy))) {
			Respawn(game->player);
		}

		enemy++;
	}

	//printf("UPDATE BULLETS FINI // \n");
	//printf("FIN UPDATE GAME // \n");
}