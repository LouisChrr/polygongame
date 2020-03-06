#include <iostream>
#include <SFML/Graphics.hpp>
#include "Agent.h"
#include "Physic.h"
#include "map"
#include "Trail.h"
#include "Bullet.h"
#include "Game.h"
#include "Ball.h"

void UpdateKeyState(bool isActive, sf::Event* event, std::map<sf::Keyboard::Key, bool>* keys) {
    std::map<sf::Keyboard::Key, bool>::iterator iterator;

    for (iterator = keys->begin(); iterator != keys->end(); iterator++)
    {
        if (event->key.code == iterator->first)
            iterator->second = isActive;
    }
}

int main()
{
    std::map<sf::Keyboard::Key, bool> Keys = {

    { sf::Keyboard::Key::Left, false },
    { sf::Keyboard::Key::Right, false },
    { sf::Keyboard::Key::Up, false },
    { sf::Keyboard::Key::Down, false },
    { sf::Keyboard::Key::Space, false }

    };

    sf::Clock clock;
    sf::RenderWindow window(sf::VideoMode(1600, 900), "SpaceX");
    sf::View myView;

    Agent* player = CreatePlayer();
    Game* game = CreateGame(player);

    //float timer = 0.0f;
    float cooldown = 0.0f;
    float trailCooldown = 0.0f;
    float ballSpawnCooldown = 0.0f;

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed) {
                UpdateKeyState(true, &event, &Keys);
            }

            else if (event.type == sf::Event::KeyReleased) {
                UpdateKeyState(false, &event, &Keys);
            }

        }

        window.clear();

        float deltaTime = clock.restart().asSeconds();
        //float deltaTime = 0.002f;
        //printf("deltatime: %f\n", deltaTime);
       // deltaTime /= 10;
        //timer += deltaTime;

        if (Keys[sf::Keyboard::Key::Left]) {
            Rotate(player, -1, deltaTime);
        }

        if (Keys[sf::Keyboard::Key::Right]) {
            Rotate(player, 1, deltaTime);
        }

        if (Keys[sf::Keyboard::Key::Up]) {
            AddForce(player, moveDir(player, 1), deltaTime);
        }

        if (Keys[sf::Keyboard::Key::Down]) {
            AddForce(player, moveDir(player, -1), deltaTime);
        }

        if (Keys[sf::Keyboard::Key::Space]) {
            if (cooldown <= 0) {
                cooldown = 0.15f;
                InstantiateBullet(player, game);
            }
        }

        cooldown -= deltaTime;
        trailCooldown -= deltaTime;
        ballSpawnCooldown -= deltaTime;

        if (trailCooldown <= 0) {
            trailCooldown = 0.02f;
            UpdateTrails(game);
        }

        if ((ballSpawnCooldown <= 0 && game->balls.size() < game->maxBallCount) || game->balls.size() < 400) {
            ballSpawnCooldown = 3.0f;
            int random = rand() % 101;
            int size = 1;
            if (random < 50) {
                size = 1;
            }
            else if (random >= 50 && random < 85) {
                size = 2;
            }
            else if(random < 95 && random >= 85){
                size = 3;
            }
            else {
                size = 5;
            }

            CreateBall(game, size);
        }

        myView.setCenter(player->shape.getPosition());
        myView.setSize(sf::Vector2f(1600.0f, 900.0f));
        myView.zoom(1.0f + (game->player->score/40.0f));
        window.setView(myView);
        
        //drawTrail(player, &window);
        //printf("deltatime: %f\n", deltaTime);
        //MoveAgent(player, deltaTime);

        UpdateGame(deltaTime, game, &window);

        //window.draw(player->shape);
        //window.draw(enemy->shape);

        window.display();
    }
}