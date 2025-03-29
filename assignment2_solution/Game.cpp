#include "Game.h"

#include <iostream>
#include <sstream>

Game::Game(const std::string &config) : m_rng(m_rd()) {
    init(config);
}

void Game::init(const std::string &path) {
    Config config(path);
    // config.Print();
    m_playerConfig = config.Player();
    m_enemyConfig = config.Enemy();
    m_bulletConfig = config.Bullet();
    m_spawningInterval = m_enemyConfig.SI;

    // set up default window parameters
    m_window.create(sf::VideoMode(config.Window().width, config.Window().height), "Assignment 2");
    m_window.setFramerateLimit(config.Window().frameLimit);

    bool imguiInitSucceed = ImGui::SFML::Init(m_window);
    if (!imguiInitSucceed)
    {
        std::cerr << "Failed to initialize ImGui library.\n";
    }

    m_randomEnemyPositionX = std::uniform_int_distribution<>(1 + m_enemyConfig.SR,
                                                             config.Window().width - m_enemyConfig.SR);
    m_randomEnemyPositionY = std::uniform_int_distribution<>(1 + m_enemyConfig.SR,
                                                             config.Window().height - m_enemyConfig.SR);
    m_randomColor = std::uniform_int_distribution<>(0, 255);
    m_randomNumberOfVertices = std::uniform_int_distribution<>(m_enemyConfig.VMIN, m_enemyConfig.VMAX);
    m_randomAngle = std::uniform_real_distribution<float>(0.0f, 360.0f);
    m_randomSpeed = std::uniform_real_distribution<float>(m_enemyConfig.SMIN, m_enemyConfig.SMAX);

    m_rng.seed(::time(nullptr));

    // attempt to load the font from a file
    if (!m_font.loadFromFile(config.Font().file))
    {
        // if we can't load the font, print an error to the error console and exit
        std::cerr << "Could not load font!\n";
        exit(-1);
    }

    // set up the text object that will be drawn to the screen
    m_text = sf::Text("Score: 0", m_font, config.Font().size);
    m_text.setFillColor(sf::Color(config.Font().R, config.Font().G, config.Font().B));
    m_text.setPosition(sf::Vector2f(5.0f, 5.0f));

    spawnPlayer();
    spawnEnemy();
}

