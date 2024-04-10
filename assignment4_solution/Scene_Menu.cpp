#include <iostream>

#include "SFML/Graphics/View.hpp"
#include "SFML/Window/Event.hpp"

#include "Action.h"
#include "Assets.h"
#include "GameEngine.h"
#include "Physics.h"
#include "Scene_Menu.h"
#include "Scene_Zelda.h"


Scene_Menu::Scene_Menu(GameEngine *gameEngine) : Scene(gameEngine) {
    init();
}

void Scene_Menu::init() {
    sf::View view = m_game->window().getView();
    view.setCenter(
            static_cast<float>(m_game->window().getSize().x) / 2.0f,
            static_cast<float>(m_game->window().getSize().y) / 2.0f
    );
    m_game->window().setView(view);

    registerAction(sf::Keyboard::W, "UP");
    registerAction(sf::Keyboard::S, "DOWN");
    registerAction(sf::Keyboard::D, "PLAY");
    registerAction(sf::Keyboard::M, "MUTE");
    registerAction(sf::Keyboard::Escape, "QUIT");

    m_titleMusic = m_game->assets().getSound("STitleTheme");
    m_titleMusic.play();

    m_title = "Lunatics";
    int titleSize = 30;

    m_menuText.setString(m_title);
    m_menuText.setFont(m_game->assets().getFont("Mario"));
    m_menuText.setCharacterSize(titleSize);
    m_menuText.setFillColor(sf::Color::Black);
    m_menuText.setPosition(
            float(m_game->window().getSize().x) / 2.0f - float(titleSize * (m_title.length() + 1)) / 2.0f,
            float(titleSize * 3)
    );

    m_menuStrings.emplace_back("LEVEL 1");
    m_menuStrings.emplace_back("LEVEL 2");
    m_menuStrings.emplace_back("LEVEL 3");

    for (int i = 0; i < m_menuStrings.size(); i++) {
        sf::Text text(m_menuStrings[i], m_game->assets().getFont("Mario"), 26);
        if (i != m_selectedMenuIndex) {
            text.setFillColor(sf::Color::Black);
        }
        text.setPosition(
                float(m_game->window().getSize().x) / 2.0f - float(26 * (m_menuStrings[i].length() + 1)) / 2.0f,
                m_menuText.getGlobalBounds().top + 10.0f + 30.0f * float(i + 1)
        );
        m_menuItems.push_back(text);
    }

    m_levelPaths.emplace_back("config/level1.txt");
    m_levelPaths.emplace_back("config/level2.txt");
    m_levelPaths.emplace_back("config/level3.txt");
}

void Scene_Menu::update() {
    sRender();
}

void Scene_Menu::onEnd() {
    m_game->quit();
}

void Scene_Menu::sDoAction(const Action &action) {
    if (action.type() == "START") {
        if (action.name() == "UP") {
            if (m_selectedMenuIndex > 0) {
                m_selectedMenuIndex--;
            } else {
                m_selectedMenuIndex = m_menuStrings.size() - 1;
            }
        } else if (action.name() == "DOWN") {
            m_selectedMenuIndex = (m_selectedMenuIndex + 1) % m_menuStrings.size();
        } else if (action.name() == "PLAY") {
            m_titleMusic.stop();
            m_game->changeScene("PLAY", std::make_shared<Scene_Zelda>(m_game, m_levelPaths[m_selectedMenuIndex]));
        } else if (action.name() == "MUTE") {
            if (m_titleMusic.getStatus() == sf::SoundSource::Playing) {
                m_titleMusic.stop();
            } else {
                m_titleMusic.play();
            }
        } else if (action.name() == "QUIT") {
            onEnd();
        }
    }
}

void Scene_Menu::sRender() {
    // set menu background
    m_game->window().clear(sf::Color(100, 100, 255));

    // draw title
    m_game->window().draw(m_menuText);

    // draw menu items
    for (int i = 0; i < m_menuStrings.size(); i++) {
        if (i != m_selectedMenuIndex) {
            m_menuItems[i].setFillColor(sf::Color::Black);
        } else {
            m_menuItems[i].setFillColor(sf::Color::White);
        }

        m_menuItems[i].setPosition(
                float(m_game->window().getSize().x) / 2.0f - float(26 * (m_menuStrings[i].length() + 1)) / 2.0f,
                m_menuText.getGlobalBounds().top + 10.0f + 30.0f * float(i + 1)
        );
        m_game->window().draw(m_menuItems[i]);
    }

    // draw help
    sf::Text help("W:UP  S:DOWN  D:PLAY  M:MUTE  ESC:BACK/QUIT", m_game->assets().getFont("Mario"), 26);
    help.setFillColor(sf::Color::Black);
    help.setPosition(
            float(m_game->window().getSize().x) / 2.0f - float(26 * (help.getString().getSize() + 1)) / 2.0f,
            float(m_game->window().getSize().y) - 30.0f * 2.0f
    );
    m_game->window().draw(help);
}
