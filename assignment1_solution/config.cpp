#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

#include "config.h"


Config::Config(const std::string &file)
        : m_windowConfig{640, 480}, m_fontConfig{} {
    std::ifstream infile(file);

    std::string line;
    while (std::getline(infile, line)) {
        // std::cout << "line: " << line << std::endl;

        std::istringstream iss(line);

        std::string type;
        if (!(iss >> type)) {
            std::cerr << "Error reading type.\n";
            break;
        }
//        std::cout << "type: " << type << std::endl;

        if (type == "Window") {
            if (!(iss >> m_windowConfig.width >> m_windowConfig.height)) {
                std::cerr << "Error reading Window config.\n";
                break;
            }

        } else if (type == "Font") {
            if (!(iss >> m_fontConfig.file >> m_fontConfig.size >> m_fontConfig.R >> m_fontConfig.G
                      >> m_fontConfig.B)) {
                std::cerr << "Error reading Font config.\n";
                break;
            }

        } else if (type == "Rectangle") {
            RectangleConfig rectangle;
            if (!(iss >> rectangle.name >> rectangle.posX >> rectangle.posY >> rectangle.speedX >> rectangle.speedY
                      >> rectangle.R >> rectangle.G >> rectangle.B >> rectangle.sizeX >> rectangle.sizeY)) {
                std::cerr << "Error reading Rectangle config.\n";
                break;
            }
            m_rectangles.push_back(rectangle);

        } else if (type == "Circle") {
            CircleConfig circle;
            if (!(iss >> circle.name >> circle.posX >> circle.posY >> circle.speedX >> circle.speedY
                      >> circle.R >> circle.G >> circle.B >> circle.radius)) {
                std::cerr << "Error reading Circle config.\n";
                break;
            }
            m_circles.push_back(circle);

        } else {
            std::cerr << "Unknown type.\n";
        }
    }
}

WindowConfig Config::Window() const {
    return m_windowConfig;
}

FontConfig Config::Font() const {
    return m_fontConfig;
}

const std::vector<RectangleConfig> &Config::Rectangles() const {
    return m_rectangles;
}

const std::vector<CircleConfig> &Config::Circles() const {
    return m_circles;
}

void Config::Print() const {
    std::cout << m_windowConfig.width << ", " << m_windowConfig.height << "\n";

    std::cout << m_fontConfig.file << ", " << m_fontConfig.size << ", ("
              << m_fontConfig.R << ", " << m_fontConfig.G << ", " << m_fontConfig.B << ")\n";

    for (const auto &r: m_rectangles) {
        std::cout << r.name << r.posX << r.posY << r.speedX << r.speedY << r.R << r.G << r.B << r.sizeX << r.sizeY
                  << "\n";
    }
    for (const auto &c: m_circles) {
        std::cout << c.name << c.posX << c.posY << c.speedX << c.speedY << c.R << c.G << c.B << c.radius << "\n";
    }
}