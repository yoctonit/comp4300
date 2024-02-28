#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

#include "config.h"


Config::Config(const std::string &file) : m_windowConfig{640, 480} {
    std::ifstream infile(file);

    if (!infile.is_open()) {
        std::cerr << "Error opening " << file << "\n";
    }

    std::string line;
    while (std::getline(infile, line)) {
        // std::cout << "line: " << line << std::endl;
        std::istringstream iss(line);

        std::string type;
        if (!(iss >> type)) {
            std::cerr << "Error reading type.\n";
            break;
        }
        // std::cout << "type: " << type << std::endl;

        if (type == "Window") {
            if (!(iss >> m_windowConfig.width >> m_windowConfig.height >> m_windowConfig.frameLimit
                      >> m_windowConfig.fullScreen)) {
                std::cerr << "Error reading Window config.\n";
                break;
            }

        } else if (type == "Font") {
            if (!(iss >> m_fontConfig.file >> m_fontConfig.size >> m_fontConfig.R >> m_fontConfig.G
                      >> m_fontConfig.B)) {
                std::cerr << "Error reading Font config.\n";
                break;
            }

        } else if (type == "Player") {
            if (!(iss >> m_playerConfig.SR >> m_playerConfig.CR >> m_playerConfig.S
                      >> m_playerConfig.FR >> m_playerConfig.FG >> m_playerConfig.FB
                      >> m_playerConfig.OR >> m_playerConfig.OG >> m_playerConfig.OB
                      >> m_playerConfig.OT >> m_playerConfig.V)) {
                std::cerr << "Error reading Player config.\n";
                break;
            }

        } else if (type == "Enemy") {
            if (!(iss >> m_enemyConfig.SR >> m_enemyConfig.CR
                      >> m_enemyConfig.SMIN >> m_enemyConfig.SMAX
                      >> m_enemyConfig.OR >> m_enemyConfig.OG >> m_enemyConfig.OB >> m_enemyConfig.OT
                      >> m_enemyConfig.VMIN >> m_enemyConfig.VMAX
                      >> m_enemyConfig.L >> m_enemyConfig.SI)) {
                std::cerr << "Error reading Enemy config.\n";
                break;
            }

        } else if (type == "Bullet") {
            if (!(iss >> m_bulletConfig.SR >> m_bulletConfig.CR >> m_bulletConfig.S
                      >> m_bulletConfig.FR >> m_bulletConfig.FG >> m_bulletConfig.FB
                      >> m_bulletConfig.OR >> m_bulletConfig.OG >> m_bulletConfig.OB >> m_bulletConfig.OT
                      >> m_bulletConfig.V >> m_bulletConfig.L)) {
                std::cerr << "Error reading Bullet config.\n";
                break;
            }

        } else {
            std::cerr << "Unknown type " + type + ".\n";
        }
    }
}

WindowConfig Config::Window() const {
    return m_windowConfig;
}

FontConfig Config::Font() const {
    return m_fontConfig;
}

PlayerConfig Config::Player() const {
    return m_playerConfig;
}

EnemyConfig Config::Enemy() const {
    return m_enemyConfig;
}

BulletConfig Config::Bullet() const {
    return m_bulletConfig;
}

void Config::Print() const {
    std::cout << "Window Config:\n"
              << "- width " << m_windowConfig.width << "\n"
              << "- height " << m_windowConfig.height << "\n"
              << "- frame limit " << m_windowConfig.frameLimit << "\n"
              << "- full screen " << m_windowConfig.fullScreen << "\n";

    std::cout << "File Config:\n"
              << "- file " << m_fontConfig.file << "\n"
              << "- size " << m_fontConfig.size << "\n"
              << "- color (" << m_fontConfig.R << ", " << m_fontConfig.G << ", " << m_fontConfig.B << ")\n";

    std::cout << "Player Config:\n"
              << "- radius " << m_playerConfig.SR << "\n"
              << "- collision radius " << m_playerConfig.CR << "\n"
              << "- speed " << m_playerConfig.S << "\n"
              << "- color (" << m_playerConfig.FR << ", " << m_playerConfig.FG << ", " << m_playerConfig.FB << ")\n"
              << "- outline color (" << m_playerConfig.OR << ", " << m_playerConfig.OG << ", " << m_playerConfig.OB
              << ")\n"
              << "- outline thickness " << m_playerConfig.OT << "\n"
              << "- number of vertices " << m_playerConfig.V << "\n";

    std::cout << "Enemy Config:\n"
              << "- radius " << m_enemyConfig.SR << "\n"
              << "- collision radius " << m_enemyConfig.CR << "\n"
              << "- speed range (" << m_enemyConfig.SMIN << ", " << m_enemyConfig.SMAX << ")\n"
              << "- outline color (" << m_enemyConfig.OR << ", " << m_enemyConfig.OG << ", " << m_enemyConfig.OB
              << ")\n"
              << "- outline thickness " << m_enemyConfig.OT << "\n"
              << "- number of vertices range (" << m_enemyConfig.VMIN << ", " << m_enemyConfig.VMAX << ")\n"
              << "- small lifespan " << m_enemyConfig.L << "\n"
              << "- spawn interval " << m_enemyConfig.SI << "\n";

    std::cout << "Bullet Config:\n"
              << "- radius " << m_bulletConfig.SR << "\n"
              << "- collision radius " << m_bulletConfig.CR << "\n"
              << "- speed " << m_bulletConfig.S << "\n"
              << "- color (" << m_bulletConfig.FR << ", " << m_bulletConfig.FG << ", " << m_bulletConfig.FB << ")\n"
              << "- outline color (" << m_bulletConfig.OR << ", " << m_bulletConfig.OG << ", " << m_bulletConfig.OB
              << ")\n"
              << "- outline thickness " << m_bulletConfig.OT << "\n"
              << "- number of vertices " << m_bulletConfig.V << "\n"
              << "- lifespan " << m_bulletConfig.L << "\n";
}
