#include "rectangle.h"

#include <utility>

Rectangle::Rectangle(RectangleConfig rc, const FontConfig& fc, sf::Font& font)
        : m_config(std::move(rc)) {
    m_shape.setPosition(m_config.posX, m_config.posY);
    m_shape.setSize(sf::Vector2f{m_config.sizeX, m_config.sizeY});
    m_shape.setFillColor(sf::Color{
            static_cast<unsigned char>(m_config.R),
            static_cast<unsigned char>(m_config.G),
            static_cast<unsigned char>(m_config.B)});

    m_text.setFont(font);
    m_text.setString(m_config.name);
    m_text.setCharacterSize(fc.size);
    m_text.setFillColor({
                                static_cast<unsigned char>(fc.R),
                                static_cast<unsigned char>(fc.G),
                                static_cast<unsigned char>(fc.B)
                        });

    sf::FloatRect textRect = m_text.getLocalBounds();
    m_text.setOrigin(textRect.left + textRect.width / 2.0f,textRect.top + textRect.height / 2.0f);
    m_text.setPosition(m_config.posX + m_config.sizeX / 2.0f, m_config.posY + m_config.sizeY / 2.0f);
}

void Rectangle::Draw(sf::RenderWindow& window) const {
    if (m_visible) {
        window.draw(m_shape);
        window.draw(m_text);
    }
}

void Rectangle::Update() {
    m_shape.setPosition(m_shape.getPosition().x + m_config.speedX, m_shape.getPosition().y + m_config.speedY);
    if (m_shape.getPosition().x < 0) {
        m_config.speedX *= -1.0;
    }
    if (m_shape.getPosition().x + m_config.sizeX > 1280) {
        m_config.speedX *= -1.0;
    }
    if (m_shape.getPosition().y < 0) {
        m_config.speedY *= -1.0;
    }
    if (m_shape.getPosition().y + m_config.sizeY > 720) {
        m_config.speedY *= -1.0;
    }
    m_text.setPosition(m_shape.getPosition().x + m_config.sizeX / 2.0f, m_shape.getPosition().y + m_config.sizeY / 2.0f);
}
