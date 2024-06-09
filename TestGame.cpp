#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>

// Constants for the window size
const int WINDOW_WIDTH = 360;
const int WINDOW_HEIGHT = 360;

// Movement speed for the hero
const float HERO_SPEED = 0.05f;
// Movement speed for the villains
const float VILLAIN_SPEED = 0.02f;

// Time interval (in seconds) to generate new villains
const float VILLAIN_GENERATION_INTERVAL = 2.0f;

// Hero class definition
class Hero {
public:
    sf::RectangleShape shape;
    sf::RectangleShape sword;

    Hero() {
        // Initialize the hero
        shape.setSize(sf::Vector2f(50.f, 50.f));
        shape.setFillColor(sf::Color::Blue);
        shape.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

        // Initialize the sword
        sword.setSize(sf::Vector2f(10.f, 50.f));
        sword.setFillColor(sf::Color::Red);
    }

    void updateSword() {
        // Position the sword relative to the hero
        sword.setPosition(shape.getPosition().x + shape.getSize().x, shape.getPosition().y);
    }

    void move(sf::Vector2f direction) {
        // Move the hero with boundary checks
        sf::Vector2f newPosition = shape.getPosition() + direction;

        if (newPosition.x >= 0 && newPosition.x + shape.getSize().x <= WINDOW_WIDTH &&
            newPosition.y >= 0 && newPosition.y + shape.getSize().y <= WINDOW_HEIGHT) {
            shape.setPosition(newPosition);
        }
    }
};

// Villain class definition
class Villain {
public:
    sf::RectangleShape shape;

    Villain(float x, float y) {
        // Initialize the villain
        shape.setSize(sf::Vector2f(50.f, 50.f));
        shape.setFillColor(sf::Color::Green);
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

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Hero vs Villains");

    // Load the background texture
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("background.jpg")) {
        return -1;
    }
    sf::Sprite backgroundSprite(backgroundTexture);

    Hero hero;
    std::vector<Villain> villains;

    // Initialize the random seed
    srand(static_cast<unsigned>(time(0)));

    sf::Clock clock;
    float timeSinceLastGeneration = 0.0f;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Move the hero with arrow keys
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            hero.move(sf::Vector2f(-HERO_SPEED, 0.f));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            hero.move(sf::Vector2f(HERO_SPEED, 0.f));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            hero.move(sf::Vector2f(0.f, -HERO_SPEED));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            hero.move(sf::Vector2f(0.f, HERO_SPEED));
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

        // Move the villains towards the hero
        for (auto& villain : villains) {
            villain.moveTowards(hero.shape.getPosition());
        }

        // Check for collisions between sword and villains
        for (auto it = villains.begin(); it != villains.end(); ) {
            if (hero.sword.getGlobalBounds().intersects(it->shape.getGlobalBounds())) {
                it = villains.erase(it); // Remove the villain if hit by the sword
            }
            else {
                ++it;
            }
        }

        window.clear();
        window.draw(hero.shape);
        window.draw(backgroundSprite);
        window.draw(hero.sword);
        for (const auto& villain : villains) {
            window.draw(villain.shape);
        }
        window.display();
    }

    return 0;
}