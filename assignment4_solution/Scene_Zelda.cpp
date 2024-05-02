#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Event.hpp"

#include "imgui.h"
#include "imgui-SFML.h"

#include "Components.h"
#include "GameEngine.h"
#include "Scene_Menu.h"
#include "Scene_Zelda.h"
#include "../assignment2/vec2.h"


Scene_Zelda::Scene_Zelda(GameEngine *game, std::string &levelPath)
        : Scene(game), m_levelPath(levelPath) {
    init(m_levelPath);
}

void Scene_Zelda::init(const std::string &levelPath) {
    loadLevel(levelPath);

    m_gridText.setCharacterSize(12);
    m_gridText.setFont(m_game->assets().getFont("Tech"));

    registerAction(sf::Keyboard::Escape, "QUIT");
    registerAction(sf::Keyboard::P, "PAUSE");
    registerAction(sf::Keyboard::Y, "TOGGLE_FOLLOW");    // toggle follow camera
    registerAction(sf::Keyboard::T, "TOGGLE_TEXTURE");   // toggle drawing (T)extures
    registerAction(sf::Keyboard::C, "TOGGLE_COLLISION"); // toggle drawing (C)ollision Box
    registerAction(sf::Keyboard::G, "TOGGLE_GRID");      // toggle drawing (G)rid

    // Register the actions required to play the game
    registerAction(sf::Keyboard::W, "UP");
    registerAction(sf::Keyboard::S, "DOWN");
    registerAction(sf::Keyboard::A, "LEFT");
    registerAction(sf::Keyboard::D, "RIGHT");
    registerAction(sf::Keyboard::Space, "ATTACK");
}

void Scene_Zelda::loadLevel(const std::string &fileName) {
    // reset the entity manager every time we load a level
    m_entityManager = EntityManager();

    // Load the level file and put all entities in the manager
    // Use the getPosition() function below to convert room-tile coords to game world coords
    std::ifstream file(fileName);
    if (!file) {
        std::cerr << "Scene_Zelda::loadLevel could not load " << fileName << " file.\n";
        exit(-1);
    }

    std::string entityType;
    while (file >> entityType) {
        if (entityType == "Tile") {
            std::string animationName;
            int roomX, roomY, tileX, tileY, blockMovement, blockVision;
            file >> animationName
                 >> roomX >> roomY
                 >> tileX >> tileY
                 >> blockMovement >> blockVision;

            auto tile = m_entityManager.addEntity("tile");
            tile->add<CAnimation>(m_game->assets().getAnimation(animationName), true);
            tile->add<CTransform>(getPosition(roomX, roomY, tileX, tileY));
            vec2 boxSize = tile->get<CAnimation>().animation.getSize();
            boxSize -= vec2(2.0f, 2.0f);
            tile->add<CBoundingBox>(tile->get<CTransform>().pos, boxSize, blockMovement, blockVision);
            tile->add<CDraggable>();
            // std::cout << "Loaded Tile " << animationName << std::endl;

        } else if (entityType == "NPC") {
            std::string animationName, aiType;
            int roomX, roomY, tileX, tileY, blockMovement, blockVision, maxHealth, damage;
            file >> animationName
                 >> roomX >> roomY
                 >> tileX >> tileY
                 >> blockMovement >> blockVision
                 >> maxHealth >> damage
                 >> aiType;

            auto npc = m_entityManager.addEntity("npc");
            npc->add<CAnimation>(m_game->assets().getAnimation(animationName), true);
            npc->add<CTransform>(getPosition(roomX, roomY, tileX, tileY));
            npc->add<CBoundingBox>(npc->get<CTransform>().pos, vec2(63.0f, 63.0f), false, false);
            npc->add<CHealth>(maxHealth, maxHealth);
            npc->add<CDamage>(damage);

            if (aiType == "Follow") {
                float speed;
                file >> speed;
                npc->add<CFollowPlayer>(getPosition(roomX, roomY, tileX, tileY), speed);

            } else if (aiType == "Patrol") {
                float speed;
                int numPositions;
                file >> speed >> numPositions;
                std::vector<vec2> positions;

                for (int i = 0; i < numPositions; i++) {
                    int xi, yi;
                    file >> xi >> yi;
                    positions.emplace_back(xi, yi);
                }
                npc->add<CPatrol>(positions, speed);
            }
            // std::cout << "Loaded NPC " << animationName << " with AI " << aiType << std::endl;

        } else if (entityType == "Player") {
            file >> m_playerConfig.X >> m_playerConfig.Y
                 >> m_playerConfig.CX >> m_playerConfig.CY
                 >> m_playerConfig.SPEED
                 >> m_playerConfig.HEALTH;
            spawnPlayer();

        } else {
            std::cerr << "Unknown entity type " << entityType << "\n";
            // exit(-1);
        }
    }
}

