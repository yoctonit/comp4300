#include <iostream>
#include <utility>

#include "SFML/Window/Event.hpp"
#include "GameEngine.h"
#include "Assets.h"
#include "Scene_Menu.h"
#include "Scene_Play.h"


GameEngine::GameEngine(const std::string &path) {
    init(path);
}

void GameEngine::init(const std::string &path) {
    m_assets.loadFromFile(path);

    m_window.create(sf::VideoMode(1280, 768), "Definitely Not Mario");
    m_window.setFramerateLimit(60);

    changeScene("MENU", std::make_shared<Scene_Menu>(this));
}

std::shared_ptr<Scene> GameEngine::currentScene() {
    return m_sceneMap[m_currentScene];
}

bool GameEngine::isRunning() {
    return m_running & m_window.isOpen(); // maybe '&&' instead of '&'?
}

sf::RenderWindow &GameEngine::window() {
    return m_window;
}

void GameEngine::run() {
    while (isRunning()) {
        sUserInput();
        update();
        m_window.display();
    }
}

void GameEngine::sUserInput() {
    sf::Event event{};

    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            quit();
        }

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::X) {
                std::cout << "Save screenshot to " << "test.png" << std::endl;
                sf::Texture texture;
                texture.create(m_window.getSize().x, m_window.getSize().y);
                texture.update(m_window);
                if (texture.copyToImage().saveToFile("test.png")) {
                    std::cout << "Screenshot saved to " << "test.png" << std::endl;
                }
            }
        }

        if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) {
            // if the current scene does not have an action associated with this key, skip the event
            if (currentScene()->getActionMap().find(event.key.code) == currentScene()->getActionMap().end()) {
                continue;
            }

            // determine start or end action by whether it was key press or release
            const std::string actionType = (event.type == sf::Event::KeyPressed) ? "START" : " END";

            // look up the action and send the action to the scene
            currentScene()->doAction(Action(currentScene()->getActionMap().at(event.key.code), actionType));
        }
    }
}

void GameEngine::changeScene(const std::string &sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene) {
    m_currentScene = sceneName;
    std::cout << "Ivan: HERE:" << sceneName <<  "\n";
    m_sceneMap[sceneName] = scene;
    std::cout << "Ivan: End Change Scene:" << sceneName <<  "\n";
}

void GameEngine::quit() {
    m_running = false;
    m_window.close();
}

void GameEngine::update() {
    currentScene()->update();
}

const Assets &GameEngine::assets() const {
    return m_assets;
}
