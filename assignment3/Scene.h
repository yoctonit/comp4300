#ifndef SCENE_H
#define SCENE_H

#include <memory>

#include "Action.h"
#include "EntityManager.h"


class GameEngine;

typedef std::map<int, std::string> ActionMap;

class Scene {
protected:
    GameEngine *m_game = nullptr;
    EntityManager m_entityManager;
    ActionMap m_actionMap;
    bool m_paused = false;
    bool m_hasEnded = false;
    size_t m_currentFrame = 0;

    virtual void onEnd() = 0;

    void setPaused(bool paused);

public:

    Scene();

    explicit Scene(GameEngine *gameEngine);

    virtual ~Scene();

    virtual void update() = 0;

    virtual void sDoAction(const Action &action) = 0;

    virtual void sRender() = 0;

    virtual void doAction(const Action &action);

    void simulate(size_t frames);

    void registerAction(int inputKey, const std::string &actionName);

    [[nodiscard]] size_t width() const;

    [[nodiscard]] size_t height() const;

    [[nodiscard]] size_t currentFrame() const;

    [[nodiscard]] bool hasEnded() const;

    [[nodiscard]] const ActionMap &getActionMap() const;

    void drawLine(const vec2 &p1, const vec2 &p2);
};

#endif //SCENE_H
