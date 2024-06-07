#include <SFML/Graphics.hpp>
#include "game.h"

int main() {
    Game game;
    game.run();
    return 0;
}

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

#include "game.h"

Game::Game() : mWindow(sf::VideoMode(800, 600), "2D Game"), mHero(), mScore(), mDifficulty() {
    // Initialize game state here
}

void Game::run() {
    sf::Clock clock;
    while (mWindow.isOpen()) {
        sf::Time deltaTime = clock.restart();
        processEvents();
        update(deltaTime);
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (mWindow.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::KeyPressed:
                handlePlayerInput(event.key.code, true);
                break;
            case sf::Event::KeyReleased:
                handlePlayerInput(event.key.code, false);
                break;
            case sf::Event::Closed:
                mWindow.close();
                break;
        }
    }
}

void Game::update(sf::Time deltaTime) {
    // Update game objects
    mHero.update(deltaTime);
    for (auto& villain : mVillains) {
        villain.update(deltaTime);
    }
    // Handle collisions and scoring
}

void Game::render() {
    mWindow.clear();
    mWindow.draw(mHero);
    for (const auto& villain : mVillains) {
        mWindow.draw(villain);
    }
    mWindow.draw(mObstacles);
    for (const auto& bonus : mBonuses) {
        mWindow.draw(bonus);
    }
    mWindow.display();
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed) {
    // Handle input
    if (key == sf::Keyboard::W) mHero.moveUp(isPressed);
    if (key == sf::Keyboard::S) mHero.moveDown(isPressed);
    if (key == sf::Keyboard::A) mHero.moveLeft(isPressed);
    if (key == sf::Keyboard::D) mHero.moveRight(isPressed);
}

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

#pragma once

class Difficulty {
public:
    Difficulty();
    void increaseDifficulty(int score);

private:
    int mCurrentLevel;
};

#include "difficulty.h"

Difficulty::Difficulty() : mCurrentLevel(1) {}

void Difficulty::increaseDifficulty(int score) {
    mCurrentLevel = score / 100 + 1;
    // Adjust game parameters based on mCurrentLevel
}

#pragma once
#include <SFML/Graphics.hpp>

class Character : public sf::Drawable {
public:
    Character();
    void setPosition(float x, float y);
    void move(float offsetX, float offsetY);
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

void Character::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(mSprite, states);
}

Hero::Hero() {
    // Initialize hero-specific attributes
    mDirection = sf::Vector2f(0.f, 0.f);
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
}

void Villain::update(sf::Time deltaTime) {
    // Update villain movement and behavior
}

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

#include "bonus.h"

Bonus::Bonus() : mCollected(false) {
    // Load texture and set sprite
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
