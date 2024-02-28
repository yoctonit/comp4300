#include "Game.h"

#include <iostream>

Game::Game(const std::string &config) : m_rng(m_rd()) {
    init(config);
}

void Game::init(const std::string &path) {
    Config config(path);
    config.Print();
    m_playerConfig = config.Player();
    m_enemyConfig = config.Enemy();
    m_bulletConfig = config.Bullet();

    // set up default window parameters
    m_window.create(sf::VideoMode(config.Window().width, config.Window().height), "Assignment 2");
    m_window.setFramerateLimit(config.Window().frameLimit);

    bool imguiInitSucceed = ImGui::SFML::Init(m_window);
    if (!imguiInitSucceed) {
        std::cout << "Failed to initialize imgui library.\n";
    }

    m_randomEnemyPositionX = std::uniform_int_distribution<>(1 + m_enemyConfig.SR,
                                                             config.Window().width - m_enemyConfig.SR);
    m_randomEnemyPositionY = std::uniform_int_distribution<>(1 + m_enemyConfig.SR,
                                                             config.Window().height - m_enemyConfig.SR);
    m_randomColor = std::uniform_int_distribution<>(0, 255);
    m_randomNumberOfVertices = std::uniform_int_distribution<>(m_enemyConfig.VMIN, m_enemyConfig.VMAX);
    m_randomAngle = std::uniform_int_distribution<>(0, 360);

    m_rng.seed(::time(nullptr));

    spawnPlayer();
    spawnEnemy();
}

void Game::run() {
    // TODO:
    // - add pause functionality in here
    // - some systems should function while paused (rendering)
    // - some systems shouldn't (movement / input)
    while (m_running) {
        m_entities.update(); // update the entity manager

        ImGui::SFML::Update(m_window, m_deltaClock.restart()); // required update call to imgui

        if (!m_pause) {
            sEnemySpawner();
            sLifespan();
            sMovement();
            sCollision();
            sUserInput();

            // increment the current frame
            // may need to be moved when pause implemented
            m_currentFrame++;
        }
        sGUI();
        sRender();
    }
}

void Game::setPaused(bool paused) {
    m_pause = paused;
}

// respawn the player in the middle of the screen
void Game::spawnPlayer() {
    // We create every entity by calling EntityManager.addEntity(tag)
    // This returns a std::shared_ptr<Entity>, so we use 'auto' to save typing
    auto entity = m_entities.addEntity("player");

    // Give this entity a Transform, so it spawns at (200,200) with velocity (1,1) and angle 0
    auto windowSize = m_window.getSize();
    entity->cTransform = std::make_shared<CTransform>(
            vec2(float(windowSize.x) / 2.0f, float(windowSize.y) / 2.0f),
            vec2::kZero,
            0.0f);

    // The entity's shape will have radius 32, 8 sides, dark grey fill, and red outline of thickness 4
    entity->cShape = std::make_shared<CShape>(
            m_playerConfig.SR, m_playerConfig.V,
            sf::Color(m_playerConfig.FR, m_playerConfig.FG, m_playerConfig.FB),
            sf::Color(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB),
            m_playerConfig.OT);

    // entity->cCollision->radius = float(m_playerConfig.CR);

    // Add an input component to the player so that we can use inputs
    entity->cInput = std::make_shared<CInput>();

    // Since we want this Entity to be our player, set our Game's player variable to be this Entity
    // This goes slightly against the EntityManager paradigm, but we use the player so much it's worth it
    m_player = entity;
}

// spawn an enemy at a random position
void Game::spawnEnemy() {
    // TODO:
    // make sure the enemy is spawned properly with the m_enemyConfig variables
    // the enemy must be spawned completely within the bounds of the window
    auto posX = float(m_randomEnemyPositionX(m_rng));
    auto posY = float(m_randomEnemyPositionY(m_rng));
    int randomColorR = m_randomColor(m_rng);
    int randomColorG = m_randomColor(m_rng);
    int randomColorB = m_randomColor(m_rng);
    int randomVertices = m_randomNumberOfVertices(m_rng);
    auto randomAngleRad = float(m_randomAngle(m_rng)) * 3.1416f / 180.0f;
    vec2 velocity{std::cos(randomAngleRad), std::sin(randomAngleRad)};

    std::cout << "Enemy spawn at: " << posX << ", " << posY << "\n";
    std::cout << "Enemy has: " << randomVertices << " vertices\n";
    std::cout << "Enemy color: (" << randomColorR << ", " << randomColorG << ", " << randomColorB << ")\n";
    std::cout << "Enemy velocity: (" << velocity.x << ", " << velocity.y << ")\n";
    std::cout << "Velocity magnitude: " << velocity.length() << "\n";

    auto entity = m_entities.addEntity("enemy");
    entity->cTransform = std::make_shared<CTransform>(vec2(posX, posY), velocity, 0.0f);
    entity->cShape = std::make_shared<CShape>(
            m_enemyConfig.SR, randomVertices,
            sf::Color(randomColorR, randomColorG, randomColorB),
            sf::Color(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB),
            m_enemyConfig.OT);

    // record when the most recent enemy was spawned
    m_lastEnemySpawnTime = m_currentFrame;
}

