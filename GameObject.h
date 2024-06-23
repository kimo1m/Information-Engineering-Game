#pragma once
#include <SFML/Graphics.hpp>

// Base class for all game objects
class GameObject {
public:
    virtual ~GameObject() = default;
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual void update(float deltaTime) = 0;
    virtual sf::FloatRect getGlobalBounds() const = 0;
};