void Game::run() {
    // - add pause functionality in here
    // - some systems should function while paused (rendering)
    // - some systems shouldn't (movement / input)
    while (m_running)
    {
        m_entities.update(); // update the entity manager

        ImGui::SFML::Update(m_window, m_deltaClock.restart()); // required update call to imgui

        if (!m_pause)
        {
            if (m_enableSpawningSystem)
            {
                sEnemySpawner();
            }
            if (m_enableLifespanSystem)
            {
                sLifespan();
            }
            if (m_enableMovementSystem)
            {
                sMovement();
            }
            if (m_enableCollisionSystem)
            {
                sCollision();
            }
            sUserInput();

            m_currentFrame++;
        }
        if (m_enableGuiSystem)
        {
            sGUI();
        }
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

    auto windowSize = m_window.getSize();
    entity->cTransform = std::make_shared<CTransform>(
            vec2(float(windowSize.x) / 2.0f, float(windowSize.y) / 2.0f),
            vec2::kZero,
            0.0f);

    entity->cShape = std::make_shared<CShape>(
            m_playerConfig.SR, m_playerConfig.V,
            sf::Color(m_playerConfig.FR, m_playerConfig.FG, m_playerConfig.FB),
            sf::Color(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB),
            m_playerConfig.OT);

    entity->cCollision = std::make_shared<CCollision>(float(m_playerConfig.CR));
    // entity->cCollision->radius = float(m_playerConfig.CR);

    // Add an input component to the player so that we can use inputs
    entity->cInput = std::make_shared<CInput>();

    // Since we want this Entity to be our player, set our Game's player variable to be this Entity
    // This goes slightly against the EntityManager paradigm, but we use the player so much it's worth it
    m_player = entity;
}

// spawn an enemy at a random position
void Game::spawnEnemy() {
    // make sure the enemy is spawned properly with the m_enemyConfig variables
    // the enemy must be spawned completely within the bounds of the window
    auto posX = float(m_randomEnemyPositionX(m_rng));
    auto posY = float(m_randomEnemyPositionY(m_rng));
    int randomColorR = m_randomColor(m_rng);
    int randomColorG = m_randomColor(m_rng);
    int randomColorB = m_randomColor(m_rng);
    int randomVertices = m_randomNumberOfVertices(m_rng);
    auto randomAngleRad = m_randomAngle(m_rng) * 3.1416f / 180.0f;
    auto randomSpeed = m_randomSpeed(m_rng);
    vec2 direction{std::cos(randomAngleRad), std::sin(randomAngleRad)};

//    std::cout << "Enemy spawn at: " << posX << ", " << posY << "\n";
//    std::cout << "Enemy has: " << randomVertices << " vertices\n";
//    std::cout << "Enemy color: (" << randomColorR << ", " << randomColorG << ", " << randomColorB << ")\n";
//    std::cout << "Enemy velocity: (" << direction.x << ", " << direction.y << ")\n";
//    std::cout << "Velocity magnitude: " << direction.length() << "\n";

    auto entity = m_entities.addEntity("enemy");
    entity->cTransform = std::make_shared<CTransform>(vec2(posX, posY), randomSpeed * direction, 0.0f);
    entity->cShape = std::make_shared<CShape>(
            m_enemyConfig.SR, randomVertices,
            sf::Color(randomColorR, randomColorG, randomColorB),
            sf::Color(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB),
            m_enemyConfig.OT);
    entity->cCollision = std::make_shared<CCollision>(float(m_enemyConfig.CR));
    entity->cScore = std::make_shared<CScore>(randomVertices * 100);

    // record when the most recent enemy was spawned
    m_lastEnemySpawnTime = m_currentFrame;
}

// spawns the small enemies when a big one (input entity e) explodes
void Game::spawnSmallEnemies(const std::shared_ptr<Entity> &enemy) {
    // spawn small enemies at the location of the input enemy e

    // when we create the smaller enemy, we have to read the values of the original enemy
    // - spawn a number of small enemies equal to the vertices of the original enemy
    // - set each small enemy to the same color as the original, half the size
    // - small enemies are worth double points of the original enemy
    int vertices = int(enemy->cShape->circle.getPointCount());
    for (int i = 0; i < vertices; i += 1)
    {
        auto entity = m_entities.addEntity("small-enemy");

        auto angleRad = float(float(i) * 2.0f * 3.1416f / float(vertices));
        vec2 direction{std::cos(angleRad), std::sin(angleRad)};
        auto pos = enemy->cTransform->pos + float(m_enemyConfig.SR) * direction;
        auto velocity = enemy->cTransform->velocity.length() * direction;

        entity->cTransform = std::make_shared<CTransform>(pos, velocity, 0.0f);

        entity->cShape = std::make_shared<CShape>(
                float(m_enemyConfig.SR) / 2.0f, vertices,
                enemy->cShape->circle.getFillColor(),
                sf::Color(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB),
                m_enemyConfig.OT);
        entity->cCollision = std::make_shared<CCollision>(float(m_enemyConfig.CR) / 2.0f);
        entity->cScore = std::make_shared<CScore>(2 * vertices * 100);
        entity->cLifespan = std::make_shared<CLifespan>(m_enemyConfig.L);
    }
}

// spawns a bullet from a given entity to a target location
void Game::spawnBullet(const std::shared_ptr<Entity> &entity, const vec2 &target) {
    // implement the spawning of a bullet which travels toward target
    // - bullet speed is given as a scalar speed
    // - you must set the velocity by using formula in notes
    auto bullet = m_entities.addEntity("bullet");
    vec2 direction = target - entity->cTransform->pos;
    direction.normalize();
    bullet->cTransform = std::make_shared<CTransform>(
            entity->cTransform->pos + float(m_playerConfig.SR) * direction,
            m_bulletConfig.S * direction,
            0.0f);
    bullet->cShape = std::make_shared<CShape>(
            m_bulletConfig.SR, m_bulletConfig.V,
            sf::Color(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB),
            sf::Color(m_bulletConfig.OR, m_bulletConfig.OG, m_bulletConfig.OB),
            m_bulletConfig.OT);
    entity->cCollision = std::make_shared<CCollision>(float(m_bulletConfig.CR));
    bullet->cLifespan = std::make_shared<CLifespan>(m_bulletConfig.L);
}

void Game::spawnSpecialWeapon(/* const std::shared_ptr<Entity> &entity */) {
    if (m_currentFrame - m_lastSpecialWeaponActivated < m_lastSpecialWeaponInterval)
    {
        return;
    }

    const int MAX_BULLETS = 16;
    vec2 playerPos = m_player->cTransform->pos;
    auto deltaAngle = 2.0f * 3.1416f / MAX_BULLETS;
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        auto angleRad = static_cast<float>(i) * deltaAngle;
        vec2 direction{std::cos(angleRad), std::sin(angleRad)};
        vec2 target = playerPos + direction;
        spawnBullet(m_player, target);
    }
    m_lastSpecialWeaponActivated = m_currentFrame;
}

