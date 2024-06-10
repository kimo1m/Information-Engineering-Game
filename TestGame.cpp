#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>

// Constants for the window size
const int WINDOW_WIDTH = 360;
const int WINDOW_HEIGHT = 360;

// Movement speed for the hero
const float HERO_SPEED = 0.5f;

// Movement speed for the villains
float VILLAIN_SPEED = 0.1f;

// Time interval (in seconds) to generate new villains
float VILLAIN_GENERATION_INTERVAL = 1.0f;

// Hero class definition
class Hero {
public:
    sf::Sprite sprite;
    sf::Texture texture;

    sf::Sprite sword;
    sf::Texture swordTexture;

    Hero() {
        // Load the hero texture
        texture.loadFromFile("hero6.png");

        // Initialize the hero sprite
        sprite.setTexture(texture);
        sprite.setScale(0.5f, 0.5f); // Adjust the scale as needed
        sprite.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

        // Load the sword texture
        if (!swordTexture.loadFromFile("sword.png")) {
            // Handle error
        }

        // Initialize the sword sprite
        sword.setTexture(swordTexture);
        sword.setScale(0.03f, 0.03f); // Adjust the scale as needed
        updateSword();
    }

    void updateSword() {
        // Position the sword relative to the hero
        sword.setPosition(sprite.getPosition().x + sprite.getGlobalBounds().width, sprite.getPosition().y);
    }

    void move(sf::Vector2f direction) {
        // Move the hero with boundary checks
        sf::Vector2f newPosition = sprite.getPosition() + direction;

        if (newPosition.x >= 0 && newPosition.x + sprite.getGlobalBounds().width <= WINDOW_WIDTH &&
            newPosition.y >= 0 && newPosition.y + sprite.getGlobalBounds().height <= WINDOW_HEIGHT) {
            sprite.setPosition(newPosition);
            updateSword();
        }
    }

    sf::FloatRect getFutureBounds(sf::Vector2f direction) const {
        sf::FloatRect futureBounds = sprite.getGlobalBounds();
        futureBounds.left += direction.x;
        futureBounds.top += direction.y;
        return futureBounds;
    }
};

// Villain class definition
class Villain {
public:
    sf::CircleShape shape;

    Villain(float x, float y) {
        // Initialize the villain shape
        shape.setRadius(15.f); // Adjust the radius as needed
        shape.setFillColor(sf::Color(255, 0, 0)); // Dark red color
        shape.setPosition(x, y);
    }

    void moveTowards(sf::Vector2f targetPosition) {
        // Calculate direction vector towards the target
        sf::Vector2f direction = targetPosition - shape.getPosition();
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length != 0) {
            direction /= length; // Normalize the direction
        }

        // Move the villain towards the target
        shape.move(direction * VILLAIN_SPEED);
    }

    sf::FloatRect getGlobalBounds() const {
        return shape.getGlobalBounds();
    }
};

// Score of how many villains killed
class Score {
private:
    int score;

public:
    Score() : score(0) {}

    void increaseScore() {
        score++;
    }

    int getScore() const {
        return score;
    }

    void increase_level() {
        if (score >= 15) {
            VILLAIN_GENERATION_INTERVAL = 0.70f;
            VILLAIN_SPEED = 0.3f;
        }
        if (score > 30) {
            VILLAIN_GENERATION_INTERVAL = 0.50f;
            VILLAIN_SPEED = 0.5f;
        }
    }
};

// Function to generate a new villain at a random position outside the screen
Villain generateRandomVillain() {
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
    return Villain(x, y);
}

// Obstacle class definition
class Obstacle {
public:
    sf::RectangleShape shape;

    Obstacle(float x, float y, float width, float height) {
        // Initialize the obstacle
        shape.setSize(sf::Vector2f(width, height));
        shape.setFillColor(sf::Color::Magenta);
        shape.setPosition(x, y);
    }

    sf::FloatRect getGlobalBounds() const {
        return shape.getGlobalBounds();
    }
};

