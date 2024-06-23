#include "Hero.h"
#include <iostream>

Hero::Hero() {
    // Check if the texture is loaded correctly
    if (!texture.loadFromFile("hero6.png")) {
        // Handle error (print message or log)
        std::cerr << "Failed to load hero6.png" << std::endl;
    }

    // Initialize the hero sprite
    sprite.setTexture(texture);
    sprite.setScale(0.5f, 0.5f); // Adjust the scale as needed
    sprite.setPosition(360 / 2, 360 / 2);

    // Load the sword texture
    if (!swordTexture.loadFromFile("sword.png")) {
        // Handle error
        std::cerr << "Error loading sword.png" << std::endl;
    }

    // Initialize the sword sprite
    sword.setTexture(swordTexture);
    sword.setScale(0.03f, 0.03f); // Adjust the scale as needed
    updateSword();
}

void Hero::updateSword() {
    // Position the sword relative to the hero
    sword.setPosition(sprite.getPosition().x + sprite.getGlobalBounds().width, sprite.getPosition().y);
}

void Hero::move(sf::Vector2f direction) {
    // Move the hero with boundary checks
    sf::Vector2f newPosition = sprite.getPosition() + direction;

    if (newPosition.x >= 0 && newPosition.x + sprite.getGlobalBounds().width <= 360 &&
        newPosition.y >= 0 && newPosition.y + sprite.getGlobalBounds().height <= 360) {
        sprite.setPosition(newPosition);
        updateSword();
    }
}

sf::FloatRect Hero::getFutureBounds(sf::Vector2f direction) const {
    sf::FloatRect futureBounds = sprite.getGlobalBounds();
    futureBounds.left += direction.x;
    futureBounds.top += direction.y;
    return futureBounds;
}

void Hero::draw(sf::RenderWindow& window) {
    window.draw(sprite);
    window.draw(sword);
}

void Hero::update(float deltaTime) {
    // No update needed for Hero in this context
}

sf::FloatRect Hero::getGlobalBounds() const {
    return sprite.getGlobalBounds();
}
