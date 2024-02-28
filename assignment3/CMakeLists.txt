add_executable(comp4300a3 main.cpp
        Action.cpp Animation.cpp Assets.cpp
        Entity.cpp EntityManager.cpp GameEngine.cpp
        Physics.cpp Scene.cpp Scene_Menu.cpp Scene_Play.cpp Vec2.cpp)
target_link_libraries(comp4300a3 PRIVATE sfml-graphics)
target_compile_features(comp4300a3 PRIVATE cxx_std_17)

if (WIN32)
    add_custom_command(
            TARGET comp4300a3
            COMMENT "Copy OpenAL DLL"
            PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:comp4300a3>
            VERBATIM)
endif ()

install(TARGETS comp4300a3)
