#include "Game.h"
#include "Agent.h"
#include "Physic.h"
#define ennemy_nb 20

Game* CreateGame(Agent* player) {
	Game* game = new Game;
	game->player = player;
	game->bullets = std::list<Bullet*>();
	game->ennemies = std::list<Agent*>();

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

void UpdateGame(float deltatime, Game* game, sf::RenderWindow* window) {
	// PLAYER
	drawTrail(game->player, window);
	MoveAgent(game->player, deltatime);
	window->draw(game->player->shape);

	//printf("UPDATE PLAYER FINI // \n");
	// ENNEMIES
	std::list<Agent*>::iterator it = game->ennemies.begin();
	while (it != game->ennemies.end()) {
		Agent* enemy = *it;
		UpdateEnemyRotation(game->player, enemy, deltatime);
		drawTrail(enemy, window);
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
						Teleport((*enemy));
					}

					erase = true;
				}
				enemy++;
			}
			bullet++;
		}
	}

	//printf("UPDATE BULLETS FINI // \n");
	//printf("FIN UPDATE GAME // \n");
}