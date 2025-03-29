#ifndef GAME_H
#define GAME_H

#include <random>
#include <ctime>

#include "Entity.h"
#include "EntityManager.h"
#include "config.h"

#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"


class Game {
    sf::RenderWindow m_window;       // the window we will draw to

    EntityManager m_entities;        // vector of entities to maintain
    sf::Font m_font;                 // the font we will use to draw
    sf::Text m_text;                 // the score text to be drawn to the screen

    PlayerConfig m_playerConfig{};
    EnemyConfig m_enemyConfig{};
    BulletConfig m_bulletConfig{};

    sf::Clock m_deltaClock;
    int m_score = 0;
    int m_currentFrame = 0;
    std::shared_ptr<Entity> m_player;
    bool m_pause = false;
    bool m_running = true;
    int m_lastEnemySpawnTime = 0;
    int m_lastSpecialWeaponActivated = 0;
    int m_lastSpecialWeaponInterval = 300; // every 5 seconds

    // gui controlled variables
    bool m_enableMovementSystem = true;
    bool m_enableLifespanSystem = true;
    bool m_enableCollisionSystem = true;
    bool m_enableSpawningSystem = true;
    bool m_enableGuiSystem = true;
    bool m_enableRenderingSystem = true;
    int m_spawningInterval{};;
    // bool m_manualSpawn = false;

    std::random_device m_rd;
    typedef std::mt19937 MyRng;
    MyRng m_rng;
    std::uniform_int_distribution<int> m_randomEnemyPositionX;
    std::uniform_int_distribution<int> m_randomEnemyPositionY;
    std::uniform_int_distribution<int> m_randomColor;
    std::uniform_int_distribution<int> m_randomNumberOfVertices;
    std::uniform_real_distribution<float> m_randomAngle;
    std::uniform_real_distribution<float> m_randomSpeed;

    void init(const std::string &path);

    void sMovement();

    void sLifespan();

    void sUserInput();

    void sRender();

    void sEnemySpawner();

    void sGUI();

    void sCollision();

    std::string getScoreText() const;

    std::string getSpecialWeaponText() const;

    std::string getButtonId(size_t entityId) const;

    void renderEntityList(const EntityVec & entities) const;

public:
    explicit Game(const std::string &config);

    void run();

    void setPaused(bool paused);

    void spawnPlayer();

    void spawnEnemy();

    void spawnSmallEnemies(const std::shared_ptr<Entity> &e);

    void spawnBullet(const std::shared_ptr<Entity> &entity, const vec2 &target);

    void spawnSpecialWeapon(/* const std::shared_ptr<Entity> &entity */);
};

#endif //GAME_H
