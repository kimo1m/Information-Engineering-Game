#include "AnimatedSprite.h"

AnimatedSprite::AnimatedSprite(float frame_duration, const std::string& texture_file)
    : frame_duration(frame_duration), current_frame(0), current_time(0.0f), is_sword_visible(false) {
    if (!texture.loadFromFile(texture_file)) {
        throw std::runtime_error("Failed to load texture file");
    }
    sprite.setTexture(texture);
}

void AnimatedSprite::add_animation_frame(const sf::IntRect& frame_rect) {
    frames.push_back(frame_rect);
}

void AnimatedSprite::update_animation(float dt) {
    if (frames.empty()) return;

    current_time += dt;
    if (current_time >= frame_duration) {
        current_time -= frame_duration;
        current_frame = (current_frame + 1) % frames.size();
        sprite.setTextureRect(frames[current_frame]);
    }
}

void AnimatedSprite::draw(sf::RenderWindow& window) {
    window.draw(sprite);
    if (is_sword_visible) {
        window.draw(sword);
    }
}

void AnimatedSprite::move(const sf::Vector2f& offset) {
    sprite.move(offset);
    sword.move(offset);
}

void AnimatedSprite::setPosition(float x, float y) {
    sprite.setPosition(x, y);
    updateSword();
}

sf::Vector2f AnimatedSprite::getPosition() const {
    return sprite.getPosition();
}

sf::FloatRect AnimatedSprite::getGlobalBounds() const {
    return sprite.getGlobalBounds();
}

sf::FloatRect AnimatedSprite::getSwordBounds() const {
    return sword.getGlobalBounds();
}

void AnimatedSprite::updateSword() {
    if (is_sword_visible) {
        sf::Vector2f heroPosition = sprite.getPosition();
        // Adjust sword position relative to the hero position
        sword.setPosition(heroPosition.x + 20.f, heroPosition.y);
    }
}
