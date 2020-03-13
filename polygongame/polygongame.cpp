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
    sf::Shader shader;

    if (!sf::Shader::isAvailable())
    {
        printf("Shader indisponible");
    }

    if (!shader.loadFromFile("glow.frag", sf::Shader::Fragment))
    {
        printf("Impossible de charger le shader");
    }

    std::map<sf::Keyboard::Key, bool> Keys = {

    { sf::Keyboard::Key::Left, false },
    { sf::Keyboard::Key::Right, false },
    { sf::Keyboard::Key::Up, false },
    { sf::Keyboard::Key::Down, false },
    { sf::Keyboard::Key::Space, false },
    { sf::Keyboard::Key::R, false },

    { sf::Keyboard::Key::Q, false },
    { sf::Keyboard::Key::D, false },
    { sf::Keyboard::Key::Z, false },
    { sf::Keyboard::Key::S, false },
    { sf::Keyboard::Key::Space, false }

    };

    sf::Clock clock;
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "SpaceX");

    sf::RenderTexture tex;
    tex.create(1920, 1080);

    Agent* player = CreatePlayer();
    Game* game = CreateGame(player);

    
    float timer = 0.0f;
    float cooldown = 0.0f;
    float trailCooldown = 0.0f;
    float ballSpawnCooldown = 0.0f;

    while (window.isOpen())
    {
        tex.clear(sf::Color(0,0,50));

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

        float deltaTime = clock.restart().asSeconds();

        if (Keys[sf::Keyboard::Key::Left] || Keys[sf::Keyboard::Key::Q]) {
            Rotate(player, -1, deltaTime);
        }

        if (Keys[sf::Keyboard::Key::Right] || Keys[sf::Keyboard::Key::D]) {
            Rotate(player, 1, deltaTime);
        }

        if (Keys[sf::Keyboard::Key::Up] || Keys[sf::Keyboard::Key::Z]) {
            AddForce(player, moveDir(player, 1), deltaTime);
        }

        if (Keys[sf::Keyboard::Key::Down] || Keys[sf::Keyboard::Key::S]) {
            AddForce(player, moveDir(player, -1), deltaTime);
        }

        if (Keys[sf::Keyboard::Key::Space]) {
            if (cooldown <= 0 && game->player->score > 0) {
                cooldown = 0.15f;
                InstantiateBullet(player, game);
            }
        }

        if (Keys[sf::Keyboard::Key::R]) {
            timer = 0.0f;
            cooldown = 0.0f;
            trailCooldown = 0.0f;
            ballSpawnCooldown = 0.0f;

            delete player;
            delete game;

            player = CreatePlayer();
            game = CreateGame(player);
        }

        timer += deltaTime;
        cooldown -= deltaTime;
        trailCooldown -= deltaTime;
        ballSpawnCooldown -= deltaTime;

        Twinkle(game, timer);

        if (trailCooldown <= 0) {
            trailCooldown = 0.02f;
            UpdateTrails(game);
        }

        if ((ballSpawnCooldown <= 0 && game->balls.size() < game->maxBallCount) || game->balls.size() < 20) {
            ballSpawnCooldown = 5.0f;
            int random = rand() % 101;
            int size = 1;
            if (random < 60) {
                size = 1;
            }
            else if (random >= 60 && random < 85) {
                size = 2;
            }
            else if(random < 95 && random >= 85){
                size = 3;
            }
            else  if(random <= 99){
                size = 5;
            }else{
                size = 15;
            }

            CreateBall(game, size);
        }

        game->targetPos = player->convexShape.getPosition();
        game->targetZoom = 2.0f + (game->player->score/40.0f);

        LerpPosition(&tex, deltaTime, game);

        UpdateGame(deltaTime, game, &tex);

        tex.display();

        window.clear();

        //shader.setParameter("texture", tex.getTexture());
        //shader.setUniform("texOffset", sf::Vector2f(1.0f/(float)tex.getSize().x, 1.0f / (float)tex.getSize().y));

        sf::Sprite sprite;
        sprite.setTexture(tex.getTexture());
        window.draw(sprite);
        window.display();
        
    }
}