void Game::sMovement() {
    // implement all entity movement in this function
    // you should read the m_player->cInput component to determine if the player is moving
    for (const auto &entity: m_entities.getEntities("bullet"))
    {
        if (entity->cTransform == nullptr) continue;
        entity->cTransform->pos += entity->cTransform->velocity;
    }

    for (const auto &entity: m_entities.getEntities("enemy"))
    {
        if (entity->cTransform == nullptr) continue;

        entity->cTransform->pos += entity->cTransform->velocity;

        if (entity->cTransform->pos.x - (float) m_enemyConfig.SR < 0)
        {
            entity->cTransform->velocity.x *= -1.0;
        }
        if (entity->cTransform->pos.x + (float) m_enemyConfig.SR > (float) m_window.getSize().x)
        {
            entity->cTransform->velocity.x *= -1.0;
        }
        if (entity->cTransform->pos.y - (float) m_enemyConfig.SR < 0)
        {
            entity->cTransform->velocity.y *= -1.0;
        }
        if (entity->cTransform->pos.y + (float) m_enemyConfig.SR > (float) m_window.getSize().y)
        {
            entity->cTransform->velocity.y *= -1.0;
        }
    }

    for (const auto &entity: m_entities.getEntities("small-enemy"))
    {
        if (entity->cTransform == nullptr) continue;

        entity->cTransform->pos += entity->cTransform->velocity;

        if (entity->cTransform->pos.x - (float) m_enemyConfig.SR / 2.0f < 0)
        {
            entity->cTransform->velocity.x *= -1.0;
        }
        if (entity->cTransform->pos.x + (float) m_enemyConfig.SR / 2.0f > (float) m_window.getSize().x)
        {
            entity->cTransform->velocity.x *= -1.0;
        }
        if (entity->cTransform->pos.y - (float) m_enemyConfig.SR / 2.0f < 0)
        {
            entity->cTransform->velocity.y *= -1.0;
        }
        if (entity->cTransform->pos.y + (float) m_enemyConfig.SR / 2.0f > (float) m_window.getSize().y)
        {
            entity->cTransform->velocity.y *= -1.0;
        }
    }

    m_player->cTransform->velocity = vec2::kZero;
    bool velocityChanged = false;
    if (m_player->cInput->right)
    {
        m_player->cTransform->velocity.x = 1.0f;
        velocityChanged = true;
    }
    if (m_player->cInput->left)
    {
        m_player->cTransform->velocity.x = -1.0f;
        velocityChanged = true;
    }
    if (m_player->cInput->up)
    {
        m_player->cTransform->velocity.y = -1.0f;
        velocityChanged = true;
    }
    if (m_player->cInput->down)
    {
        m_player->cTransform->velocity.y = 1.0f;
        velocityChanged = true;
    }
    if (velocityChanged)
    {
        m_player->cTransform->velocity.normalize();
        m_player->cTransform->velocity *= m_playerConfig.S;
    }
    // std::cout << m_player->cTransform->velocity.x << ", " << m_player->cTransform->velocity.y << '\n';
    m_player->cTransform->pos += m_player->cTransform->velocity;

    if (m_player->cTransform->pos.x - (float) m_playerConfig.SR < 0)
    {
        m_player->cTransform->pos.x = (float) m_playerConfig.SR;
    }
    if (m_player->cTransform->pos.x + (float) m_playerConfig.SR > (float) m_window.getSize().x)
    {
        m_player->cTransform->pos.x = (float) m_window.getSize().x - (float) m_playerConfig.SR;
    }
    if (m_player->cTransform->pos.y - (float) m_playerConfig.SR < 0)
    {
        m_player->cTransform->pos.y = (float) m_playerConfig.SR;
    }
    if (m_player->cTransform->pos.y + (float) m_playerConfig.SR > (float) m_window.getSize().y)
    {
        m_player->cTransform->pos.y = (float) m_window.getSize().y - (float) m_playerConfig.SR;
    }
}

