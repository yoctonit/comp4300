#ifndef ASSETS_H
#define ASSETS_H

#include <string>
#include <map>

#include "Animation.h"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>


class Assets {
    std::map<std::string, sf::Texture> m_textureMap;
    std::map<std::string, Animation> m_animationMap;
    std::map<std::string, sf::Font> m_fontMap;

public:
    Assets();

    void loadFromFile(const std::string &path);

    void addTexture(const std::string &name, const std::string &path);

    void addAnimation(const std::string &name, const Animation& animation);

    void addFont(const std::string &name, const std::string &path);

    const sf::Texture &getTexture(const std::string &name) const;

    const Animation &getAnimation(const std::string &name) const;

    const sf::Font &getFont(const std::string &name) const;
};

#endif //ASSETS_H
