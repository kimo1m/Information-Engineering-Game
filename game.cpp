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
    // Check for collisions and update scoring
    for (auto& villain : mVillains) {
        if (mHero.getBounds().intersects(villain.getBounds())) {
            // Handle collision (e.g., reduce health, etc.)
        }
    }
    for (auto& bonus : mBonuses) {
        if (bonus.isCollected(mHero.getBounds())) {
            // Handle bonus collection
            mScore.increase(10); // Example bonus score increment
        }
    }
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
    mScore.draw(mWindow);
    mWindow.display();
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed) {
    if (key == sf::Keyboard::W) mHero.moveUp(isPressed);
    if (key == sf::Keyboard::S) mHero.moveDown(isPressed);
    if (key == sf::Keyboard::A) mHero.moveLeft(isPressed);
    if (key == sf::Keyboard::D) mHero.moveRight(isPressed);
}