vec2 Scene_Zelda::getPosition(int rx, int ry, int tx, int ty) const {
    // Implement this function, which takes in the room (rx, ry)
    // as well as the tile (tx, ty), and return the vec2 game world
    // position of the center of the entity
    auto roomX = static_cast<float>(rx);
    auto roomY = static_cast<float>(ry);
    auto tileX = static_cast<float>(tx);
    auto tileY = static_cast<float>(ty);

    return {
            roomX * width() + tileX * m_gridSize.x - m_gridSize.x / 2.0f,
            roomY * height() + tileY * m_gridSize.y - m_gridSize.y / 2.0f
    };
}

void Scene_Zelda::spawnPlayer() {
    auto player = m_entityManager.addEntity("player");

    // Implement this function so that it uses the parameters input from the level file
    // Those parameters should be stored in the m_playerConfig variable
    player->add<CTransform>(vec2(m_playerConfig.X, m_playerConfig.Y));
    player->add<CAnimation>(m_game->assets().getAnimation("LinkStandDown"), true);
    player->add<CBoundingBox>(
            vec2(m_playerConfig.X, m_playerConfig.Y),
            vec2(m_playerConfig.CX, m_playerConfig.CY),
            true, false
    );
    player->add<CHealth>(m_playerConfig.HEALTH, m_playerConfig.HEALTH / 2);
    player->add<CState>("stand_down");
}

void Scene_Zelda::spawnSword(const std::shared_ptr<Entity> &entity) {
    auto sword = m_entityManager.addEntity("sword");

    // Implement the spawning of the sword, which:
    // - should be given the appropriate lifespan
    sword->add<CLifespan>(8, m_currentFrame);

    // - should spawn at the appropriate location based on player's facing direction
    vec2 facing = entity->get<CTransform>().facing;
    vec2 boxC = entity->get<CBoundingBox>().center;
    float swordL = 45.0f;
    float swordW = 15.0f;
    vec2 swordCenter = vec2(
            boxC.x + facing.x * (swordL + 10.0f),
            boxC.y + facing.y * (swordL + 10.0f)
    );
    sword->add<CTransform>(swordCenter);
    if (facing.x != 0) {
        sword->add<CBoundingBox>(swordCenter, vec2(swordL, swordW), false, false);
    } else if (facing.y != 0) {
        sword->add<CBoundingBox>(swordCenter, vec2(swordW, swordL), false, false);
    }

    // - be given a damage value of 1
    sword->add<CDamage>(1);

    // - should play the "Slash" sound
    m_game->assets().getSound("SSwordSlash").play();
}

void Scene_Zelda::update() {
    m_entityManager.update();

    // Implement pause functionality
    if (!m_paused) {
        sDrag();
        sAI();
        sMovement();
        sStatus();
        sCollision();
        sAnimation();
        sCamera();
        m_currentFrame++;
    }

    sGUI();
    sRender();
}

void Scene_Zelda::sMovement() {
    // Implement all player movement functionality here based on
    // the player's input component variables
    auto p = player();

    if (p) {
        p->get<CTransform>().velocity = vec2(0.0f, 0.0f);
        if (p->get<CInput>().up && !p->get<CInput>().down &&
            !p->get<CInput>().left && !p->get<CInput>().right) {
            p->get<CTransform>().velocity.y = -m_playerConfig.SPEED;
            // p->get<CTransform>().facing.y = -1;
        } else if (!p->get<CInput>().up && p->get<CInput>().down &&
                   !p->get<CInput>().left && !p->get<CInput>().right) {
            p->get<CTransform>().velocity.y = m_playerConfig.SPEED;
            // p->get<CTransform>().facing.y = 1;
        } else if (!p->get<CInput>().up && !p->get<CInput>().down &&
                   p->get<CInput>().left && !p->get<CInput>().right) {
            p->get<CTransform>().velocity.x = -m_playerConfig.SPEED;
            // p->get<CTransform>().facing.x = -1;
        } else if (!p->get<CInput>().up && !p->get<CInput>().down &&
                   !p->get<CInput>().left && p->get<CInput>().right) {
            p->get<CTransform>().velocity.x = m_playerConfig.SPEED;
            // p->get<CTransform>().facing.x = 1;
        }
        if (p->get<CInput>().attack) {
            if (m_entityManager.getEntities("sword").empty()) {
                spawnSword(p);
            }
        }
    }

    for (const auto &entity: m_entityManager.getEntities()) {
        if (entity->has<CTransform>()) {
            // check if npc returns to home with follow AI
            // if (entity->has<CFollowPlayer>()) {
            //     auto& npcTrans = entity->get<CTransform>();
            //     vec2`& npcHome = entity->get<CFollowPlayer>().home;
            //     if (npcTrans.velocity.x != 0 || npcTrans.velocity.y != 0) {
            //         float dx = npcHome.x - npcTrans.pos.x;
            //         float dy = npcHome.y - npcTrans.pos.y;
            //         // if npc is closing home point set to home point
            //         if (dx < 1 && dy < 1 && dx > -1 && dy > -1) {
            //             npcTrans.prevPos = npcTrans.pos;
            //             entity->get<CBoundingBox>().prevCenter =
            //                 entity->get<CBoundingBox>().center;
            //             
            //             npcTrans.pos = npcHome;
            //             entity->get<CBoundingBox>().center = npcHome;
            //         }
            //     }
            // }

            // move entities
            entity->get<CTransform>().prevPos = entity->get<CTransform>().pos;
            entity->get<CTransform>().pos += entity->get<CTransform>().velocity;

            entity->get<CBoundingBox>().prevCenter = entity->get<CBoundingBox>().center;
            entity->get<CBoundingBox>().center = entity->get<CTransform>().pos;
            // entity->get<CBoundingBox>().center += entity->get<CTransform>().velocity;
        }
    }

}

