#pragma once
#include <SFML/Graphics.hpp>

class Bonus : public sf::Drawable {
public:
    Bonus();
    void spawn(float x, float y);
    void update(sf::Time deltaTime);
    bool isCollected(sf::FloatRect bounds) const;

protected:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    sf::Sprite mSprite;
    sf::Texture mTexture;
    bool mCollected;
};
