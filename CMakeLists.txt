cmake_minimum_required(VERSION 3.16)
project(COMP4300 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
add_subdirectory(assignment2_solution)

add_subdirectory(assignment3)
add_subdirectory(assignment3_solution)

add_subdirectory(assignment4)
