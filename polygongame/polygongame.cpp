#include <iostream>
#include <SFML/Graphics.hpp>
#include "Agent.h"
#include "Physic.h"
#include "map"
#include "Trail.h"
#include "Bullet.h"
#include "Game.h"

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


    Agent* player = CreatePlayer();
    Agent* enemy = CreateEnemy();
    Game* game = new Game;

    float timer = 0.0f;
    float cooldown = 0.0f;
    float trailCooldown = 0.0f;

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

        timer += deltaTime;

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

        std::list<Bullet*>::iterator it = game->bullets.begin();

        while (it != game->bullets.end()) {

            UpdatePosition(*it, deltaTime);

            if (outOfBounds(*it) || CheckDamage((*it), enemy)) {
                it = game->bullets.erase(it);
            }

            else {
                window.draw((*it)->shape);
                it++;
            }
        }

        if (trailCooldown <= 0) {
            trailCooldown = 0.01f;
            updateTrail(player);
            updateTrail(enemy);
        }

        drawTrail(player, &window);

        MoveAgent(player, deltaTime);

        UpdateEnemyRotation(player, enemy, deltaTime);
        drawTrail(enemy, &window);
        AddForce(enemy, moveDir(enemy, 1), deltaTime);
        MoveAgent(enemy, deltaTime);

        window.draw(player->shape);
        window.draw(enemy->shape);

        window.display();
    }
}