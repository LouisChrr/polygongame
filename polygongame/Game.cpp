#include "Game.h"
#include "Agent.h"
#include "Physic.h"
#include "Ball.h"
#include <cmath>
#include <iostream>
#define ennemy_nb 10

void UpdateText(Game* game, sf::RenderTexture* tex) {
	sf::Font font;
	font.loadFromFile("OldeEnglish.ttf");
	std::string score = std::to_string((int)game->player->score);
	game->scoreText = new sf::Text(score, font);

	game->scoreText->setCharacterSize(40 + game->player->shape.getRadius());

	//game->scoreText->setString(std::to_string(game->player->score));
	game->scoreText->setPosition(game->player->convexShape.getPosition().x, game->player->convexShape.getPosition().y - 240);

	tex->draw(*(game->scoreText));
}

Game* CreateGame(Agent* player) {
	Game* game = new Game;
	game->player = player;
	game->bullets = std::list<Bullet*>();
	game->ennemies = std::list<Agent*>();
	game->balls = std::list<Ball*>();
	game->stars = std::list<sf::CircleShape>();
	sf::Font font;
	font.loadFromFile("OldeEnglish.ttf");
	std::string score = std::to_string(game->player->score);
	game->scoreText = new sf::Text(score, font);

	game->scoreText->setOrigin(game->scoreText->getCharacterSize() / 2, game->scoreText->getCharacterSize() / 2);
	game->scoreText->setCharacterSize(60);
	game->scoreText->setFillColor(sf::Color::White);
	game->scoreText->setString(std::to_string(game->player->score));
	game->scoreText->setPosition(game->player->shape.getPosition().x, game->player->shape.getPosition().y - 120);

	for (int j = 0; j <400 ; j++) {

		sf::CircleShape star(rand() % 5);
		star.setPosition(rand() % 8000, rand() % 8000);
		game->stars.push_back(star);

	}

	for (int i = 0; i < ennemy_nb; i++) {
		Agent* ennemy = CreateEnemy();
		game->ennemies.push_back(ennemy);
		//printf("Ennemy cree nb: %d\n", i);
	}

	return game;
}

void LerpPosition(sf::RenderTexture* tex, float deltaTime, Game* game) {

	sf::View myView;

	myView = tex->getView();
	myView.setSize(sf::Vector2f(1920.0f, 1080.0f));

	sf::Vector2f pos = myView.getCenter();

	float x = Lerp(pos.x, game->targetPos.x, deltaTime * 5);
	float y = Lerp(pos.y, game->targetPos.y, deltaTime * 5);

	myView.setCenter(x,y);

	myView.zoom(game->targetZoom);
	tex->setView(myView);

}

void UpdateScore(Agent* agent, int add) {
	agent->score += add;

	if (agent->type == PLAYER) {
		agent->convexShape.setPoint(0, sf::Vector2f(0.0f, 0.0f));
		agent->convexShape.setPoint(1, sf::Vector2f(-40.0f * ((agent->score / 30.0f) + 1.0f), 160.0f * ((agent->score / 30.0f) + 1.0f)));
		agent->convexShape.setPoint(2, sf::Vector2f(40.0f * ((agent->score / 30.0f) + 1.0f), 160.0f * ((agent->score / 30.0f) + 1.0f)));
		agent->convexShape.setOrigin(0.0f, agent->convexShape.getPoint(1).y / 2);
		agent->shape.setRadius(40.0f * ((agent->score / 30.0f) + 1.0f));
		agent->shape.setOrigin(agent->shape.getRadius(), agent->shape.getRadius());
	}
	else {
		agent->shape.setRadius(40.0f * ((agent->score / 30.0f) + 1.0f));
		agent->shape.setOrigin(agent->shape.getRadius(), agent->shape.getRadius());
	}

	



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

		if (agent != *enemy && VectorMagnitude((*enemy)->shape.getPosition() - agent->shape.getPosition()) <= (*enemy)->shape.getRadius() + agent->shape.getRadius()) {

			if (agent->type == PLAYER) {

				Respawn(agent, game);

			}

			else {

				if ((*enemy)->score > agent->score) {
					Respawn(agent, game);
				}
				else if ((*enemy)->score < agent->score) {
					Respawn((*enemy), game);
				}

			}
		}
		enemy++;
	}

}