void Game::sLifespan() {
    // for all entities
    // - if entity has no lifespan component, skip it
    // - if entity has > 0 remaining lifespan, subtract 1
    // - if it has lifespan and is alive scale its alpha channel properly
    // - if it has lifespan and its time is up destroy the entity
    for (const auto &entity: m_entities.getEntities())
    {
        if (entity->cLifespan == nullptr) continue;

        if (entity->cLifespan->remaining > 0)
        {
            entity->cLifespan->remaining -= 1;
        }

        if (entity->cLifespan->remaining > 0 && entity->isActive())
        {
            sf::Color color = entity->cShape->circle.getFillColor();
            color.a = static_cast<int>((float(entity->cLifespan->remaining) / float(entity->cLifespan->total)) * 255);
            // std::cout << "color a: " << int(color.a) << "\n";
            entity->cShape->circle.setFillColor(color);
        }
        else
        {
            entity->destroy();
        }
    }

}

void Game::sCollision() {
    // implement all proper collisions between entities
    // be sure to use the collision radius, not the shape radius
    for (const auto &bullet: m_entities.getEntities("bullet"))
    {
        for (const auto &entity: m_entities.getEntities("enemy"))
        {
            if (!bullet->isActive()) continue;
            //check collision
            float dist = (entity->cTransform->pos - bullet->cTransform->pos).length_squared();
            float sumOfRadii = (float(m_enemyConfig.CR) + float(m_bulletConfig.CR));
//            std::cout << "entity->cTransform->pos: " << entity->cTransform->pos.x
//                      << "," << entity->cTransform->pos.y << '\n';
//            std::cout << "bullet->cTransform->pos: " << bullet->cTransform->pos.x
//                      << "," << bullet->cTransform->pos.y << '\n';
//            std::cout << "here's bullet: " << dist << " <? " << sumOfRadii * sumOfRadii << "\n";
            if (dist < sumOfRadii * sumOfRadii)
            {
                spawnSmallEnemies(entity);
                entity->destroy();
                bullet->destroy();
                m_score += entity->cScore->score;
            }
        }
        // we need another loop for small entities because small entities don't spawn further
        for (const auto &entity: m_entities.getEntities("small-enemy"))
        {
            if (!bullet->isActive()) continue;
            //check collision
            float dist = (entity->cTransform->pos - bullet->cTransform->pos).length_squared();
            float sumOfRadii = (float(m_enemyConfig.CR) / 2.0f + float(m_bulletConfig.CR));
            if (dist < sumOfRadii * sumOfRadii)
            {
                entity->destroy();
                bullet->destroy();
                m_score += entity->cScore->score;
            }
        }
    }

    //check collision with player
    for (const auto &entity: m_entities.getEntities("enemy"))
    {
        if (!entity->isActive()) continue;

        float dist = (entity->cTransform->pos - m_player->cTransform->pos).length_squared();
        float sumOfRadii = (float(m_enemyConfig.CR) + float(m_playerConfig.CR));
        if (dist < sumOfRadii * sumOfRadii)
        {
            spawnSmallEnemies(entity);
            entity->destroy();
            m_player->destroy();
            spawnPlayer();
        }
    }
    for (const auto &entity: m_entities.getEntities("small-enemy"))
    {
        if (!entity->isActive()) continue;

        float dist = (entity->cTransform->pos - m_player->cTransform->pos).length_squared();
        float sumOfRadii = (float(m_enemyConfig.CR) / 2.0f + float(m_playerConfig.CR));
        if (dist < sumOfRadii * sumOfRadii)
        {
            entity->destroy();
            m_player->destroy();
            spawnPlayer();
        }
    }
}

