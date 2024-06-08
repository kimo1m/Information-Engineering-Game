#pragma once
#include <SFML/Graphics.hpp>
#include "score.h"
#include "difficulty.h"
#include "character.h"
#include "obstacles.h"
#include "bonus.h"

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update(sf::Time deltaTime);
    void render();

    void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);

    sf::RenderWindow mWindow;
    Score mScore;
    Difficulty mDifficulty;
    Hero mHero;
    std::vector<Villain> mVillains;
    Obstacles mObstacles;
    std::vector<Bonus> mBonuses;
};