void UpdateBalls(sf::RenderTexture* tex, Game* game, float deltaTime) {
	//printf("ON UPDATE LES BALLS\n");
	//std::list<Ball*>::iterator it = game->balls.begin();

	if (game->frame % 1 == 0) {
		//printf("Update player - balls %d\n", game->frame);
		std::list<Ball*>::iterator it = game->balls.begin();
		while (it != game->balls.end()) {
				
				if (CheckCollision(*it, game->player)) {
					UpdateScore(game->player, (*it)->size);
					//float radius = 40.0f * ((game->player->score / 30.0f) + 1.0f);
					//game->player->shape.setRadius(radius);
					//game->player->shape.setOrigin(radius, radius);
					game->player->convexShape.setOrigin(0.0f, game->player->convexShape.getPoint(1).y / 2);
					game->player->shape.setRadius(40.0f * ((game->player->score / 30.0f) + 1.0f));
					game->player->shape.setOrigin(game->player->shape.getRadius(), game->player->shape.getRadius());
					it = game->balls.erase(it);
					it = game->balls.end();
					//return;
				}
				else {
					it++;
				}
				
		}
	}


	if (game->frame % 1 == 0) {
		std::list<Agent*>::iterator enemy = game->ennemies.begin();
		while (enemy != game->ennemies.end()) {
			bool erase = false;

			std::list<Ball*>::iterator ball = game->balls.begin();
			while (ball != game->balls.end()) {
				if (CheckCollision(*ball, *enemy) && (*enemy)->score <= 100) {
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
		tex->draw((*it)->shape);
		it++;
	}

	//printf("FIN UPDATE BALLS\n");
}



void UpdateGame(float deltatime, Game* game, sf::RenderTexture* tex) {
	// GAME

	sf::RectangleShape fond(sf::Vector2f(8000,8000));
	fond.setFillColor(sf::Color::Black);
	fond.setPosition(0, 0);
	tex->draw(fond);

	std::list<sf::CircleShape>::iterator ite = game->stars.begin();
	while (ite != game->stars.end()) {
		tex->draw(*ite);
		ite++;
	}

	game->frame++;

	// BALLS
	UpdateBalls(tex, game, deltatime);


	// PLAYER
	drawTrail(game->player, tex, deltatime);
	MoveAgent(game->player, deltatime);


	//game->player->lEye = sf::CircleShape(game->player->shape.getRadius() / 3);
	//game->player->lEye.setFillColor(sf::Color::White);
	//game->player->lEye.setPosition(sf::Vector2f(game->player->shape.getPosition().x - game->player->shape.getRadius() / 4, game->player->shape.getPosition().y - game->player->shape.getRadius() / 3));

	//game->player->lEye.setPosition(50.0f * (sf::Vector2f(cos(DegToRad(game->player->shape.getRotation())), sin(DegToRad(game->player->shape.getRotation())))) + sf::Vector2f(game->player->shape.getPosition().x, game->player->shape.getPosition().y) + sf::Vector2f(0,game->player->shape.getRadius()) );
	//game->player->lEye.setOrigin(sf::Vector2f(game->player->lEye.getRadius(), game->player->lEye.getRadius()));
	//game->player->lEye.setOrigin(game->player->shape.getOrigin().x + game->player->shape.getRadius(), game->player->shape.getOrigin().y + game->player->shape.getRadius());
	//game->player->lEye.setPosition(game->player->shape.getPosition() - game->player->lEye.getOrigin());

	//game->player->lEye.setOutlineThickness(1);
	//game->player->lEye.setOutlineColor(sf::Color::Black);

	//game->player->rEye = sf::CircleShape(game->player->shape.getRadius() / 3);
	//game->player->rEye.setFillColor(sf::Color::White);
	//game->player->rEye.setPosition(sf::Vector2f(game->player->shape.getPosition().x + game->player->shape.getRadius() / 4, game->player->shape.getPosition().y - game->player->shape.getRadius() / 3));

	//game->player->rEye.setPosition(50.0f*(sf::Vector2f(cos(DegToRad(game->player->shape.getRotation())), sin(DegToRad(game->player->shape.getRotation())))) + sf::Vector2f(game->player->shape.getPosition().x  + game->player->shape.getRadius() / 4 , game->player->shape.getPosition().y ) + sf::Vector2f(0,0) );
	//game->player->rEye.setOrigin(sf::Vector2f(game->player->rEye.getRadius(), game->player->rEye.getRadius()));

	//game->player->rEye.setOrigin(game->player->shape.getOrigin().x + game->player->shape.getRadius(), game->player->shape.getOrigin().y + game->player->shape.getRadius());
	//game->player->rEye.setPosition(game->player->shape.getPosition() - game->player->rEye.getOrigin());

	//game->player->rEye.setOutlineThickness(1);
	//game->player->rEye.setOutlineColor(sf::Color::Black);
	tex->draw(game->player->convexShape);
	//window->draw(game->player->shape);
	//window->draw(game->player->lEye);
	//window->draw(game->player->rEye);

	//UI
	UpdateText(game, tex);


	//printf("UPDATE PLAYER FINI // \n");
	// ENNEMIES
	std::list<Agent*>::iterator it = game->ennemies.begin();
	while (it != game->ennemies.end()) {
		Agent* enemy = *it;
		UpdateEnemyRotation(game->player, enemy, deltatime);
		drawTrail(enemy, tex, deltatime);
		AddForce(enemy, moveDir(enemy, 1), deltatime);
		MoveAgent(enemy, deltatime);
		tex->draw(enemy->shape);
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
					tex->draw((*bullet)->shape);
				}
				else {
					(*enemy)->health -= (*bullet)->damage;

					if ((*enemy)->health <= 0) {
						(*enemy)->health = 100;
						Respawn((*enemy), game);
					}

					erase = true;
				}
				enemy++;
			}
			bullet++;
		}
	}

	//TRAILDAMAGE
	if (game->frame % 1 == 0) {
		std::list<Agent*>::iterator enemy = game->ennemies.begin();

		CheckHeadDamage(game->player, game);

		while (enemy != game->ennemies.end()) {

			CheckHeadDamage(*enemy, game);

			std::list<Agent*>::iterator enemy1 = game->ennemies.begin();
			while (enemy1 != game->ennemies.end()) {
				if (CheckTrailDamage((*enemy), (*enemy1)) && enemy != enemy1) {
					Respawn(*enemy, game);
				}
				enemy1++;
			}

			if (CheckTrailDamage((*enemy), game->player)) {
				Respawn(*enemy, game);
			}

			if (CheckTrailDamage(game->player, (*enemy))) {
				Respawn(game->player, game);
			}

			enemy++;
		}

	}
	//printf("FIN UPDATE GAME // \n");
}

