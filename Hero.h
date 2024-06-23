#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"

// Hero class definition
class Hero : public GameObject {
public:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Sprite sword;
    sf::Texture swordTexture;

    Hero();
    void updateSword();
    void move(sf::Vector2f direction);
    sf::FloatRect getFutureBounds(sf::Vector2f direction) const;
    void draw(sf::RenderWindow& window) override;
    void update(float deltaTime) override;
    sf::FloatRect getGlobalBounds() const override;
};
#pragma once