bool checkCollision(const sf::FloatRect& futureBounds, const std::vector<Obstacle>& obstacles) {
    for (const auto& obstacle : obstacles) {
        if (futureBounds.intersects(obstacle.getGlobalBounds())) {
            return true; // Collision detected
        }
    }
    return false; // No collision detected
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Hero vs Villains");

    // Load the background texture
    sf::Texture backgroundTexture;
    backgroundTexture.loadFromFile("background.jpg");
    sf::Sprite backgroundSprite(backgroundTexture);

    // Load the background music
    sf::Music backgroundMusic;
    backgroundMusic.openFromFile("musicrecord2.ogg");
    
    backgroundMusic.setLoop(true); // Set the music to loop
    backgroundMusic.play(); // Play the music

    Hero hero;
    Score score;

    std::vector<Villain> villains;
    std::vector<Obstacle> obstacles; // Vector to hold obstacles

    // Create some obstacles
    obstacles.push_back(Obstacle(100.f, 100.f, 50.f, 150.f));
    obstacles.push_back(Obstacle(200.f, 250.f, 200.f, 30.f));

    // Initialize the random seed
    srand(static_cast<unsigned>(time(0)));

    sf::Clock clock;
    float timeSinceLastGeneration = 0.0f;

    bool isGameOver = false; // Flag to indicate if the game is over

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        if (!isGameOver) {
            // Move the hero with arrow keys
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                sf::Vector2f direction(-HERO_SPEED, 0.f);
                if (!checkCollision(hero.getFutureBounds(direction), obstacles)) {
                    hero.move(direction);
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                sf::Vector2f direction(HERO_SPEED, 0.f);
                if (!checkCollision(hero.getFutureBounds(direction), obstacles)) {
                    hero.move(direction);
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                sf::Vector2f direction(0.f, -HERO_SPEED);
                if (!checkCollision(hero.getFutureBounds(direction), obstacles)) {
                    hero.move(direction);
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                sf::Vector2f direction(0.f, HERO_SPEED);
                if (!checkCollision(hero.getFutureBounds(direction), obstacles)) {
                    hero.move(direction);
                }
            }

            // Move the villains towards the hero
            for (auto& villain : villains) {
                sf::Vector2f targetPosition = hero.sprite.getPosition();
                sf::Vector2f direction = targetPosition - villain.shape.getPosition();
                float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                if (length != 0) {
                    direction /= length;
                }
                sf::Vector2f newVillainPosition = villain.shape.getPosition() + direction * VILLAIN_SPEED;
                if (!checkCollision(villain.shape.getGlobalBounds(), obstacles)) {
                    villain.moveTowards(targetPosition);
                }
            }

            // Update the sword position
            hero.updateSword();

            // Generate a new villain at intervals
            float deltaTime = clock.restart().asSeconds();
            timeSinceLastGeneration += deltaTime;
            if (timeSinceLastGeneration >= VILLAIN_GENERATION_INTERVAL) {
                villains.push_back(generateRandomVillain());
                timeSinceLastGeneration = 0.0f;
            }

            // Check for collisions between sword and villains
            for (auto it = villains.begin(); it != villains.end(); ) {
                if (hero.sword.getGlobalBounds().intersects(it->shape.getGlobalBounds())) {
                    score.increaseScore(); // Increase score when a villain is killed
                    it = villains.erase(it); // Remove the villain if hit by the sword
                }
                else {
                    ++it;
                }
            }

            // Check for collisions between hero and villains
            for (const auto& villain : villains) {
                if (hero.sprite.getGlobalBounds().intersects(villain.shape.getGlobalBounds())) {
                    isGameOver = true;
                    break;
                }
            }

            // Increase the level based on score
            score.increase_level();
        }

        // Show the score of the hero based on villains killed
        sf::Font font;
        font.loadFromFile("BebasNeue-Regular.ttf");
        sf::Text scoreText("Score: " + std::to_string(score.getScore()), font, 24);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(10.f, 10.f);

        // Game Over text
        sf::Text gameOverText("Game Over", font, 48);
        gameOverText.setFillColor(sf::Color::Red);
        gameOverText.setPosition(WINDOW_WIDTH / 2 - gameOverText.getGlobalBounds().width / 2, WINDOW_HEIGHT / 2 - gameOverText.getGlobalBounds().height / 2);

        // Clear the window and draw everything
        window.clear();
        window.draw(backgroundSprite); // Draw the background
        window.draw(hero.sprite); // Draw the hero
        window.draw(hero.sword); // Draw the sword

        for (const auto& villain : villains) {
            window.draw(villain.shape); // Draw each villain
        }

        for (const auto& obstacle : obstacles) {
            window.draw(obstacle.shape); // Draw each obstacle
        }

        window.draw(scoreText); // Draw the score text

        if (isGameOver) {
            window.draw(gameOverText); // Draw the game over text
        }

        window.display();
    }

    return 0;
}