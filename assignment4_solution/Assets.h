#ifndef ASSETS_H
#define ASSETS_H

#include <map>
#include <string>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "Animation.h"


class Assets {
    std::map<std::string, sf::Texture> m_textureMap;
    std::map<std::string, Animation> m_animationMap;
    std::map<std::string, sf::Font> m_fontMap;
    std::map<std::string, sf::SoundBuffer> m_soundBuffers;
    std::map<std::string, sf::Sound> m_sounds;

    void addTexture(const std::string &name, const std::string &path);

    void addAnimation(const std::string &name, const Animation &animation);

    void addFont(const std::string &name, const std::string &path);

    void addSound(const std::string &name, const std::string &path);

public:
    Assets();

    ~Assets();

    void loadFromFile(const std::string &path);

    [[nodiscard]] const std::map<std::string, sf::Texture> &getTextures() const;

    [[nodiscard]] const std::map<std::string, Animation> &getAnimations() const;

    [[nodiscard]] std::map<std::string, sf::Sound> &getSounds();

    [[nodiscard]] const sf::Texture &getTexture(const std::string &name) const;

    [[nodiscard]] const Animation &getAnimation(const std::string &name) const;

    [[nodiscard]] const sf::Font &getFont(const std::string &name) const;

    [[nodiscard]] sf::Sound &getSound(const std::string &name);
};

#endif //ASSETS_H