void Scene_Zelda::sGUI() {
    ImGui::Begin("Scene Properties");

    if (ImGui::BeginTabBar("MyTabBar")) {

        if (ImGui::BeginTabItem("Debug")) {
            ImGui::Checkbox("Draw Grid (G)", &m_drawGrid);
            ImGui::Checkbox("Draw Textures (T)", &m_drawTextures);
            ImGui::Checkbox("Draw Debug (C)", &m_drawCollision);
            ImGui::Checkbox("Follow Cam (Y)", &m_follow);

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Animations")) {
            ImGui::Text("Click a Tile to create on the Game, then drag to move.");

            static bool bm = true;
            static bool bv = true;
            ImGui::Checkbox("Block Movement", &bm);
            ImGui::Checkbox("Block Vision", &bv);

            int i = 0;
            for (const auto &[name, anim]: m_game->assets().getAnimations()) {
                if (name.find("Tile") != std::string::npos) {
                    if (i++ % 5 != 0) ImGui::SameLine();

                    ImVec2 size;
                    if (ImGui::ImageButton(name.c_str(), anim.getSprite(), size)) {
                        auto tile = m_entityManager.addEntity("tile");
                        tile->add<CAnimation>(m_game->assets().getAnimation(name), true);
                        auto view = m_game->window().getView().getCenter();
                        tile->add<CTransform>(vec2(view.x, view.y));
                        tile->add<CBoundingBox>(
                                vec2(view.x, view.y),
                                tile->get<CAnimation>().animation.getSize() - vec2(2.0f, 2.0f),
                                bm,
                                bv
                        );
                        tile->add<CDraggable>();
                        // std::cout << "create " << name << " with block movement=" << bm << " and block vision=" << bv << std::endl;
                    }
                    ImGui::SetItemTooltip("%s", name.c_str());
                }
            }
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Sounds")) {
            for (auto &[name, sound]: m_game->assets().getSounds()) {
                ImGui::PushID(name.c_str());
                if (ImGui::Button("play")) {
                    sound.play();
                }
                ImGui::PopID();
                ImGui::SameLine();
                ImGui::PushID(name.c_str());
                if (ImGui::Button("stop")) {
                    sound.stop();
                }
                ImGui::PopID();
                ImGui::SameLine();
                ImGui::Text("%s", name.c_str());
            }
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Entity Manager")) {
            static ImGuiTreeNodeFlags tflags = ImGuiTreeNodeFlags_Framed;

            if (ImGui::TreeNodeEx("Entities by Tag", tflags)) {
                for (auto &[tag, entityVec]: m_entityManager.getEntityMap()) {
                    if (ImGui::TreeNodeEx(tag.c_str(), tflags)) {
                        for (const auto &entity: entityVec) {
                            ImGui::Text("%s", std::to_string(entity->id()).c_str());
                            ImGui::SameLine();
                            ImGui::Text("%s", entity->tag().c_str());
                            ImGui::SameLine();
                            ImGui::Text("(%.f, %.f)",
                                        entity->get<CTransform>().pos.x,
                                        entity->get<CTransform>().pos.y
                            );
                        }
                        ImGui::TreePop();
                    }
                }

                ImGui::TreePop();
            }

            if (ImGui::TreeNodeEx("All Entities", tflags)) {
                for (const auto &entity: m_entityManager.getEntities()) {
                    ImGui::Text("%s", std::to_string(entity->id()).c_str());
                    ImGui::SameLine();
                    ImGui::Text("%s", entity->tag().c_str());
                    ImGui::SameLine();
                    ImGui::Text("(%.f, %.f)",
                                entity->get<CTransform>().pos.x,
                                entity->get<CTransform>().pos.y
                    );
                }
                ImGui::TreePop();
            }

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }
    ImGui::End();
}

void Scene_Zelda::movementStart(const std::string &actionName) {
    if (!player()) return;

    if (actionName == "UP") {
        player()->get<CInput>().up = true;

    } else if (actionName == "DOWN") {
        player()->get<CInput>().down = true;

    } else if (actionName == "LEFT") {
        player()->get<CInput>().left = true;

    } else if (actionName == "RIGHT") {
        player()->get<CInput>().right = true;

    } else if (actionName == "ATTACK") {
        m_game->assets().getSound("SSwordSlash").play();
        player()->get<CInput>().attack = true;
    }
}

void Scene_Zelda::sDoAction(const Action &action) {
    // Implement all actions described for the game here
    // Only the setting of the player's input component variables should be set here
    // Do minimal logic in this function

    if (action.name() == "MOUSE_MOVE") {
        m_mousePos = action.pos();
    }

    if (action.type() == "START") {
        if (action.name() == "PAUSE") {
            setPaused(!m_paused);
        } else if (action.name() == "QUIT") {
            onEnd();
        } else if (action.name() == "TOGGLE_FOLLOW") {
            m_follow = !m_follow;
        } else if (action.name() == "TOGGLE_TEXTURE") {
            m_drawTextures = !m_drawTextures;
        } else if (action.name() == "TOGGLE_COLLISION") {
            m_drawCollision = !m_drawCollision;
        } else if (action.name() == "TOGGLE_GRID") {
            m_drawGrid = !m_drawGrid;
        } else if (action.name() == "LEFT_CLICK") {
            // detect the picking up of entities
            vec2 wPos = windowToWorld(m_mousePos);
            for (const auto &entity: m_entityManager.getEntities()) {
                if (!entity->has<CDraggable>()) { continue; }

                if (Physics::IsInside(wPos, entity)) {
                    auto &dragging = entity->get<CDraggable>().dragging;
                    dragging = !dragging;
                }
            }
        } else {
            movementStart(action.name());
        }

    } else if (action.type() == "END") {
        if (!player()) return;

        if (action.name() == "UP") {
            player()->get<CInput>().up = false;
        } else if (action.name() == "DOWN") {
            player()->get<CInput>().down = false;
        } else if (action.name() == "LEFT") {
            player()->get<CInput>().left = false;
        } else if (action.name() == "RIGHT") {
            player()->get<CInput>().right = false;
        } else if (action.name() == "ATTACK") {
            player()->get<CInput>().attack = false;
        }
    }
}

void Scene_Zelda::sAI() {
    // Implement enemy AI
    // Implement Follow behavior
    // Implement Patrol behavior
    for (const auto &npc: m_entityManager.getEntities("npc")) {
        auto p = player();
        if (!p) break;

        vec2 &pPos = p->get<CTransform>().pos;
        vec2 &npcPos = npc->get<CTransform>().pos;

        // Follow AI
        if (getRoomXY(pPos) == getRoomXY(npcPos) && npc->has<CFollowPlayer>()) {

            bool blockVision = false;
            // check if any tile is blocking vision between npc and player
            for (const auto &entity: m_entityManager.getEntities()) {
                if (entity->has<CBoundingBox>() && entity != p) {
                    if (entity->get<CBoundingBox>().blockVision && Physics::EntityIntersect(pPos, npcPos, entity)) {
                        blockVision = true;
                        break;
                    }
                }
            }

            if (!blockVision) { // npc will follow player
                npc->get<CTransform>().velocity = Physics::getSpeedAB(npcPos, pPos, npc->get<CFollowPlayer>().speed);
            } else { // npc will go back to home
                npc->get<CTransform>().velocity =
                        Physics::getSpeedAB(npcPos, npc->get<CFollowPlayer>().home, npc->get<CFollowPlayer>().speed);
            }
        }

        // Patrol AI
        if (npc->has<CPatrol>()) {
            auto &npcTrans = npc->get<CTransform>();
            size_t &currIdx = npc->get<CPatrol>().currentPosition;
            auto &positions = npc->get<CPatrol>().positions;
            size_t nextIdx = (currIdx + 1) % positions.size();
            vec2 room = getRoomXY(npcTrans.pos);
            vec2 toCoord = getPosition(
                    room.x, room.y,
                    positions[nextIdx].x, positions[nextIdx].y
            );
            npcTrans.velocity = Physics::getSpeedAB(npcPos, toCoord, npc->get<CPatrol>().speed);
            // if npc is closing to the next point 
            // change speed to the next point
            if (length(npcPos - toCoord) < 2.0f) {
                currIdx = (currIdx + 1) % positions.size();
            }
        }
    }
}

void Scene_Zelda::sStatus() {
    // Implement Lifespan here
    // Implement Invincibility Frames here
    for (const auto &entity: m_entityManager.getEntities()) {
        if (entity->has<CLifespan>()) {
            if (m_currentFrame - entity->get<CLifespan>().frameCreated > entity->get<CLifespan>().lifespan) {
                entity->destroy();
            }
        }
        if (entity->has<CInvincibility>()) {
            // add invincibility about 30 frames (0.5 sec)
            if (m_currentFrame - entity->get<CInvincibility>().iframes > 60) {
                entity->remove<CInvincibility>();
            }
        }
    }
}

void Scene_Zelda::sCollision() {
    // Implement entity - tile collisions
    // Implement player - enemy collisions with appropriate damage calculations
    // Implement sword - NPC collisions
    // Implement entity - heart collisions and life gain logic
    // Implement black tile collisions / 'teleporting'
    // You may want to use helper functions for these behaviors or this function will get long
    auto p = player();
    if (!p) return;

    // player and tiles
    for (const auto &tile: m_entityManager.getEntities("tile")) {
        RectOverlap ro = Physics::AisNearB(p, tile, m_gridSize);

        switch (ro.direction) {
            case ODirection::UP:
                // std::cout << "block player move down\n";
                if (tile->get<CBoundingBox>().blockMove) {
                    p->get<CTransform>().pos.y -= ro.overlap.y;
                    p->get<CBoundingBox>().center.y -= ro.overlap.y;
                }
                break;
            case ODirection::DOWN:
                // std::cout << "block player move up\n";
                if (tile->get<CBoundingBox>().blockMove) {
                    p->get<CTransform>().pos.y += ro.overlap.y;
                    p->get<CBoundingBox>().center.y += ro.overlap.y;
                }
                break;
            case ODirection::LEFT:
                // std::cout << "block player move right\n";
                if (tile->get<CBoundingBox>().blockMove) {
                    p->get<CTransform>().pos.x -= ro.overlap.x;
                    p->get<CBoundingBox>().center.x -= ro.overlap.x;
                }
                break;
            case ODirection::RIGHT:
                // std::cout << "block player move left\n";
                if (tile->get<CBoundingBox>().blockMove) {
                    p->get<CTransform>().pos.x += ro.overlap.x;
                    p->get<CBoundingBox>().center.x += ro.overlap.x;
                }
                break;
            case ODirection::NONE:
                break;
        }

        // heart collides npc or player
        if (tile->get<CAnimation>().animation.getName() == "TileHeart") {
            for (const auto &entity: m_entityManager.getEntities()) {
                vec2 overlap = Physics::GetOverlap(tile, entity);
                if ((overlap.x > 0 && overlap.y > 0) && entity->has<CHealth>()) {
                    entity->get<CHealth>().current = entity->get<CHealth>().max;
                    m_game->assets().getSound("SLinkPickupHeart").play();
                    tile->destroy();
                }
            }
        }
    }

    // player and NPCs
    for (const auto &npc: m_entityManager.getEntities("npc")) {
        RectOverlap ro = Physics::AisNearB(npc, p, m_gridSize);

        switch (ro.direction) {
            case ODirection::UP:
                if (npc->get<CTransform>().velocity != vec2(0, 0)) {
                    // std::cout << "enemy backs to up\n";
                    npc->get<CTransform>().pos.y -= ro.overlap.y;
                    npc->get<CBoundingBox>().center.y -= ro.overlap.y;
                }
                break;
            case ODirection::DOWN:
                if (npc->get<CTransform>().velocity != vec2(0, 0)) {
                    // std::cout << "enemy backs to down\n";
                    npc->get<CTransform>().pos.y += ro.overlap.y;
                    npc->get<CBoundingBox>().center.y += ro.overlap.y;
                }
                break;
            case ODirection::LEFT:
                if (npc->get<CTransform>().velocity != vec2(0, 0)) {
                    // std::cout << "enemy backs to left\n";
                    npc->get<CTransform>().pos.x -= ro.overlap.x;
                    npc->get<CBoundingBox>().center.x -= ro.overlap.x;
                }
                break;
            case ODirection::RIGHT:
                if (npc->get<CTransform>().velocity != vec2(0, 0)) {
                    // std::cout << "enemy backs to right\n";
                    npc->get<CTransform>().pos.x += ro.overlap.x;
                    npc->get<CBoundingBox>().center.x += ro.overlap.x;
                }
                break;
            case ODirection::NONE:
                break;
        }

        // damage
        if (ro.direction != ODirection::NONE) {
            if (npc->has<CDamage>() && !p->has<CInvincibility>()) {
                p->get<CHealth>().current -= npc->get<CDamage>().damage;
                p->add<CInvincibility>(m_currentFrame);

                // play link damaged sound
                m_game->assets().getSound("SLinkDamaged").play();

                if (p->get<CHealth>().current < 0) {
                    // game over: respawn player or something
                    p->destroy();
                    spawnPlayer();
                    m_game->assets().getSound("SLinkDied").play();
                }
            }
        }

        // npc and sword
        for (const auto &sword: m_entityManager.getEntities("sword")) {
            vec2 overlap = Physics::GetOverlap(sword, npc);

            if (overlap.x >= 0 && overlap.y >= 0 && sword->has<CDamage>()) {
                npc->get<CHealth>().current -= sword->get<CDamage>().damage;

                m_game->assets().getSound("SEnemyDamaged").play();

                sword->remove<CDamage>();
                if (npc->get<CHealth>().current < 0) {
                    npc->destroy();
                    m_game->assets().getSound("SEnemyDied").play();
                    break;
                }
            }
        }

        // npc and tiles
        for (const auto &tile: m_entityManager.getEntities("tile")) {
            ro = Physics::AisNearB(npc, tile, m_gridSize);

            switch (ro.direction) {
                case ODirection::UP:
                    if (npc->get<CTransform>().velocity != vec2(0, 0)) {
                        if (tile->get<CBoundingBox>().blockMove) {
                            // std::cout << "enemy backs to up\n";
                            npc->get<CTransform>().pos.y -= ro.overlap.y;
                            npc->get<CBoundingBox>().center.y -= ro.overlap.y;
                        }
                    }
                    break;
                case ODirection::DOWN:
                    if (npc->get<CTransform>().velocity != vec2(0, 0)) {
                        if (tile->get<CBoundingBox>().blockMove) {
                            // std::cout << "enemy backs to down\n";
                            npc->get<CTransform>().pos.y += ro.overlap.y;
                            npc->get<CBoundingBox>().center.y += ro.overlap.y;
                        }
                    }
                    break;
                case ODirection::LEFT:
                    if (npc->get<CTransform>().velocity != vec2(0, 0)) {
                        if (tile->get<CBoundingBox>().blockMove) {
                            // std::cout << "enemy backs to left\n";
                            npc->get<CTransform>().pos.x -= ro.overlap.x;
                            npc->get<CBoundingBox>().center.x -= ro.overlap.x;
                        }
                    }
                    break;
                case ODirection::RIGHT:
                    if (npc->get<CTransform>().velocity != vec2(0, 0)) {
                        if (tile->get<CBoundingBox>().blockMove) {
                            // std::cout << "enemy backs to right\n";
                            npc->get<CTransform>().pos.x += ro.overlap.x;
                            npc->get<CBoundingBox>().center.x += ro.overlap.x;
                        }
                    }
                    break;
                case ODirection::NONE:
                    break;
            }
        }
    }
}

void Scene_Zelda::animatePlayer() {
    auto p = player();
    if (!p) return;

    if (p->get<CInput>().attack) {
        changePlayerStateTo("attack", p->get<CTransform>().facing);

    } else if (p->get<CTransform>().velocity.x > 0) { // face right
        p->get<CTransform>().facing = vec2(1, 0);
        changePlayerStateTo("move", p->get<CTransform>().facing);
        p->get<CTransform>().scale.x = 1;

    } else if (p->get<CTransform>().velocity.x < 0) { // face left
        p->get<CTransform>().facing = vec2(-1, 0);
        changePlayerStateTo("move", p->get<CTransform>().facing);
        p->get<CTransform>().scale.x = -1;

    } else if (p->get<CTransform>().velocity.y < 0) { // face up
        p->get<CTransform>().facing = vec2(0, -1);
        changePlayerStateTo("move", p->get<CTransform>().facing);

    } else if (p->get<CTransform>().velocity.y > 0) { // face down
        p->get<CTransform>().facing = vec2(0, 1);
        changePlayerStateTo("move", p->get<CTransform>().facing);

    } else if (p->get<CTransform>().velocity.x == 0 && p->get<CTransform>().velocity.y == 0) { // stand
        changePlayerStateTo("stand", p->get<CTransform>().facing);
    }

    if (p->get<CState>().changeAnimation) {
        std::string s = p->get<CState>().state;
        if (s == "stand_down") {
            p->add<CAnimation>(m_game->assets().getAnimation("LinkStandDown"), true);
        } else if (s == "stand_up") {
            p->add<CAnimation>(m_game->assets().getAnimation("LinkStandUp"), true);
        } else if (s == "stand_right" || s == "stand_left") {
            p->add<CAnimation>(m_game->assets().getAnimation("LinkStandRight"), true);
        } else if (s == "move_down") {
            p->add<CAnimation>(m_game->assets().getAnimation("LinkMoveDown"), true);
        } else if (s == "move_up") {
            p->add<CAnimation>(m_game->assets().getAnimation("LinkMoveUp"), true);
        } else if (s == "move_right" || s == "move_left") {
            p->add<CAnimation>(m_game->assets().getAnimation("LinkMoveRight"), true);
        } else if (s == "attack_down") {
            p->add<CAnimation>(m_game->assets().getAnimation("LinkAtkDown"), true);
        } else if (s == "attack_up") {
            p->add<CAnimation>(m_game->assets().getAnimation("LinkAtkUp"), true);
        } else if (s == "attack_right" || s == "attack_left") {
            p->add<CAnimation>(m_game->assets().getAnimation("LinkAtkRight"), true);
        }
    }
}

void Scene_Zelda::sAnimation() {
    // Implement player facing direction animation
    // Implement sword animation based on player facing
    // The sword should move if the player changes direction mid-swing
    // Implement destruction of entities with non-repeating finished animations

    animatePlayer();

    for (const auto &entity: m_entityManager.getEntities()) {
        if (entity->has<CAnimation>()) {
            entity->get<CAnimation>().animation.update();
        }
    }
}

void Scene_Zelda::sCamera() {
    // Implement camera view logic
    // get the current view, which we will modify in the if-statement below
    sf::View view = m_game->window().getView();

    auto p = player();
    if (!p) return;
    auto &pPos = p->get<CTransform>().pos;
    if (m_follow) {
        // calculate view for player follow camera
        view.setCenter(pPos.x, pPos.y);
    } else {
        // calculate view for room-based camera
        vec2 r = getRoomXY(pPos);
        view.setCenter(
                r.x * width() + width() / 2.0f,
                r.y * height() + height() / 2.0f
        );
    }

    // then set the window view
    m_game->window().setView(view);
}

void Scene_Zelda::onEnd() {
    // When the scene ends, change back to the MENU scene
    // Stop the music
    // Play the menu music
    // Use m_game->changeScene(correct params);
    m_game->changeScene("MENU", std::make_shared<Scene_Menu>(m_game));
}

void Scene_Zelda::sRender() {
    m_game->window().clear(sf::Color(255, 192, 122));
    sf::RectangleShape tick({1.0f, 6.0f});
    tick.setFillColor(sf::Color::Black);

    // draw all Entity textures / animations
    if (m_drawTextures) {
        for (const auto &entity: m_entityManager.getEntities()) {
            auto &transform = entity->get<CTransform>();
            sf::Color c = sf::Color::White;
            if (entity->has<CInvincibility>()) {
                c = sf::Color(255, 255, 255, 128);
            }
            if (entity->has<CAnimation>()) {
                auto &animation = entity->get<CAnimation>().animation;
                animation.getSprite().setRotation(transform.angle);
                animation.getSprite().setPosition(
                        transform.pos.x, transform.pos.y
                );
                animation.getSprite().setScale(
                        transform.scale.x, transform.scale.y
                );
                animation.getSprite().setColor(c);
                m_game->window().draw(animation.getSprite());
            }
        }

        for (const auto &entity: m_entityManager.getEntities()) {
            auto &transform = entity->get<CTransform>();
            if (entity->has<CHealth>()) {
                auto &h = entity->get<CHealth>();
                vec2 size(64, 6);
                sf::RectangleShape rect({size.x, size.y});
                rect.setPosition(
                        transform.pos.x - 32,
                        transform.pos.y - 48
                );
                rect.setFillColor(sf::Color(96, 96, 96));
                rect.setOutlineColor(sf::Color::Black);
                rect.setOutlineThickness(2);
                m_game->window().draw(rect);

                float ratio = (float) (h.current) / h.max;
                size.x *= ratio;
                rect.setSize({size.x, size.y});
                rect.setFillColor(sf::Color(255, 0, 0));
                rect.setOutlineThickness(0);
                m_game->window().draw(rect);

                for (int i = 0; i < h.max; i++) {
                    tick.setPosition(
                            rect.getPosition() + sf::Vector2f(i * 64 * 1.0 / h.max, 0)
                    );
                    m_game->window().draw(tick);
                }
            }
        }
    }

    // draw all Entity collision bounding boxes with a rectangle shape
    if (m_drawCollision) {
        // draw bounding box
        sf::CircleShape dot(4);
        for (const auto &entity: m_entityManager.getEntities()) {
            if (entity->has<CBoundingBox>()) {
                auto &box = entity->get<CBoundingBox>();
                sf::RectangleShape rect;
                rect.setSize(sf::Vector2f(box.size.x - 1, box.size.y - 1));
                rect.setOrigin(sf::Vector2f(box.halfSize.x, box.halfSize.y));
                rect.setPosition(box.center.x, box.center.y);
                rect.setFillColor(sf::Color(0, 0, 0, 0));
                if (box.blockMove && box.blockVision) {
                    rect.setOutlineColor(sf::Color::Black);
                }
                if (box.blockMove && !box.blockVision) {
                    rect.setOutlineColor(sf::Color::Blue);
                }
                if (!box.blockMove && box.blockVision) {
                    rect.setOutlineColor(sf::Color::Red);
                }
                if (!box.blockMove && !box.blockVision) {
                    rect.setOutlineColor(sf::Color::White);
                }
                rect.setOutlineThickness(1);
                m_game->window().draw(rect);

                // draw line between player and npc
                if (!player()) continue;
                if (entity->tag() == "npc") {
                    auto &ePos = entity->get<CTransform>().pos;
                    auto view = m_game->window().getView().getCenter();
                    if (ePos.x >= view.x - (float) width() / 2.0 &&
                        ePos.x <= view.x + (float) width() / 2.0 &&
                        ePos.y >= view.y - (float) height() / 2.0 &&
                        ePos.y <= view.y + (float) height() / 2.0
                            ) {
                        drawLine(
                                player()->get<CTransform>().pos,
                                entity->get<CTransform>().pos
                        );
                    }
                }
            }

            // draw patrol points
            if (entity->has<CFollowPlayer>()) {
                auto &h = entity->get<CFollowPlayer>().home;
                dot.setPosition(h.x, h.y);
                m_game->window().draw(dot);
            }
            if (entity->has<CPatrol>()) {
                for (auto p: entity->get<CPatrol>().positions) {
                    vec2 r = getRoomXY(entity->get<CTransform>().pos);
                    vec2 pos = getPosition(r.x, r.y, p.x, p.y);
                    dot.setPosition(pos.x, pos.y);
                    m_game->window().draw(dot);
                }
            }
        }
    }

    // draw the grid so that can easily debug
    if (m_drawGrid) {
        float leftX = m_game->window().getView().getCenter().x - width() / 2.0;
        float rightX = leftX + width() + m_gridSize.x;
        float nextGridX = leftX - ((int) leftX % (int) m_gridSize.x);
        float topY = m_game->window().getView().getCenter().y - height() / 2.0;
        float bottomY = topY + height() + m_gridSize.y;
        float nextGridY = topY - ((int) topY % (int) m_gridSize.x);

        // draw room coordinate
        auto p = player();
        if (p) {
            vec2 r = getRoomXY(p->get<CTransform>().pos);
            m_gridText.setString(
                    "room \n" + std::to_string((int) r.x) + " " +
                    std::to_string((int) r.y)
            );
            m_gridText.setPosition(
                    leftX + m_gridSize.x + 3,
                    topY + m_gridSize.y / 2
            );
            m_game->window().draw(m_gridText);
        }

        for (float x = nextGridX; x < rightX; x += m_gridSize.x) {
            drawLine(vec2(x, topY), vec2(x, bottomY));
        }

        for (float y = nextGridY; y < bottomY; y += m_gridSize.y) {
            drawLine(vec2(leftX, y), vec2(rightX, y));

            for (float x = nextGridX; x < rightX; x += m_gridSize.x) {
                int w = width();
                int h = height();
                std::string xCell = std::to_string(
                        ((((int) x % w) + w) % w) / (int) m_gridSize.x
                );
                std::string yCell = std::to_string(
                        ((((int) y % h) + h) % h) / (int) m_gridSize.y
                );
                m_gridText.setString("(" + xCell + "," + yCell + ")");
                m_gridText.setPosition(x + 3, y + 2);
                m_game->window().draw(m_gridText);
            }
        }
    }
}

std::shared_ptr<Entity> Scene_Zelda::player() {
    for (auto e: m_entityManager.getEntities("player")) {
        return e;
    }
    return nullptr;
}

void Scene_Zelda::changePlayerStateTo(const std::string &state, const vec2 &facing) {
    auto p = player();
    if (!p) return;

    std::string newState;

    if (state == "stand") {
        if (facing == vec2(1, 0)) {
            newState = "stand_right";
        } else if (facing == vec2(-1, 0)) {
            newState = "stand_left";
        } else if (facing == vec2(0, -1)) {
            newState = "stand_up";
        } else if (facing == vec2(0, 1)) {
            newState = "stand_down";
        }
    }

    if (state == "move") {
        if (facing == vec2(1, 0)) {
            newState = "move_right";
        } else if (facing == vec2(-1, 0)) {
            newState = "move_left";
        } else if (facing == vec2(0, -1)) {
            newState = "move_up";
        } else if (facing == vec2(0, 1)) {
            newState = "move_down";
        }
    }

//    vec2 offset = p->get<CBoundingBox>().center;
    if (state == "attack") {
        if (facing == vec2(1, 0)) {
            newState = "attack_right";
//            offset.x -= 10;
        } else if (facing == vec2(-1, 0)) {
            newState = "attack_left";
//            offset.x += 10;
        } else if (facing == vec2(0, -1)) {
            newState = "attack_up";
//            offset.y += 10;
        } else if (facing == vec2(0, 1)) {
            newState = "attack_down";
//            offset.y -= 10;
        }
    }

    if (p->get<CState>().previousState != newState) {
        p->get<CState>().previousState = p->get<CState>().state;
        p->get<CState>().state = newState;
        // change player bounding box for attack animation
//        if (state == "attack") {
//            p->get<CBoundingBox>().center = offset;
//        } else {
//            p->get<CBoundingBox>().center = p->get<CTransform>().pos;
//        }
        p->get<CState>().changeAnimation = true;
    } else {
        p->get<CState>().changeAnimation = false;
    }
}

vec2 Scene_Zelda::windowToWorld(const vec2 &pos) {
    auto view = m_game->window().getView();
    float wx = view.getCenter().x - width() / 2.0f;
    float wy = view.getCenter().y - height() / 2.0f;
    return {pos.x + wx, pos.y + wy};
}

void Scene_Zelda::sDrag() {
    for (const auto &entity: m_entityManager.getEntities()) {
        if (entity->has<CDraggable>() && entity->get<CDraggable>().dragging) {
            vec2 wPos = windowToWorld(m_mousePos);
            entity->get<CTransform>().pos = wPos;
            entity->get<CBoundingBox>().center = wPos;
        }
    }
}

vec2 Scene_Zelda::getRoomXY(const vec2 &pos) {
    auto winSize = m_game->window().getSize();
    int roomX = static_cast<int>(pos.x) / static_cast<int>(winSize.x);
    int roomY = static_cast<int>(pos.y) / static_cast<int>(winSize.y);
    if (pos.x < 0) roomX--;
    if (pos.y < 0) roomY--;
    return {(float) roomX, (float) roomY};
}
