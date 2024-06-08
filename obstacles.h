#pragma once
#include <SFML/Graphics.hpp>

class Obstacles : public sf::Drawable {
public:
    Obstacles();
    void addObstacle(sf::FloatRect obstacle);
    bool checkCollision(sf::FloatRect bounds) const;

protected:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    std::vector<sf::RectangleShape> mObstacles;
};
