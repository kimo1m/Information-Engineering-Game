#include "Villain.h"
#include <cmath>

Villain::Villain(float x, float y) {
    // Initialize the villain shape
    shape.setRadius(15.f); // Adjust the radius as needed
    shape.setFillColor(sf::Color(255, 0, 0)); // Dark red color
    shape.setPosition(x, y);
}

void Villain::moveTowards(sf::Vector2f targetPosition) {
    // Calculate direction vector towards the target
    sf::Vector2f direction = targetPosition - shape.getPosition();
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0) {
        direction /= length; // Normalize the direction
    }

    // Move the villain towards the target
    shape.move(direction * 0.1f); // Use VILLAIN_SPEED if accessible
}

void Villain::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

void Villain::update(float deltaTime) {
    // Movement will be handled in the main loop
}

sf::FloatRect Villain::getGlobalBounds() const {
    return shape.getGlobalBounds();
}
