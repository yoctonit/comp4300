#include "rectangle.h"

#include <utility>

Rectangle::Rectangle(const RectangleConfig &sc, const FontConfig &fc, sf::Font &font, WindowConfig wc) : Shape() {
    m_type = "rectangle";
    m_name = sc.name;

    m_posX = sc.posX;
    m_posY = sc.posY;

    m_sizeX = sc.sizeX;
    m_sizeY = sc.sizeY;

    m_speedX = sc.speedX;
    m_speedY = sc.speedY;

    m_windowWidth = static_cast<float>(wc.width);
    m_windowHeight = static_cast<float>(wc.height);

    m_R = static_cast<unsigned char>(sc.R);
    m_G = static_cast<unsigned char>(sc.G);
    m_B = static_cast<unsigned char>(sc.B);
    m_color[0] = static_cast<float>(sc.R) / 255.f;
    m_color[1] = static_cast<float >(sc.G) / 255.f;
    m_color[2] = static_cast<float>(sc.B) / 255.f;

    m_fontSize = fc.size;
    m_fontColorR = static_cast<unsigned char>(fc.R);
    m_fontColorG = static_cast<unsigned char>(fc.G);
    m_fontColorB = static_cast<unsigned char>(fc.B);

    m_text.setFont(font);
    m_text.setString(m_name);
    m_text.setCharacterSize(m_fontSize);
    m_text.setFillColor({m_fontColorR, m_fontColorG, m_fontColorB});

    CalculateNamePosition();
//    sf::FloatRect textRect = m_text.getLocalBounds();
//    m_text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
//    m_text.setPosition(m_posX + m_sizeX / 2.0f, m_posY + m_sizeY / 2.0f);

    m_shape.setPosition(m_posX, m_posY);
    m_shape.setSize(sf::Vector2f{m_sizeX, m_sizeY});
    m_shape.setFillColor(sf::Color{m_R, m_G, m_B});
}

void Rectangle::Draw(sf::RenderWindow &window) const {
    if (m_visible) {
        window.draw(m_shape);
        window.draw(m_text);
    }
}

void Rectangle::Update() {
    m_posX += m_speedX;
    m_posY += m_speedY;

    if (m_posX < 0) {
        m_speedX *= -1.0;
    }
    if (m_posX + m_sizeX * m_scale > m_windowWidth) {
        m_speedX *= -1.0;
    }
    if (m_posY < 0) {
        m_speedY *= -1.0;
    }
    if (m_posY + m_sizeY * m_scale > m_windowHeight) {
        m_speedY *= -1.0;
    }
    m_shape.setPosition(m_posX, m_posY);
    m_shape.setScale(m_scale, m_scale);
    CalculateNamePosition();
//    m_text.setPosition(m_posX + m_sizeX / 2.0f, m_posY + m_sizeY / 2.0f);
}

void Rectangle::CalculateNamePosition() {
    sf::FloatRect textRect = m_text.getLocalBounds();
    m_text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    m_text.setPosition(m_posX + m_sizeX * m_scale / 2.0f, m_posY + m_sizeY * m_scale / 2.0f);
}

void Rectangle::SetColor() {
    m_shape.setFillColor(sf::Color{
            static_cast<unsigned char>(m_color[0] * 255.f),
            static_cast<unsigned char>(m_color[1] * 255.f),
            static_cast<unsigned char>(m_color[2] * 255.f)
    });
}