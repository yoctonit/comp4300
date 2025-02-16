file(COPY
        fonts/tech.ttf
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/fonts)
file(COPY
        config.txt
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(comp4300a2 main.cpp Entity.cpp EntityManager.cpp Game.cpp Vec2.cpp)
target_link_libraries(comp4300a2 PRIVATE sfml-graphics)
target_link_libraries(comp4300a2 PRIVATE sfml-imgui)
target_compile_features(comp4300a2 PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET comp4300a2
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:comp4300a2>
        VERBATIM)
endif()

install(TARGETS comp4300a2)
