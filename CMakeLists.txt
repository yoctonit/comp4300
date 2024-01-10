cmake_minimum_required(VERSION 3.16)
project(COMP4300 LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_subdirectory(deps)

add_subdirectory(assignment1)
add_subdirectory(assignment1_solution)

add_subdirectory(assignment2)