std::string Game::getScoreText() const {
    std::ostringstream oss;
    oss << "Score: " << m_score;
    return oss.str();
}

std::string Game::getSpecialWeaponText() const {
    std::ostringstream oss;
    int readyIn = (m_lastSpecialWeaponInterval - (m_currentFrame - m_lastSpecialWeaponActivated)) / 60;
    // std::cout << "readyIn " << readyIn << std::endl;
    if (readyIn > 0)
    {
        oss << "Special weapon ready in: " << readyIn;
    }
    else
    {
        oss << "Special weapon: Ready";
    }
    return oss.str();
}

std::string Game::getButtonId(size_t entityId) const {
    std::ostringstream oss;
    oss << "D##" << entityId;
    return oss.str();
}

void Game::sEnemySpawner() {
    if (m_currentFrame - m_lastEnemySpawnTime == m_spawningInterval)
    {
        spawnEnemy();
    }
}

void Game::renderEntityList(const EntityVec &entities) const {
    for (const auto &entity: entities)
    {
        auto entityColor = entity->cShape->circle.getFillColor();
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor(entityColor.r, entityColor.g, entityColor.b));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor(entityColor.r, entityColor.g, entityColor.b));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor(entityColor.r, entityColor.g, entityColor.b));
        if (ImGui::Button(getButtonId(entity->id()).c_str()))
        {
            // std::cout << entity->id() << std::endl;
            entity->destroy();
        }
        ImGui::PopStyleColor(3);
        ImGui::SameLine();
        ImGui::Text("%d %s (%d, %d)",
                    entity->id(),
                    entity->tag().c_str(),
                    static_cast<int>(entity->cTransform->pos.x),
                    static_cast<int>(entity->cTransform->pos.y));
    }
//                        ImGui::PushID(i);
//                        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(i / 7.0f, 0.6f, 0.6f));
//                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(i / 7.0f, 0.7f, 0.7f));
//                        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(i / 7.0f, 0.8f, 0.8f));
//                        ImGui::Button("Click");
//                        ImGui::PopStyleColor(3);
//                        ImGui::PopID();
}