// spawns the small enemies when a big one (input entity e) explodes
void Game::spawnSmallEnemies(const std::shared_ptr<Entity> &enemy) {
    // TODO: spawn small enemies at the location of the input enemy e

    // when we create the smaller enemy, we have to read the values of the original enemy
    // - spawn a number of small enemies equal to the vertices of the original enemy
    // - set each small enemy to the same color as the original, half the size
    // - small enemies are worth double points of the original enemy
    int vertices = int(enemy->cShape->circle.getPointCount());
    // float angle = 0;
    for (int i = 0; i < vertices; i += 1) {
        auto entity = m_entities.addEntity("small-enemy");

        auto angleRad = float(float(i) * 2.0f * 3.1416f / float(vertices));
        vec2 velocity{std::cos(angleRad), std::sin(angleRad)};
        auto pos = enemy->cTransform->pos + float(m_enemyConfig.SR) * velocity;

        entity->cTransform = std::make_shared<CTransform>(pos, velocity, 0.0f);

        entity->cShape = std::make_shared<CShape>(
                m_enemyConfig.SR / 2, vertices,
                enemy->cShape->circle.getFillColor(),
                sf::Color(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB),
                m_enemyConfig.OT);
        entity->cLifespan = std::make_shared<CLifespan>(m_enemyConfig.L);
    }
}

// spawns a bullet from a given entity to a target location
void Game::spawnBullet(const std::shared_ptr<Entity> &entity, const vec2 &target) {
    // TODO: implement the spawning of a bullet which travels toward target
    // - bullet speed is given as a scalar speed
    // - you must set the velocity by using formula in notes
    auto bullet = m_entities.addEntity("bullet");
    vec2 velocity = target - entity->cTransform->pos;
    velocity.normalize();
    bullet->cTransform = std::make_shared<CTransform>(entity->cTransform->pos, velocity, 0.0f);
    bullet->cShape = std::make_shared<CShape>(
            m_bulletConfig.SR, m_bulletConfig.V,
            sf::Color(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB),
            sf::Color(m_bulletConfig.OR, m_bulletConfig.OG, m_bulletConfig.OB),
            m_bulletConfig.OT);
    bullet->cLifespan = std::make_shared<CLifespan>(m_bulletConfig.L);
}

void Game::spawnSpecialWeapon(const std::shared_ptr<Entity> &entity) {
    // TODO: implement your own special weapon
}

