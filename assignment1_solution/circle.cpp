#include "circle.h"

#include <utility>

Circle::Circle(CircleConfig rc, sf::Font& font)
        : m_config(std::move(rc)) {
    m_shape.setPosition(m_config.posX, m_config.posY);
    m_shape.setRadius(m_config.radius);
    m_shape.setFillColor(sf::Color{
            static_cast<unsigned char>(m_config.R),
            static_cast<unsigned char>(m_config.G),
            static_cast<unsigned char>(m_config.B)});

    m_text.setFont(font);
    m_text.setString(m_config.name);
    m_text.setCharacterSize(16);
    m_text.setFillColor(sf::Color::White);

    sf::FloatRect textRect = m_text.getLocalBounds();
    m_text.setOrigin(textRect.left + textRect.width / 2.0f,textRect.top + textRect.height / 2.0f);
    m_text.setPosition(m_config.posX + m_config.radius / 2.0f, m_config.posY + m_config.radius / 2.0f);
}

void Circle::Draw(sf::RenderWindow& window) const {
    if (m_visible) {
        window.draw(m_shape);
        window.draw(m_text);
    }
}
