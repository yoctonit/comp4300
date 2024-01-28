#include <iostream>
#include <memory>
#include <fstream>

#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
#include "config.h"
#include "shape.h"
#include "rectangle.h"
#include "circle.h"

int main() {
    Config config("config_a1.txt");

    const int wWidth = config.Window().width;
    const int wHeight = config.Window().height;
    sf::RenderWindow window(sf::VideoMode(wWidth, wHeight), "SFML Works!");
    window.setFramerateLimit(60);

    // initialize IMGUI and create a clock used for its internal timing
    bool imguiLoaded = ImGui::SFML::Init(window);
    if (!imguiLoaded) {
        // if we can't load the ImGui, print an error to the error console and exit
        std::cerr << "Could not load ImGui!\n";
        exit(-1);
    }

    sf::Clock deltaClock;

    // scale the imgui ui by a given factor, does not affect text size
    ImGui::GetStyle().ScaleAllSizes(1.0f);

    // the imgui color {r, g, b} wheel requires floats from 0-1 instead of ints from 0-255
    float c[3] = {0.0f, 1.0f, 1.0f};

    // let's make a shape that we will draw to the screen
    float circleRadius = 50;    // radius to draw the circle
    int circleSegments = 32;    // number of segments to draw the circle with
    float circleSpeedX = 1.0f;  // we will use this to move the circle later
    float circleSpeedY = 0.5f;  // you will read these values from the file
    bool drawCircle = true;     // wheter or not to draw the circle
    bool drawText = true;       // wheter or not to draw the text

    // create the sfml circle shape based on our parameters
    sf::CircleShape circle(circleRadius, circleSegments);   // create a circle shape with radius 50
    circle.setPosition(10.0f, 10.0f);                       // set the top-left position of the circle

    // let's load a font, so we can display some text
    sf::Font myFont;
    FontConfig fontConfig = config.Font();

    // attempt to load the font from a file
    if (!myFont.loadFromFile(fontConfig.file)) {
        // if we can't load the font, print an error to the error console and exit
        std::cerr << "Could not load font!\n";
        exit(-1);
    }

    // set up the text object that will be drawn to the screen
    sf::Text text("Sample Text", myFont, 24);

    // position the top-left corner of the text so that the text aligns on the bottom
    // text character size is in pixels, so move the text up from the bottom by its height
    text.setPosition(0, (float) wHeight - (float) text.getCharacterSize());

    // set up a character array to set the text
    char displayString[255] = "Sample Text";

    std::vector<std::shared_ptr<Shape>> shapes;
    for (const auto &shapeConfig: config.Rectangles()) {
        shapes.push_back(std::make_shared<Rectangle>(shapeConfig, fontConfig, myFont, config.Window()));
    }
    for (const auto &shapeConfig: config.Circles()) {
        shapes.push_back(std::make_shared<Circle>(shapeConfig, fontConfig, myFont, config.Window()));
    }

    // main loop - continues for each frame while window is open
    while (window.isOpen()) {
        // for (auto event = sf::Event{}; window.pollEvent(event);)
        // {
        //     if (event.type == sf::Event::Closed)
        //     {
        //         window.close();
        //     }
        // }

        // event handling
        sf::Event event{};
        while (window.pollEvent(event)) {
            // pass the event to imgui to be parsed
            ImGui::SFML::ProcessEvent(window, event);

            // this event triggers whe the window is closed
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // this event is triggered when a key is pressed
            if (event.type == sf::Event::KeyPressed) {
                // print the key that was pressed to the console
                std::cout << "Key pressed with code = " << event.key.code << "\n";

                // example, what happens when x is pressed
                if (event.key.code == sf::Keyboard::X) {
                    // reverse the x direction of the circle on the screen
                    circleSpeedX *= -1.0f;
                }

                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
            }
        }

        // update imgui for this frame with the time that the last frame took
        ImGui::SFML::Update(window, deltaClock.restart());

//        ImGui::ShowDemoWindow();

        // draw the UI
        ImGui::Begin("Window title");
        ImGui::Text("Select shape");
//        const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO" };
//        static int item_current_idx = 0; // Here we store our selection data as an index.
//        if (ImGui::BeginListBox("Shapes"))
//        {
//            for (int n = 0; n < shapes.size(); n++)
//            {
//                const bool is_selected = (item_current_idx == n);
//                std::string shapeName = shapes[n]->m_text.getString();
//                if (ImGui::Selectable(shapeName.c_str(), is_selected))
//                    item_current_idx = n;
//
//                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
//                if (is_selected)
//                    ImGui::SetItemDefaultFocus();
//            }
//            ImGui::EndListBox();
//        }
//        const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO" };
        static int item_current_idx = 0; // Here we store our selection data as an index.
        if (ImGui::BeginCombo("shape", shapes[item_current_idx]->Name().c_str(), ImGuiComboFlags_PopupAlignLeft)) {
            for (int n = 0; n < shapes.size(); n++) {
                const bool is_selected = (item_current_idx == n);
                std::string name = shapes[n]->Name();
                if (ImGui::Selectable(shapes[n]->Name().c_str(), is_selected))
                    item_current_idx = n;

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        ImGui::Checkbox("Draw Shape", &shapes[item_current_idx]->Visibility());
        ImGui::SliderFloat("Scale", shapes[item_current_idx]->ScaleRef(), 0.0f, 4.0f);
        ImGui::SliderFloat("VelocityX", shapes[item_current_idx]->VelocityXRef(), -8.0f, 8.0f);
        ImGui::SliderFloat("VelocityY", shapes[item_current_idx]->VelocityYRef(), -8.0f, 8.0f);
//        ImGui::SameLine();
//        ImGui::Checkbox("Draw Text", &drawText);
//        ImGui::SliderFloat("Radius", &circleRadius, 0.0f, 300.0f);
//        ImGui::SliderInt("Sides", &circleSegments, 3, 64);
        ImGui::ColorEdit3("Color Shape", shapes[item_current_idx]->Color());
        ImGui::InputText("Name", displayString, 255);
        if (ImGui::Button("Set Text")) {
//            text.setString(displayString);
            shapes[item_current_idx]->SetName(displayString);
        }
//        ImGui::SameLine();
//        if (ImGui::Button("Reset Circle")) {
//            circle.setPosition(0, 0);
//        }
        ImGui::End();

        // set the circle properties, because they may have been updated with the ui
        shapes[item_current_idx]->SetColor();
        circle.setPointCount(circleSegments);
        circle.setRadius(circleRadius);

        // basic animation - move circle if it's still in frame
        circle.setPosition(circle.getPosition().x + circleSpeedX, circle.getPosition().y + circleSpeedY);

        for (auto &shape: shapes) {
            shape->Update();
        }

        // basic rendering function calls
        window.clear();     // clear the window of anything previously drawn
//        if (drawCircle)     // draw the circle if the boolean is true
//        {
//            window.draw(circle);
//        }
//        if (drawText)       // draw the text if the boolean is true
//        {
//            window.draw(text);
//        }
        for (const auto &shape: shapes) {
            shape->Draw(window);
        }
        ImGui::SFML::Render(window);    // draw the ui last so it's on top
        window.display();               // call the window display function
    }

    std::cout << "closed\n";
    return 0;
}