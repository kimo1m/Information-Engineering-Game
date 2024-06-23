#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"

// Villain class definition
class Villain : public GameObject {
public:
    sf::CircleShape shape;

    Villain(float x, float y);
    void moveTowards(sf::Vector2f targetPosition);
    void draw(sf::RenderWindow& window) override;
    void update(float deltaTime) override;
    sf::FloatRect getGlobalBounds() const override;
};
