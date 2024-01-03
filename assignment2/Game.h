#ifndef GAME_H
#define GAME_H

#include "Entity.h"
#include "EntityManager.h"

#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"


struct PlayerConfig {
    int SR, CR, FR, FG, FB, OR, OG, OB, OT, V;
    float S;
};
struct EnemyConfig {
    int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI;
    float SMIN, SMAX;
};
struct BulletConfig {
    int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L;
    float S;
};


class Game {
    sf::RenderWindow m_window;              // the window we will draw to
    EntityManager m_entities;               // vector of entities to maintain
    sf::Font m_font;                 // the font we will use to draw
    sf::Text m_text;                 // the score text to be drawn to the screen
    PlayerConfig m_playerConfig;
    EnemyConfig m_enemyConfig;
    BulletConfig m_bulletConfig;
    sf::Clock m_deltaClock;
    int m_score = 0;
    int m_currentFrame = 0;
    std::shared_ptr<Entity> m_player;
    bool m_pause = false;
    bool m_running = true;
    int m_lastEnemySpawnTime;

    void init(const std::string &path);

    void sMovement();

    void sLifespan();

    void sUserInput();

    void sRender();

    void sEnemySpawner();

    void sGUI();

    void sCollision();

public:
    Game(const std::string &config);

    // void update();
    void run();

    void setPaused(bool paused);

    void spawnPlayer();

    void spawnEnemy();

    void spawnSmallEnemies(std::shared_ptr<Entity> e);

    void spawnBullet(std::shared_ptr<Entity> entity, const Vec2 &target);

    void spawnSpecialWeapon(std::shared_ptr<Entity> entity);
};

#endif //GAME_H
