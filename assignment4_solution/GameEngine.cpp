#include <iostream>
#include <string>

#include "imgui.h"
#include "imgui-SFML.h"
#include "SFML/Window/Event.hpp"

#include "Assets.h"
#include "GameEngine.h"
#include "Scene_Menu.h"


GameEngine::GameEngine(const std::string &path) {
    init(path);
}

void GameEngine::init(const std::string &path) {
    m_assets.loadFromFile(path);

    m_window.create(sf::VideoMode(1280, 768), "Definitely Not Mario");
    m_window.setFramerateLimit(60);

    if (!ImGui::SFML::Init(m_window)) {
        std::cerr << "Failed ImGui initialization\n";
    }

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
        ImGui::SFML::Update(m_window, m_deltaClock.restart());
        update();
        ImGui::SFML::Render(m_window);
        m_window.display();
    }
}

void GameEngine::sUserInput() {
    sf::Event event{};

    while (m_window.pollEvent(event)) {
        ImGui::SFML::ProcessEvent(m_window, event);

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
            const std::string actionType = (event.type == sf::Event::KeyPressed) ? "START" : "END";

            // look up the action and send the action to the scene
            currentScene()->doAction(Action(currentScene()->getActionMap().at(event.key.code), actionType));
        }

        // this line ignores mouse events if ImGui is the thing clicked
        if (ImGui::GetIO().WantCaptureMouse) { continue; }

        // mouse event below
        auto mousePos = sf::Mouse::getPosition(m_window);
        vec2 pos = vec2(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
        if (event.type == sf::Event::MouseButtonPressed) {
            switch (event.mouseButton.button) {
                case sf::Mouse::Left:
                    currentScene()->doAction(Action("LEFT_CLICK", "START", pos));
                    break;
                case sf::Mouse::Middle:
                    currentScene()->doAction(Action("MIDDLE_CLICK", "START", pos));
                    break;
                case sf::Mouse::Right:
                    currentScene()->doAction(Action("RIGHT_CLICK", "START", pos));
                    break;
                default:
                    break;
            }
        }

        if (event.type == sf::Event::MouseButtonReleased) {
            switch (event.mouseButton.button) {
                case sf::Mouse::Left:
                    currentScene()->doAction(Action("LEFT_CLICK", "END", pos));
                    break;
                case sf::Mouse::Middle:
                    currentScene()->doAction(Action("MIDDLE_CLICK", "END", pos));
                    break;
                case sf::Mouse::Right:
                    currentScene()->doAction(Action("RIGHT_CLICK", "END", pos));
                    break;
                default:
                    break;
            }
        }

        if (event.type == sf::Event::MouseMoved) {
            currentScene()->doAction(Action("MOUSE_MOVE", vec2(static_cast<float>(event.mouseMove.x), static_cast<float>(event.mouseMove.y))));
        }
    }
}

void GameEngine::changeScene(const std::string &sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene) {
    m_currentScene = sceneName;
    m_sceneMap[sceneName] = std::move(scene);
}

void GameEngine::quit() {
    m_running = false;
    m_window.close();
}

void GameEngine::update() {
    currentScene()->update();
}

Assets &GameEngine::assets() {
    return m_assets;
}