void Game::sMovement() {
    // TODO: implement all entity movement in this function
    // you should read the m_player-cInput component to determine if the player is moving
    // sample:
    for (const auto &entity: m_entities.getEntities("bullet")) {
        if (entity->cTransform == nullptr) continue;

        // if entity has transform component...
        entity->cTransform->pos += m_bulletConfig.S * entity->cTransform->velocity;
    }

    for (const auto &entity: m_entities.getEntities("enemy")) {
        if (entity->cTransform == nullptr) continue;

        // if entity has transform component...
        entity->cTransform->pos += m_enemyConfig.SMIN * entity->cTransform->velocity;

        if (entity->cTransform->pos.x - (float) m_enemyConfig.SR < 0) {
            entity->cTransform->velocity.x *= -1.0;
        }
        if (entity->cTransform->pos.x + (float) m_enemyConfig.SR > (float) m_window.getSize().x) {
            entity->cTransform->velocity.x *= -1.0;
        }
        if (entity->cTransform->pos.y - (float) m_enemyConfig.SR < 0) {
            entity->cTransform->velocity.y *= -1.0;
        }
        if (entity->cTransform->pos.y + (float) m_enemyConfig.SR > (float) m_window.getSize().y) {
            entity->cTransform->velocity.y *= -1.0;
        }
    }

    for (const auto &entity: m_entities.getEntities("small-enemy")) {
        if (entity->cTransform == nullptr) continue;

        // if entity has transform component...
        entity->cTransform->pos += m_enemyConfig.SMIN * entity->cTransform->velocity;

        if (entity->cTransform->pos.x - (float) m_enemyConfig.SR < 0) {
            entity->cTransform->velocity.x *= -1.0;
        }
        if (entity->cTransform->pos.x + (float) m_enemyConfig.SR > (float) m_window.getSize().x) {
            entity->cTransform->velocity.x *= -1.0;
        }
        if (entity->cTransform->pos.y - (float) m_enemyConfig.SR < 0) {
            entity->cTransform->velocity.y *= -1.0;
        }
        if (entity->cTransform->pos.y + (float) m_enemyConfig.SR > (float) m_window.getSize().y) {
            entity->cTransform->velocity.y *= -1.0;
        }
    }

    // Sample movement speed update
    m_player->cTransform->velocity = vec2::kZero;
    bool velocityChanged = false;
    if (m_player->cInput->right) {
        m_player->cTransform->velocity.x = 1.0f;
        velocityChanged = true;
    }
    if (m_player->cInput->left) {
        m_player->cTransform->velocity.x = -1.0f;
        velocityChanged = true;
    }
    if (m_player->cInput->up) {
        m_player->cTransform->velocity.y = -1.0f;
        velocityChanged = true;
    }
    if (m_player->cInput->down) {
        m_player->cTransform->velocity.y = 1.0f;
        velocityChanged = true;
    }
    if (velocityChanged) m_player->cTransform->velocity.normalize();
//    std::cout << m_player->cTransform->velocity.x << ", " << m_player->cTransform->velocity.y << '\n';
    m_player->cTransform->pos += m_playerConfig.S * m_player->cTransform->velocity;
    m_player->cTransform->pos += m_playerConfig.S * m_player->cTransform->velocity;

    if (m_player->cTransform->pos.x - (float) m_playerConfig.SR < 0) {
        m_player->cTransform->pos.x = (float) m_playerConfig.SR;
    }
    if (m_player->cTransform->pos.x + (float) m_playerConfig.SR > (float) m_window.getSize().x) {
        m_player->cTransform->pos.x = (float) m_window.getSize().x - (float) m_playerConfig.SR;
    }
    if (m_player->cTransform->pos.y - (float) m_playerConfig.SR < 0) {
        m_player->cTransform->pos.y = (float) m_playerConfig.SR;
    }
    if (m_player->cTransform->pos.y + (float) m_playerConfig.SR > (float) m_window.getSize().y) {
        m_player->cTransform->pos.y = (float) m_window.getSize().y - (float) m_playerConfig.SR;
    }

//    m_player->cTransform->pos.x += m_player->cTransform->velocity.x;
//    m_player->cTransform->pos.y += m_player->cTransform->velocity.y;
}

void Game::sLifespan() {
    // for all entities
    // - if entity has no lifespan component, skip it
    // - if entity has > 0 remaining lifespan, subtract 1
    // - if it has lifespan and is alive scale its alpha channel properly
    // - if it has lifespan and its time is up destroy the entity
    for (const auto &entity: m_entities.getEntities()) {
        if (entity->cLifespan == nullptr) continue;

        if (entity->cLifespan->remaining > 0) {
            entity->cLifespan->remaining -= 1;
        }

        if (entity->cLifespan->remaining > 0 && entity->isActive()) {
            sf::Color color = entity->cShape->circle.getFillColor();
            color.a = static_cast<int>((float(entity->cLifespan->remaining) / float(entity->cLifespan->total)) * 255);
            // std::cout << "color a: " << int(color.a) << "\n";
            entity->cShape->circle.setFillColor(color);
        } else {
            entity->destroy();
        }
    }

}

void Game::sCollision() {
    // TODO: implement all proper collisions between entities
    // be sure to use the collision radius, not the shape radius
    // sample
    for (const auto& bullet: m_entities.getEntities("bullet")) {
        for (const auto& entity: m_entities.getEntities("enemy")) {
            // if (entity->id() == m_player->id()) continue;
            if (!bullet->isActive()) continue;
            if (!bullet->isActive()) continue;
            //check collision
            float dist = (entity->cTransform->pos - bullet->cTransform->pos).length_squared();
            float sumOfRadii = (float(m_enemyConfig.CR) + float(m_bulletConfig.CR));
            std::cout << "entity->cTransform->pos: " << entity->cTransform->pos.x << "," << entity->cTransform->pos.y << '\n';
            std::cout << "bullet->cTransform->pos: " << bullet->cTransform->pos.x << "," << bullet->cTransform->pos.y << '\n';
            std::cout << "here's bullet: " << dist << " <? " << sumOfRadii * sumOfRadii << "\n";
            if (dist < sumOfRadii * sumOfRadii) {
                spawnSmallEnemies(entity);
                entity->destroy();
                bullet->destroy();
            }
        }
        // we need another loop for small entities because small entities don't spawn further
    }
}

void Game::sEnemySpawner() {
    if (m_currentFrame - m_lastEnemySpawnTime == m_enemyConfig.SI) {
        spawnEnemy();
    }
}

