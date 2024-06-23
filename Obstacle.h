#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"

// Obstacle class definition
class Obstacle : public GameObject {
public:
    sf::RectangleShape shape;

    Obstacle(float x, float y, float width, float height);
    void draw(sf::RenderWindow& window) override;
    void update(float deltaTime) override;
    sf::FloatRect getGlobalBounds() const override;
};
