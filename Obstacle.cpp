#include "Obstacle.h"

Obstacle::Obstacle(float x, float y, float width, float height) {
    // Initialize the obstacle
    shape.setSize(sf::Vector2f(width, height));
    shape.setFillColor(sf::Color::Magenta);
    shape.setPosition(x, y);
}

void Obstacle::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

void Obstacle::update(float deltaTime) {
    // No update needed for Obstacle in this context
}

sf::FloatRect Obstacle::getGlobalBounds() const {
    return shape.getGlobalBounds();
}
