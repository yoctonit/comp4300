#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <vector>


struct WindowConfig {
    int width, height, frameLimit, fullScreen;
};
struct FontConfig {
    std::string file;
    int size;
    int R, G, B; // color
};
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


class Config {
private:
    WindowConfig m_windowConfig;
    FontConfig m_fontConfig{};
    PlayerConfig m_playerConfig{};
    EnemyConfig m_enemyConfig{};
    BulletConfig m_bulletConfig{};

public:
    explicit Config(const std::string &file);

    [[nodiscard]] WindowConfig Window() const;

    [[nodiscard]] FontConfig Font() const;

    [[nodiscard]] PlayerConfig Player() const;

    [[nodiscard]] EnemyConfig Enemy() const;

    [[nodiscard]] BulletConfig Bullet() const;

    void Print() const;
};

#endif //CONFIG_H
