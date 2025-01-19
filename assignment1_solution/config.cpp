#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

#include "config.h"


Config::Config(const std::string &file)
        : window{640, 480}, font{} {
    std::ifstream infile(file);

    std::string line;
    while (std::getline(infile, line))
    {
        std::istringstream iss(line);

        std::string type;
        if (!(iss >> type))
        {
            std::cerr << "Error reading configuration type.\n";
            break;
        }

        if (type == "Window")
        {
            if (!(iss >> window.width >> window.height))
            {
                std::cerr << "Error reading Window config.\n";
                break;
            }
        }
        else if (type == "Font")
        {
            if (!(iss >> font.file >> font.size >> font.R >> font.G >> font.B))
            {
                std::cerr << "Error reading Font config.\n";
                break;
            }
        }
        else if (type == "Rectangle")
        {
            ShapeConfig shape;
            shape.type = "rectangle";
            if (!(iss >> shape.name >> shape.posX >> shape.posY >> shape.speedX >> shape.speedY
                      >> shape.R >> shape.G >> shape.B >> shape.sizeX >> shape.sizeY))
            {
                std::cerr << "Error reading Rectangle config.\n";
                break;
            }
            shapes.push_back(shape);
        }
        else if (type == "Circle")
        {
            ShapeConfig shape;
            shape.type = "circle";
            if (!(iss >> shape.name >> shape.posX >> shape.posY >> shape.speedX >> shape.speedY
                      >> shape.R >> shape.G >> shape.B >> shape.radius))
            {
                std::cerr << "Error reading Circle config.\n";
                break;
            }
            shape.sizeX = 2 * shape.radius;
            shape.sizeY = shape.sizeX;
            shapes.push_back(shape);
        }
        else
        {
            std::cerr << "Unknown config type.\n";
        }
    }
}

std::ostream &operator<<(std::ostream &os, const Config &config) {
    os << "Game configuration:\n";
    os << "-------------------\n";

    os << "Window: " << config.window.width << " x " << config.window.height << "\n";

    os << "Font:\n"
       << "\t- file " << config.font.file << "\n"
       << "\t- size " << config.font.size << "\n"
       << "\t- color (" << config.font.R << ", " << config.font.G << ", " << config.font.B << ")\n";

    for (const auto &shape: config.shapes)
    {
        if (shape.type == "rectangle")
        {
            os << "Rectangle " << shape.name << ":\n"
               << "\t- position (" << shape.posX << ", " << shape.posY << ")\n"
               << "\t- speed (" << shape.speedX << ", " << shape.speedY << ")\n"
               << "\t- color (" << shape.R << shape.G << shape.B << ")\n"
               << "\t- size (" << shape.sizeX << ", " << shape.sizeY << ")\n";
        }
        else if (shape.type == "circle")
        {
            os << "Circle " << shape.name << ":\n"
               << "\t- position (" << shape.posX << ", " << shape.posY << ")\n"
               << "\t- speed (" << shape.speedX << ", " << shape.speedY << ")\n"
               << "\t- color (" << shape.R << shape.G << shape.B << ")\n"
               << "\t- radius " << shape.sizeX << "\n";
        }
    }

    return os;
}
