#ifndef ANIMATED_SPRITE_H
#define ANIMATED_SPRITE_H

#include <SFML/Graphics.hpp>
#include <vector>

class AnimatedSprite {
public:
    AnimatedSprite(float frame_duration, const std::string& texture_file);

    void add_animation_frame(const sf::IntRect& frame_rect);
    void update_animation(float dt);
    void draw(sf::RenderWindow& window);
    void move(const sf::Vector2f& offset);
    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;
    sf::FloatRect getGlobalBounds() const;
    sf::FloatRect getSwordBounds() const;
    void updateSword();

private:
    sf::Texture texture;
    sf::Sprite sprite;
    std::vector<sf::IntRect> frames;
    std::size_t current_frame;
    float frame_duration;
    float current_time;
    bool is_sword_visible;
    sf::RectangleShape sword;
};

#endif // ANIMATED_SPRITE_H
