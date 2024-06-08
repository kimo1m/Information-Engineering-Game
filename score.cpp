#include "score.h"

Score::Score() : mCurrentScore(0) {
    mFont.loadFromFile("arial.ttf");
    mText.setFont(mFont);
    mText.setCharacterSize(24);
    mText.setFillColor(sf::Color::White);
    mText.setString("Score: 0");
}

void Score::increase(int points) {
    mCurrentScore += points;
    mText.setString("Score: " + std::to_string(mCurrentScore));
}

void Score::reset() {
    mCurrentScore = 0;
    mText.setString("Score: 0");
}

void Score::draw(sf::RenderWindow& window) {
    window.draw(mText);
}
