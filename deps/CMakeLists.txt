add_library(sfml-imgui
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui-SFML.cpp
    imgui/imgui.cpp
    )
target_link_libraries(sfml-imgui PRIVATE sfml-graphics)
target_include_directories(sfml-imgui PUBLIC imgui)

# imgui/imconfig-SFML.h
# imgui/imconfig.h
# imgui/imgui_demo.cpp
# imgui/imgui_draw.cpp
# imgui/imgui_internal.h
# imgui/imgui_tables.cpp
# imgui/imgui_widgets.cpp
# imgui/imgui-SFML_export.h
# imgui/imgui-SFML.cpp
# imgui/imgui-SFML.h
# imgui/imgui.cpp
# imgui/imgui.h
# imgui/imstb_rectpack.h
# imgui/imstb_textedit.h
# imgui/imstb_truetype.h
