#include "bonus.h"

Bonus::Bonus() : mCollected(false) {
    // Load texture and set sprite
    mTexture.loadFromFile("bonus.png"); // Ensure you have the texture file
    mSprite.setTexture(mTexture);
}

void Bonus::spawn(float x, float y) {
    mSprite.setPosition(x, y);
    mCollected = false;
}

void Bonus::update(sf::Time deltaTime) {
    // Update bonus state (e.g., time before disappearing)
}

bool Bonus::isCollected(sf::FloatRect bounds) const {
    return !mCollected && mSprite.getGlobalBounds().intersects(bounds);
}

void Bonus::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (!mCollected) {
        target.draw(mSprite, states);
    }
}
