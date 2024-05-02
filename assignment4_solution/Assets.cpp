#include <cassert>
#include <fstream>
#include <iostream>
#include <SFML/Audio/SoundBuffer.hpp>
#include "Assets.h"


Assets::Assets() = default;

Assets::~Assets() = default;

void Assets::loadFromFile(const std::string &path) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "Could not load " << path << " file!\n";
        exit(-1);
    }

    std::string assetType;
    while (file >> assetType) {
        if (assetType == "Texture") {
            std::string name;
            std::string imageFile;
            file >> name >> imageFile;
            addTexture(name, imageFile);
        } else if (assetType == "Animation") {
            std::string animationName;
            std::string texName;
            int frames, speed;
            file >> animationName >> texName >> frames >> speed;
            const sf::Texture &tex = getTexture(texName);
            addAnimation(animationName, Animation(animationName, tex, frames, speed));
        } else if (assetType == "Font") {
            std::string fontName;
            std::string fontPath;
            file >> fontName >> fontPath;
            addFont(fontName, fontPath);
        } else if (assetType == "Sound") {
            std::string soundName;
            std::string soundPath;
            file >> soundName >> soundPath;
            addSound(soundName, soundPath);
        } else {
            std::cerr << "Incorrect asset type: " << assetType << "\n";
            exit(-1);
        }
    }
}

void Assets::addTexture(const std::string &name, const std::string &path) {
    sf::Texture texture;
    if (!texture.loadFromFile(path)) {
        std::cerr << "Could not load image: " << path << "!\n";
        exit(-1);
    }
    m_textureMap[name] = texture;
}

void Assets::addAnimation(const std::string &name, const Animation &animation) {
    m_animationMap[name] = animation;
}

void Assets::addFont(const std::string &name, const std::string &path) {
    sf::Font font;
    if (!font.loadFromFile(path)) {
        std::cerr << "Could not load font: " << path << "\n";
        exit(-1);
    }
    m_fontMap[name] = font;
}

void Assets::addSound(const std::string &name, const std::string &path) {
    sf::SoundBuffer sb;
    if (!sb.loadFromFile(path)) {
        std::cerr << "Could not load sound: " << path << "\n";
        exit(-1);
    }
    m_soundBuffers[name] = sb;
    m_sounds[name] = sf::Sound(m_soundBuffers.at(name));
}

const sf::Texture &Assets::getTexture(const std::string &name) const {
    assert(m_textureMap.find(name) != m_textureMap.end());
    return m_textureMap.at(name);
}

const Animation &Assets::getAnimation(const std::string &name) const {
    assert(m_animationMap.find(name) != m_animationMap.end());
    return m_animationMap.at(name);
}

const sf::Font &Assets::getFont(const std::string &name) const {
    assert(m_fontMap.find(name) != m_fontMap.end());
    return m_fontMap.at(name);
}

sf::Sound &Assets::getSound(const std::string &name) {
    assert(m_sounds.find(name) != m_sounds.end());
    return m_sounds.at(name);
}

const std::map<std::string, sf::Texture> &Assets::getTextures() const {
    return m_textureMap;
}

const std::map<std::string, Animation> &Assets::getAnimations() const {
    return m_animationMap;
}

std::map<std::string, sf::Sound> &Assets::getSounds() {
    return m_sounds;
}
