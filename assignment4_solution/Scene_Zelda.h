#include <memory>
#include "SFML/Graphics/Text.hpp"

// #include "Common.h"
#include "Components.h"
#include "Physics.h"
#include "Scene.h"


class Scene_Zelda : public Scene {
    struct PlayerConfig {
        float X, Y, CX, CY, SPEED, HEALTH;
    };

protected:
    std::string m_levelPath;
    PlayerConfig m_playerConfig{};

    bool m_drawTextures = true;
    bool m_drawCollision = false;
    bool m_drawGrid = false;
    bool m_follow = false;

    const vec2 m_gridSize = {64, 64};
    sf::Text m_gridText;

    vec2 m_mousePos;

    void init(const std::string &levelPath);

    void loadLevel(const std::string &levelPath);

    void onEnd() override;

    void spawnPlayer();

    void spawnSword(const std::shared_ptr<Entity> &entity);

    vec2 getPosition(int rx, int ry, int tx, int ty) const;

    std::shared_ptr<Entity> player();

    void sDoAction(const Action &action) override;

    void sMovement();

    void sAI();

    void sStatus();

    void sAnimation();

    void sCollision();

    void sCamera();

    void sGUI();

    void sRender() override;

    void sDrag();

    void changePlayerStateTo(const std::string &state, const vec2 &facing);

    vec2 windowToWorld(const vec2 &pos);

    vec2 getRoomXY(const vec2 &pos);

    void movementStart(const std::string &actionName);

    void animatePlayer();

public:

    Scene_Zelda(GameEngine *gameEngine, std::string &levelPath);

    void update() override;
};
