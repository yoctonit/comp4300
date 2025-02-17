
[COMP4300 - Game Programming - Lecture 04 - SFML + ImGui Tutorial](https://youtu.be/cK_LbRWaIOU?si=6pJKobPtrM7bwgXo)
Komentar: Dodao sam Makefile. Tj. prekucao ga sa ekrana. Mada on ne radi ovakav kakav je, jer sam ja koristio cmake u svom podesavanju projekta, tako da putanje nistu iste. Ali sam ga dodao zbog celine i reference.

make -j 8 # compile with parallelism of 8 threads
make run -j 8

SFML
* Simple and Fast Multimedia Library
* Multi-Platform, Multi-Language
* Handles the things we don't want to: Windows, Rendering, Audio, Images, etc...
* Based on OpenGL
	* Can be used with raw OpenGL code
* Super easy to learn and use

ImGui
* Immediate Mode GUI
* Amazing for dev/debugging GUI
	* Can customize, but not super pretty
* Works by directly modifying variables in memory rather than callback functions
* Works very well with Game Loop structure
* Easy to learn and implement

Game Loops
```
// display initial GUI screen
while (true)
{
  // process user input if any exists
  // update internal game simulation
  // render current game screen
}
```

```
// display initial GUI screen
while (true)
{
  processInput();
  update();
  render();
}
```

Widgets
```
ImGui::Checkbox("Draw Shape", &shape.draw);

ImGui::SliderInt("SomeInt", &intVariable, min, max);
ImGui::SliderFloat("Scale", &shape.scale, min, max);

float v[2] = {0.0f, 0.0f};
ImGui::SliderFloat2("Velocity", v, min, max);

float shapeColor[3] = {0.0f, 0.0f, 0.0f};
ImGui::ColorEdit3("Color", shapeColor);

char buffer[255];
ImGui::InputText("Default", buffer, 255);

if (ImGui::Button("Button Text"))
{
    // this happens when button is pressed
}
ImGui::SameLine();
if (ImGui::Button("Button Two"))
{
    // this will be on the same line
}

if (ImGui::BeginTabBar("MyTabBar"))
{
    if (ImGui::BeginTabItem("Avocado"))
    {
        ImGui::Text("This is the Avocado tab!");
        ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Broccoli"))
    {
        ImGui::Text("This is the Broccoli tab!");
        ImGui::EndTabItem();
    }
    ImGui::EndTabBar();
}

// by default, combo uses raw string arrays
const char* items[] = {"AAAA", "BBBB", "CCCC", "DDDD"};
static int item_index = 0;
ImGui::Combo("Combo", &item_index, items, IM_ARRAYSIZE(items));

// you may find it easier to work with vectors
std::vector<const char*> items;
for (auro& s: shapes) { items.push_pack(s.nameString.c_str()); }
int item_index = 0;
ImGui::Combo("Shape", &item_index, items.data(), (int)items.size());
```

ImGui Naming
```
// ImGui elements need to be given unique names, or the last
// one will override the behavior of any other of the same name

if (ImGui::Button("Delete")) { deleteThingOne(); }
if (ImGui::Button("Delete")) { deleteThingTwo(); }

// Can fix this by adding ## to name, which will not display
if (ImGui::Button("Delete##D1")) { deleteThingOne(); }
if (ImGui::Button("Delete##D2")) { deleteThingTwo(); }
```

ImGui Demo/Examples at 38:52.
* https://pthom.github.io/imgui_manual_online/manual/imgui_manual.html

SFML Tutorial at 42:00
