#include "character.h"

Character::Character() {
    // Load texture and set sprite
}

void Character::setPosition(float x, float y) {
    mSprite.setPosition(x, y);
}

void Character::move(float offsetX, float offsetY) {
    mSprite.move(offsetX, offsetY);
}

sf::FloatRect Character::getBounds() const {
    return mSprite.getGlobalBounds();
}

void Character::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(mSprite, states);
}

Hero::Hero() {
    // Initialize hero-specific attributes
    mDirection = sf::Vector2f(0.f, 0.f);
    mTexture.loadFromFile("hero.png"); // Ensure you have the texture file
    mSprite.setTexture(mTexture);
}

void Hero::moveUp(bool isPressed) {
    mDirection.y = isPressed ? -1.f : 0.f;
}

void Hero::moveDown(bool isPressed) {
    mDirection.y = isPressed ? 1.f : 0.f;
}

void Hero::moveLeft(bool isPressed) {
    mDirection.x = isPressed ? -1.f : 0.f;
}

void Hero::moveRight(bool isPressed) {
    mDirection.x = isPressed ? 1.f : 0.f;
}

void Hero::update(sf::Time deltaTime) {
    move(mDirection.x * deltaTime.asSeconds() * 100.f, mDirection.y * deltaTime.asSeconds() * 100.f);
}

Villain::Villain() {
    // Initialize villain-specific attributes
    mTexture.loadFromFile("villain.png"); // Ensure you have the texture file
    mSprite.setTexture(mTexture);
}

void Villain::update(sf::Time deltaTime) {
    // Update villain movement and behavior
}
