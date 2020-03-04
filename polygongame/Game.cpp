#include "Game.h"
#include "Agent.h"
#include "Physic.h"
#include "Ball.h"
#define ennemy_nb 5

void UpdateText(Game* game, sf::RenderWindow* window) {
	sf::Font font;
	font.loadFromFile("OldeEnglish.ttf");
	std::string score = std::to_string(game->player->score);
	game->scoreText = new sf::Text(score, font);

	//game->scoreText->setString(std::to_string(game->player->score));
	game->scoreText->setPosition(game->player->shape.getPosition().x, game->player->shape.getPosition().y - 120);

	window->draw(*(game->scoreText));
}

Game* CreateGame(Agent* player) {
	Game* game = new Game;
	game->player = player;
	game->bullets = std::list<Bullet*>();
	game->ennemies = std::list<Agent*>();
	game->balls = std::list<Ball*>();
	sf::Font font;
	font.loadFromFile("OldeEnglish.ttf");
	std::string score = std::to_string(game->player->score);
	game->scoreText = new sf::Text(score, font);

	game->scoreText->setOrigin(game->scoreText->getCharacterSize() / 2, game->scoreText->getCharacterSize() / 2);
	game->scoreText->setCharacterSize(60);
	game->scoreText->setFillColor(sf::Color::White);
	game->scoreText->setString(std::to_string(game->player->score));
	game->scoreText->setPosition(game->player->shape.getPosition().x, game->player->shape.getPosition().y - 120);

	

	for (int i = 0; i < ennemy_nb; i++) {
		Agent* ennemy = CreateEnemy();
		game->ennemies.push_back(ennemy);
		//printf("Ennemy cree nb: %d\n", i);
	}

	return game;
}



void UpdateScore(Agent* agent, int add) {
	agent->score += add;

	if (agent->score <= 0)
		agent->score = 0;

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

		if (agent != *enemy && VectorMagnitude((*enemy)->shape.getPosition() - agent->shape.getPosition()) <= (*enemy)->shape.getRadius() + agent->shape.getRadius() * 1.15f) {

			if ((*enemy)->score > agent->score) {
				(*enemy)->score += agent->score;
				Respawn(agent);
			}
			else if ((*enemy)->score < agent->score) {
				agent->score += (*enemy)->score;
				Respawn((*enemy));
			}
		}
		enemy++;
	}

}

void UpdateBalls(sf::RenderWindow* window, Game* game, float deltaTime) {
	//printf("ON UPDATE LES BALLS\n");
	//std::list<Ball*>::iterator it = game->balls.begin();

	if (game->frame % 2 == 0) {
		//printf("Update player - balls %d\n", game->frame);
		std::list<Ball*>::iterator it = game->balls.begin();
		while (it != game->balls.end()) {
				
				if (CheckCollision(*it, game->player)) {
					UpdateScore(game->player, (*it)->size);
					float radius = 40.0f * ((game->player->score / 10.0f) + 1.0f);
					game->player->shape.setRadius(radius);
					game->player->shape.setOrigin(radius, radius);
					it = game->balls.erase(it);
					it = game->balls.end();
					//return;
				}
				else {
					it++;
				}
				
		}
	}


	if (game->frame % 2 == 0) {
		std::list<Agent*>::iterator enemy = game->ennemies.begin();
		while (enemy != game->ennemies.end()) {
			bool erase = false;

			std::list<Ball*>::iterator ball = game->balls.begin();
			while (ball != game->balls.end()) {
				if (CheckCollision(*ball, *enemy)) {
					UpdateScore(*enemy, (*ball)->size);
					float radius = 40.0f * (((*enemy)->score / 10.0f) + 1.0f);
					(*enemy)->shape.setRadius(radius);
					(*enemy)->shape.setOrigin(radius, radius);
					//it = game->balls.erase(it);
					erase = true;
					//return;
					//printf("ENNEMY COLISSION BALL ALERTE PREVENEZ TOUS LES KEYS\n");

					ball = game->balls.erase(ball);
				}
				if (ball != game->balls.end()) {
					ball++;
				}
			}
			enemy++;


		}



	}

	
	std::list<Ball*>::iterator it = game->balls.begin();
	while (it != game->balls.end()) {
		window->draw((*it)->shape);
		it++;
	}

	//printf("FIN UPDATE BALLS\n");
}



void UpdateGame(float deltatime, Game* game, sf::RenderWindow* window) {
	// GAME
	game->frame++;

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
	if (game->frame % 2 == 0) {
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

	}
	//printf("FIN UPDATE GAME // \n");
}

