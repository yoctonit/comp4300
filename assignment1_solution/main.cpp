#include <iostream>
#include <memory>
#include <fstream>

#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"

#include "config.h"
#include "shape.h"

int main() {
    Config config("config_a1.txt");
    // std::cout << config;

    const auto windowWidth = static_cast<float>(config.window.width);
    const auto windowHeight = static_cast<float>(config.window.height);
    sf::RenderWindow window(sf::VideoMode(config.window.width, config.window.height), "Assignment 1");
    window.setFramerateLimit(60);

    // initialize IMGUI and create a clock used for its internal timing
    bool imguiLoaded = ImGui::SFML::Init(window);
    if (!imguiLoaded)
    {
        // if we can't load the ImGui, print an error to the error console and exit
        std::cerr << "Could not load ImGui!\n";
        exit(-1);
    }

    sf::Clock deltaClock;

    // scale the imgui ui and font size by a given factor
    ImGui::GetStyle().ScaleAllSizes(1.0f);
    // ImGui::GetIO().FontGlobalScale = 2.0f;

    // let's load a font, so we can display some text
    sf::Font myFont;

    // attempt to load the font from a file
    if (!myFont.loadFromFile(config.font.file))
    {
        // if we can't load the font, print an error to the error console and exit
        std::cerr << "Could not load font!\n";
        exit(-1);
    }

    // set up the text object that will be drawn to the screen
    sf::Text text("Sample Text", myFont, config.font.size);

    // let's make a shape types that we will draw to the screen
    sf::CircleShape circle(10.0f, 32);
    sf::RectangleShape rectangle;

    std::vector<Shape> shapes;
    for (const auto &shapeConfig: config.shapes)
    {
        shapes.emplace_back(
                shapeConfig.type, shapeConfig.name,
                shapeConfig.posX, shapeConfig.posY,
                shapeConfig.speedX, shapeConfig.speedY,
                shapeConfig.R, shapeConfig.G, shapeConfig.B,
                shapeConfig.sizeX, shapeConfig.sizeY, shapeConfig.radius
        );
    }

    bool shapeVisible = shapes[0].IsVisible();
    float shapeScale = shapes[0].GetScale();
    float shapeVelocityX = shapes[0].m_speedX;
    float shapeVelocityY = shapes[0].m_speedY;
    // the imgui color {r, g, b} wheel requires floats from 0-1 instead of ints from 0-255
    float shapeColor[3] = {
            static_cast<float>(shapes[0].m_R) / 255.0f,
            static_cast<float>(shapes[0].m_G) / 255.0f,
            static_cast<float>(shapes[0].m_B) / 255.0f,
    };
    // set up a character array to set the name of the shape
    char shapeName[255];
    strcpy(shapeName, shapes[0].Name().c_str());

    // main loop - continues for each frame while window is open
    while (window.isOpen())
    {
        sf::Event event{};
        while (window.pollEvent(event))
        {
            // pass the event to imgui to be parsed
            ImGui::SFML::ProcessEvent(window, event);

            // this event triggers whe the window is closed
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            // this event is triggered when a key is pressed
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    window.close();
                }
            }
        }

        // update imgui for this frame with the time that the last frame took
        ImGui::SFML::Update(window, deltaClock.restart());

        // draw the UI
        ImGui::Begin("Edit shapes");

        static int item_current_idx = 0; // Here we store our selection data as an index.
        if (ImGui::BeginCombo("Shape", shapes[item_current_idx].Name().c_str(), ImGuiComboFlags_PopupAlignLeft))
        {
            for (int n = 0; n < shapes.size(); n++)
            {
                const bool is_selected = (item_current_idx == n);
                std::string name = shapes[n].Name();
                if (ImGui::Selectable(shapes[n].Name().c_str(), is_selected))
                {
                    item_current_idx = n;
                    shapeVisible = shapes[n].IsVisible();
                    shapeScale = shapes[n].GetScale();
                    shapeVelocityX = shapes[n].m_speedX;
                    shapeVelocityY = shapes[n].m_speedY;
                    shapeColor[0] = static_cast<float>(shapes[n].m_R) / 255.0f;
                    shapeColor[1] = static_cast<float>(shapes[n].m_G) / 255.0f;
                    shapeColor[2] = static_cast<float>(shapes[n].m_B) / 255.0f;
                    strcpy(shapeName, shapes[n].Name().c_str());
                }

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
        ImGui::Checkbox("Draw Shape", &shapeVisible);
        ImGui::SliderFloat("Scale", &shapeScale, 0.0f, 4.0f);
        ImGui::SliderFloat("VelocityX", &shapeVelocityX, -8.0f, 8.0f);
        ImGui::SliderFloat("VelocityY", &shapeVelocityY, -8.0f, 8.0f);
        ImGui::ColorEdit3("Color Circle", shapeColor);
        ImGui::InputText("Name", shapeName, 255);
        ImGui::SameLine();
        if (ImGui::Button("Set name"))
        {
            shapes[item_current_idx].SetName(shapeName);
        }
        ImGui::End();

        // Update shapes
        //--------------
        shapes[item_current_idx].SetVisible(shapeVisible);
        shapes[item_current_idx].SetScale(shapeScale);
        shapes[item_current_idx].m_speedX = shapeVelocityX;
        shapes[item_current_idx].m_speedY = shapeVelocityY;
        shapes[item_current_idx].m_R = static_cast<int>(shapeColor[0] * 255.0f);
        shapes[item_current_idx].m_G = static_cast<int>(shapeColor[1] * 255.0f);
        shapes[item_current_idx].m_B = static_cast<int>(shapeColor[2] * 255.0f);

        for (auto &shape: shapes)
        {
            // basic animation - move each shape if it's still in frame
            shape.m_posX += shape.m_speedX;
            shape.m_posY += shape.m_speedY;

            // check right window boundary
            if (shape.m_posX + shape.m_sizeX * shape.GetScale() > windowWidth)
            {
                shape.m_posX = windowWidth - shape.m_sizeX * shape.GetScale();
                shape.m_speedX *= -1.0f;
                if (shape.Name() == shapes[item_current_idx].Name())
                {
                    shapeVelocityX = shape.m_speedX;
                }
            }

            // check left window boundary
            if (shape.m_posX < 0.0f)
            {
                shape.m_posX = 0.0f;
                shape.m_speedX *= -1.0f;
                if (shape.Name() == shapes[item_current_idx].Name())
                {
                    shapeVelocityX = shape.m_speedX;
                }
            }

            // check top window boundary
            if (shape.m_posY < 0.0f)
            {
                shape.m_posY = 0.0f;
                shape.m_speedY *= -1.0f;
                if (shape.Name() == shapes[item_current_idx].Name())
                {
                    shapeVelocityY = shape.m_speedY;
                }
            }

            // check bottom window boundary
            if (shape.m_posY + shape.m_sizeY * shape.GetScale() > windowHeight)
            {
                shape.m_posY = windowHeight - shape.m_sizeY * shape.GetScale();
                shape.m_speedY *= -1.0f;
                if (shape.Name() == shapes[item_current_idx].Name())
                {
                    shapeVelocityY = shape.m_speedY;
                }
            }
        }

        // Draw phase
        //-----------
        // 1. Clear the window of anything previously drawn
        window.clear();
        // 2. Draw shapes
        for (const auto &shape: shapes)
        {
            if (!shape.IsVisible()) continue;
            sf::Color color(shape.m_R, shape.m_G, shape.m_B);

            if (shape.Type() == "rectangle")
            {
                rectangle.setFillColor(color);
                rectangle.setSize(sf::Vector2f(shape.m_sizeX * shape.GetScale(), shape.m_sizeY * shape.GetScale()));
                rectangle.setPosition(shape.m_posX, shape.m_posY);

                window.draw(rectangle);
            }
            else if (shape.Type() == "circle")
            {
                circle.setFillColor(color);
                circle.setRadius(shape.m_radius * shape.GetScale());
                circle.setPosition(shape.m_posX, shape.m_posY);

                window.draw(circle);
            }

            // draw shape's name
            text.setString(shape.Name().c_str());
            auto textBounds = text.getLocalBounds();
            text.setOrigin(textBounds.left + textBounds.width / 2.0f,
                           textBounds.top + textBounds.height / 2.0f);
            text.setPosition(sf::Vector2f(shape.m_posX + shape.m_sizeX * shape.GetScale() / 2.0f,
                                          shape.m_posY + shape.m_sizeY * shape.GetScale() / 2.0f));
            window.draw(text);
        }
        // 3. Draw the UI last so it's on top
        ImGui::SFML::Render(window);

        // 4. Switch buffers
        window.display();
    }

    return 0;
}