void Game::sGUI() {
    ImGui::Begin("Geometry Wars");

    ImGui::Text("Stuff Goes Here");

    ImGui::End();
}

void Game::sRender() {
    // TODO: change the code below to draw ALL of the entities
    // sample drawing of the player Entity that we have created
    m_window.clear();

    // set the position of the shape based on the entity's transform->pos
    m_player->cShape->circle.setPosition(m_player->cTransform->pos.x, m_player->cTransform->pos.y);

    // set the rotation of the shape based on the entity's transform->angle
    m_player->cTransform->angle += 1.0f;
    m_player->cShape->circle.setRotation(m_player->cTransform->angle);

    // draw the entity's sf::CircleShape
    m_window.draw(m_player->cShape->circle);

    for (const auto &entity: m_entities.getEntities("enemy")) {
        entity->cShape->circle.setPosition(entity->cTransform->pos.x, entity->cTransform->pos.y);
        entity->cTransform->angle += 1.0f;
        entity->cShape->circle.setRotation(entity->cTransform->angle);

        m_window.draw(entity->cShape->circle);
    }

    for (const auto &entity: m_entities.getEntities("small-enemy")) {
        entity->cShape->circle.setPosition(entity->cTransform->pos.x, entity->cTransform->pos.y);
        entity->cTransform->angle += 1.0f;
        entity->cShape->circle.setRotation(entity->cTransform->angle);

        m_window.draw(entity->cShape->circle);
    }

    for (const auto &entity: m_entities.getEntities("bullet")) {
        entity->cShape->circle.setPosition(entity->cTransform->pos.x, entity->cTransform->pos.y);
        entity->cTransform->angle += 1.0f;
        entity->cShape->circle.setRotation(entity->cTransform->angle);

        m_window.draw(entity->cShape->circle);
    }

    // draw the ui last
    ImGui::SFML::Render(m_window);

    m_window.display();
}

void Game::sUserInput() {
    // TODO: handle user input here
    // note that you should only be setting the player's input component variables here
    // you should not implement the player's movement logic here
    // the movement system will read the variables you set in this function

    sf::Event event{};
    while (m_window.pollEvent(event)) {
        // pass the event to imgui to be parsed
        ImGui::SFML::ProcessEvent(m_window, event);

        // this event triggers when the window is closed
        if (event.type == sf::Event::Closed) {
            m_running = false;
        }

        // this event is triggered when a key is pressed
        if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
                case sf::Keyboard::W:
                    // std::cout << "W Key Pressed\n";
                    m_player->cInput->up = true;
                    break;

                case sf::Keyboard::S:
                    // std::cout << "S Key Pressed\n";
                    m_player->cInput->down = true;
                    break;

                case sf::Keyboard::A:
                    // std::cout << "A Key Pressed\n";
                    m_player->cInput->left = true;
                    break;

                case sf::Keyboard::D:
                    // std::cout << "D Key Pressed\n";
                    m_player->cInput->right = true;
                    break;

                case sf::Keyboard::P:
                    std::cout << "P Key Pressed\n";
                    // setPaused(!m_pause);
//                    m_pause = !m_pause;
                    break;

                case sf::Keyboard::Escape:
                    m_running = false;
                    break;

                default:
                    break;
            }
        }

        // this event is triggered when a key is released
        if (event.type == sf::Event::KeyReleased) {
            switch (event.key.code) {
                case sf::Keyboard::W:
                    // std::cout << "W Key Released\n";
                    m_player->cInput->up = false;
                    break;

                case sf::Keyboard::S:
                    // std::cout << "S Key Released\n";
                    m_player->cInput->down = false;
                    break;

                case sf::Keyboard::A:
                    // std::cout << "A Key Released\n";
                    m_player->cInput->left = false;
                    break;

                case sf::Keyboard::D:
                    // std::cout << "D Key Released\n";
                    m_player->cInput->right = false;
                    break;

                default:
                    break;
            }

        }

        if (event.type == sf::Event::MouseButtonPressed) {
            // this line ignores mouse events if ImGui is the thing being clicked
            if (ImGui::GetIO().WantCaptureMouse) { continue; }

            if (event.mouseButton.button == sf::Mouse::Left) {
//                std::cout << "Left Mouse Button Clicked at(" << event.mouseButton.x
//                          << ", " << event.mouseButton.y << ")\n";
                vec2 target(float(event.mouseButton.x), float(event.mouseButton.y));
                spawnBullet(m_player, target);
            }

            if (event.mouseButton.button == sf::Mouse::Right) {
                std::cout << "Right Mouse Button Clicked at(" << event.mouseButton.x
                          << ", " << event.mouseButton.y << ")\n";
                // TODO: call special weapon here
                // spawnSmallEnemies(m_player);
            }
        }
    }
}
