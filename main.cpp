#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <memory>
#include "GameObject.h"
#include "Hero.h"
#include "Villain.h"
#include "Obstacle.h"
#include "Score.h"
#include "AnimatedSprite.h" // Include AnimatedSprite header

// Constants for the window size
const int WINDOW_WIDTH = 360;
const int WINDOW_HEIGHT = 360;

// Movement speed for the hero
const float HERO_SPEED = 0.5f;

// Movement speed for the villains
float VILLAIN_SPEED = 0.1f;

// Time interval (in seconds) to generate new villains
float VILLAIN_GENERATION_INTERVAL = 1.0f;

// Function to generate a new villain at a random position outside the screen
std::unique_ptr<Villain> generateRandomVillain() {
    int side = rand() % 4;
    float x, y;
    switch (side) {
    case 0: // Left side
        x = -50.f;
        y = static_cast<float>(rand() % WINDOW_HEIGHT);
        break;
    case 1: // Right side
        x = WINDOW_WIDTH + 50.f;
        y = static_cast<float>(rand() % WINDOW_HEIGHT);
        break;
    case 2: // Top side
        x = static_cast<float>(rand() % WINDOW_WIDTH);
        y = -50.f;
        break;
    case 3: // Bottom side
        x = static_cast<float>(rand() % WINDOW_WIDTH);
        y = WINDOW_HEIGHT + 50.f;
        break;
    }
    return std::make_unique<Villain>(x, y);
}

// Check collision between future bounds and obstacles
bool checkCollision(const sf::FloatRect& futureBounds, const std::vector<std::unique_ptr<GameObject>>& gameObjects) {
    for (const auto& obj : gameObjects) {
        if (dynamic_cast<Obstacle*>(obj.get()) && futureBounds.intersects(obj->getGlobalBounds())) {
            return true; // Collision detected
        }
    }
    return false; // No collision detected
}

int main() {
    // Initialize SFML window, background, music, and other constants
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Hero vs Villains");

    // Load the background texture
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("background.jpg")) {
        std::cerr << "Failed to load background.jpg" << std::endl;
        return -1;
    }
    sf::Sprite backgroundSprite(backgroundTexture);

    // Load the background music
    sf::Music backgroundMusic;
    if (!backgroundMusic.openFromFile("musicrecord2.ogg")) {
        std::cerr << "Failed to load musicrecord2.ogg" << std::endl;
        return -1;
    }
    backgroundMusic.setLoop(true); // Set the music to loop
    backgroundMusic.play(); // Play the music

    // Create hero and score objects
    AnimatedSprite hero(0.2f, "character.png"); // Adjust frame duration and texture file path
    hero.add_animation_frame(sf::IntRect(0, 0, 50, 50)); // Add animation frames
    hero.add_animation_frame(sf::IntRect(50, 0, 50, 50)); // Example frames, adjust as needed
    hero.add_animation_frame(sf::IntRect(100, 0, 50, 50));
    hero.setPosition(180.f, 180.f); // Set initial position

    std::unique_ptr<Score> score = std::make_unique<Score>();

    // Create vector for game objects and add obstacles
    std::vector<std::unique_ptr<GameObject>> gameObjects;
    gameObjects.push_back(std::make_unique<Obstacle>(100.f, 100.f, 50.f, 150.f));
    gameObjects.push_back(std::make_unique<Obstacle>(200.f, 250.f, 200.f, 30.f));

    // Initialize random seed
    srand(static_cast<unsigned>(time(0)));

    // SFML clock for timing
    sf::Clock clock;
    float timeSinceLastGeneration = 0.0f;

    bool isGameOver = false; // Flag to indicate if the game is over

    // Game loop
    while (window.isOpen()) {
        // Handle SFML events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Process game logic if game is not over
        if (!isGameOver) {
            // Move the hero with arrow keys
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                sf::Vector2f direction(-HERO_SPEED, 0.f);
                if (!checkCollision(hero.getGlobalBounds(), gameObjects)) {
                    hero.move(direction);
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                sf::Vector2f direction(HERO_SPEED, 0.f);
                if (!checkCollision(hero.getGlobalBounds(), gameObjects)) {
                    hero.move(direction);
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                sf::Vector2f direction(0.f, -HERO_SPEED);
                if (!checkCollision(hero.getGlobalBounds(), gameObjects)) {
                    hero.move(direction);
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                sf::Vector2f direction(0.f, HERO_SPEED);
                if (!checkCollision(hero.getGlobalBounds(), gameObjects)) {
                    hero.move(direction);
                }
            }

            // Update the animation of the hero
            hero.update_animation(clock.restart().asSeconds());

            // Update the sword position
            hero.updateSword();

            // Move the villains towards the hero
            for (auto& obj : gameObjects) {
                if (auto villain = dynamic_cast<Villain*>(obj.get())) {
                    sf::Vector2f targetPosition = hero.getPosition();
                    sf::Vector2f direction = targetPosition - villain->shape.getPosition();
                    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                    if (length != 0) {
                        direction /= length;
                    }
                    sf::Vector2f newVillainPosition = villain->shape.getPosition() + direction * VILLAIN_SPEED;
                    if (!checkCollision(villain->shape.getGlobalBounds(), gameObjects)) {
                        villain->moveTowards(targetPosition);
                    }
                }
            }

            // Generate a new villain at intervals
            float deltaTime = clock.restart().asSeconds();
            timeSinceLastGeneration += deltaTime;
            if (timeSinceLastGeneration >= VILLAIN_GENERATION_INTERVAL) {
                gameObjects.push_back(generateRandomVillain());
                timeSinceLastGeneration = 0.0f;
            }

            // Check for collisions between sword and villains
            auto swordBounds = hero.getSwordBounds();
            for (auto it = gameObjects.begin(); it != gameObjects.end(); ) {
                if (auto villain = dynamic_cast<Villain*>(it->get())) {
                    if (swordBounds.intersects(villain->shape.getGlobalBounds())) {
                        score->increaseScore(); // Increase score when a villain is killed
                        it = gameObjects.erase(it); // Remove the villain if hit by the sword
                    }
                    else {
                        ++it;
                    }
                }
                else {
                    ++it;
                }
            }

            // Check for collisions between hero and villains
            for (const auto& obj : gameObjects) {
                if (auto villain = dynamic_cast<Villain*>(obj.get())) {
                    if (hero.getGlobalBounds().intersects(villain->shape.getGlobalBounds())) {
                        isGameOver = true;
                        break;
                    }
                }
            }

            // Increase the level based on score
            score->increase_level();
        }

        // Draw everything to the window
        window.clear();
        window.draw(backgroundSprite); // Draw the background

        // Draw hero and sword
        hero.draw(window);

        // Draw game objects (obstacles, villains)
        for (const auto& obj : gameObjects) {
            obj->draw(window); // Draw each game object
        }

        // Draw score text
        sf::Font font;
        font.loadFromFile("BebasNeue-Regular.ttf");
        sf::Text scoreText("Score: " + std::to_string(score->getScore()), font, 24);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(10.f, 10.f);
        window.draw(scoreText);

        // Draw game over text if game is over
        if (isGameOver) {
            sf::Text gameOverText("Game Over", font, 48);
            gameOverText.setFillColor(sf::Color::Red);
            gameOverText.setPosition(WINDOW_WIDTH / 2 - gameOverText.getGlobalBounds().width / 2, WINDOW_HEIGHT / 2 - gameOverText.getGlobalBounds().height / 2);
            window.draw(gameOverText);
        }

        window.display(); // Display the drawn frame
    }

    return 0;
}
