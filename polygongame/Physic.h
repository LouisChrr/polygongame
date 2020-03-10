#pragma once

float DegToRad(float deg);
void AddForce(Agent* agent, sf::Vector2f force, float deltaTime);
sf::Vector2f ClampForce(sf::Vector2f force);
float VectorMagnitude(sf::Vector2f vector);
float Lerp(float a, float b, float t);