void Game::sGUI() {
    ImGui::Begin("Geometry Wars");

    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
    if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
    {
        if (ImGui::BeginTabItem("Systems"))
        {
            ImGui::Checkbox("Movement", &m_enableMovementSystem);
            ImGui::Checkbox("Lifespan", &m_enableLifespanSystem);
            ImGui::Checkbox("Collision", &m_enableCollisionSystem);
            ImGui::Checkbox("Spawning", &m_enableSpawningSystem);
            ImGui::Indent();
            ImGui::SliderInt("Spawn", &m_spawningInterval, 0, m_enemyConfig.SI);
            if (ImGui::Button("Manual Spawn"))
            {
                // m_manualSpawn = true;
                spawnEnemy();
            }
            ImGui::Unindent();
            ImGui::Checkbox("Gui", &m_enableGuiSystem);
            ImGui::Checkbox("Rendering", &m_enableRenderingSystem);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Entity Manager"))
        {
            if (ImGui::TreeNode("Entities by Tag"))
            {
                if (ImGui::CollapsingHeader("Bullet", ImGuiTreeNodeFlags_None))
                {
                    renderEntityList(m_entities.getEntities("bullet"));
                }
                if (ImGui::CollapsingHeader("Enemy", ImGuiTreeNodeFlags_None))
                {
                    renderEntityList(m_entities.getEntities("enemy"));
                }
                if (ImGui::CollapsingHeader("Player", ImGuiTreeNodeFlags_None))
                {
                    renderEntityList(m_entities.getEntities("player"));
                }
                if (ImGui::CollapsingHeader("Small Enemies", ImGuiTreeNodeFlags_None))
                {
                    renderEntityList(m_entities.getEntities("small-enemy"));
                }
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("All Entities"))
            {
                renderEntityList(m_entities.getEntities());
                ImGui::TreePop();
            }
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    ImGui::End();
}

void Game::sRender() {
    m_window.clear();

    if (m_enableRenderingSystem)
    {
        for (const auto &entity: m_entities.getEntities())
        {
            entity->cShape->circle.setPosition(entity->cTransform->pos.x, entity->cTransform->pos.y);
            entity->cTransform->angle += 1.0f;
            entity->cShape->circle.setRotation(entity->cTransform->angle);

            m_window.draw(entity->cShape->circle);
        }
        std::string message = getScoreText() + "    " + getSpecialWeaponText();
        m_text.setString(message.c_str());
        m_window.draw(m_text);
    }

    // draw the ui last
    ImGui::SFML::Render(m_window);

    m_window.display();
}

void Game::sUserInput() {
    // handle user input here
    // note that you should only be setting the player's input component variables here
    // you should not implement the player's movement logic here
    // the movement system will read the variables you set in this function

    sf::Event event{};
    while (m_window.pollEvent(event))
    {
        // pass the event to imgui to be parsed
        ImGui::SFML::ProcessEvent(m_window, event);

        // this event triggers when the window is closed
        if (event.type == sf::Event::Closed)
        {
            m_running = false;
        }

        // this event is triggered when a key is pressed
        if (event.type == sf::Event::KeyPressed)
        {
            switch (event.key.code)
            {
                case sf::Keyboard::W:
                    m_player->cInput->up = true;
                    break;

                case sf::Keyboard::S:
                    m_player->cInput->down = true;
                    break;

                case sf::Keyboard::A:
                    m_player->cInput->left = true;
                    break;

                case sf::Keyboard::D:
                    m_player->cInput->right = true;
                    break;

                case sf::Keyboard::P:
                    // std::cout << "P Key Pressed\n";
                    m_pause = !m_pause;
                    break;

                case sf::Keyboard::G:
                    m_enableGuiSystem = !m_enableGuiSystem;
                    break;

                case sf::Keyboard::Escape:
                    m_running = false;
                    break;

                default:
                    break;
            }
        }

        // this event is triggered when a key is released
        if (event.type == sf::Event::KeyReleased)
        {
            switch (event.key.code)
            {
                case sf::Keyboard::W:
                    m_player->cInput->up = false;
                    break;

                case sf::Keyboard::S:
                    m_player->cInput->down = false;
                    break;

                case sf::Keyboard::A:
                    m_player->cInput->left = false;
                    break;

                case sf::Keyboard::D:
                    m_player->cInput->right = false;
                    break;

                default:
                    break;
            }
        }

        if (event.type == sf::Event::MouseButtonPressed)
        {
            // this line ignores mouse events if ImGui is the thing being clicked
            if (ImGui::GetIO().WantCaptureMouse) continue;

            if (event.mouseButton.button == sf::Mouse::Left)
            {
//                std::cout << "Left Mouse Button Clicked at(" << event.mouseButton.x
//                          << ", " << event.mouseButton.y << ")\n";
                vec2 target(float(event.mouseButton.x), float(event.mouseButton.y));
                spawnBullet(m_player, target);
            }

            if (event.mouseButton.button == sf::Mouse::Right)
            {
//                std::cout << "Right Mouse Button Clicked at(" << event.mouseButton.x
//                          << ", " << event.mouseButton.y << ")\n";
                spawnSpecialWeapon();
            }
        }
    }
}
