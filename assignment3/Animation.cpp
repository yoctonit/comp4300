#include "Animation.h"
#include <cmath>
#include <utility>


Animation::Animation() = default;

Animation::Animation(const std::string &name, const sf::Texture &t)
        : Animation(name, t, 1, 0) {}

Animation::Animation(std::string name, const sf::Texture &t, size_t frameCount, size_t speed)
        : m_name(std::move(name)), m_sprite(t), m_frameCount(frameCount), m_currentFrame(0), m_speed(speed) {
    m_size = vec2((float) t.getSize().x / float(frameCount), (float) t.getSize().y);
    m_sprite.setOrigin(m_size.x / 2.0f, m_size.y / 2.0f);
    m_sprite.setTextureRect(sf::IntRect(std::floor(float(m_currentFrame) * m_size.x), 0, m_size.x, m_size.y));
}

// updates the animation to show the next frame, depending on its speed
// animation loops when it reaches the end
void Animation::update() {
    // add the speed variable to the current frame
    m_currentFrame++;

    // TODO: 1) calculate the correct frame of animation to play based on currentFrame and speed
    //       2) set the texture rectangle properly (see constructor for sample)
}

bool Animation::hasEnded() const {
    // TODO: detect when animation has ended (last frame waw played) and return true
    return false;
}

const vec2 &Animation::getSize() const {
    return m_size;
}

const std::string &Animation::getName() const {
    return m_name;
}

sf::Sprite &Animation::getSprite() {
    return m_sprite;
}

