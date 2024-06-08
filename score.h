#pragma once
#include <SFML/Graphics.hpp>

class Score {
public:
    Score();
    void increase(int points);
    void reset();
    void draw(sf::RenderWindow& window);

private:
    int mCurrentScore;
    sf::Font mFont;
    sf::Text mText;
};
