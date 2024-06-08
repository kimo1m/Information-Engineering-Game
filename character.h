#pragma once
#include <SFML/Graphics.hpp>

class Character : public sf::Drawable {
public:
    Character();
    void setPosition(float x, float y);
    void move(float offsetX, float offsetY);
    sf::FloatRect getBounds() const;
    virtual void update(sf::Time deltaTime) = 0;

protected:
    sf::Sprite mSprite;
    sf::Texture mTexture;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

class Hero : public Character {
public:
    Hero();
    void moveUp(bool isPressed);
    void moveDown(bool isPressed);
    void moveLeft(bool isPressed);
    void moveRight(bool isPressed);
    void update(sf::Time deltaTime) override;

private:
    sf::Vector2f mDirection;
};

class Villain : public Character {
public:
    Villain();
    void update(sf::Time deltaTime) override;
};
