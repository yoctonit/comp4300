#ifndef SCENE_PLAY_H
#define SCENE_PLAY_H

#include <map>
#include <memory>

#include "SFML/Graphics/Text.hpp"
#include "Scene.h"
#include "EntityManager.h"


class Scene_Play : public Scene {
    struct PlayerConfig {
        float X, Y, CX, CY, SPEED, MAX_SPEED, JUMP, GRAVITY;
        std::string WEAPON;
    };

protected:
    std::shared_ptr<Entity> m_player;
    std::string m_levelPath;
    PlayerConfig m_playerConfig;
    bool m_drawTextures = true;
    bool m_drawCollision = false;
    bool m_drawGrid = false;
    const vec2 m_gridSize = {64, 64};
    sf::Text m_gridText;
    // Physics m_worldPhysics;

    void init(const std::string &levelPath);

    vec2 gridToMidPixel(float, float, std::shared_ptr<Entity>);

    void loadLevel(const std::string &fileName);

    void spawnPlayer();

    void spawnBullet(std::shared_ptr<Entity> entity);

    void sMovement();

    void sLifespan();

    void sCollision();

    void sAnimation();

    void sRender() override;

    void sDoAction(const Action &action) override;

    void onEnd() override;

//    void changePlayerStateTo(PlayerState s);
//    void spawnCoinSpin(std::shared_ptr<Entity> tile);
//    void spawnBrickDebris(std::shared_ptr<Entity> tile);

public:
    Scene_Play(GameEngine *gameEngine, const std::string &levelPath);

    void update() override;
};

#endif //SCENE_PLAY_H
