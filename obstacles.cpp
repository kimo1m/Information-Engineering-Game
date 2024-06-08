#include "obstacles.h"

Obstacles::Obstacles() {
    // Initialize obstacles
}

void Obstacles::addObstacle(sf::FloatRect obstacle) {
    sf::RectangleShape rect(sf::Vector2f(obstacle.width, obstacle.height));
    rect.setPosition(obstacle.left, obstacle.top);
    mObstacles.push_back(rect);
}

bool Obstacles::checkCollision(sf::FloatRect bounds) const {
    for (const auto& obstacle : mObstacles) {
        if (obstacle.getGlobalBounds().intersects(bounds)) {
            return true;
        }
    }
    return false;
}

void Obstacles::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (const auto& obstacle : mObstacles) {
        target.draw(obstacle, states);
    }
}
    