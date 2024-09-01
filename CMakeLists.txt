cmake_minimum_required(VERSION 3.15...3.30)

project(
    Wormgine    VERSION 0.1
                DESCRIPTION "A simple ECS-based game engine."
                LANGUAGES CXX
)


set(CMAKE_CXX_STANDARD 20)
include(FetchContent)


FetchContent_Declare(
  entt
  GIT_REPOSITORY https://github.com/skypjack/entt.git
  GIT_TAG v3.11.0
)
FetchContent_MakeAvailable(entt)


set(SFML_VERSION "2.6.1")
FetchContent_Declare(
  SFML
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG ${SFML_VERSION}
)

# FetchContent_MakeAvailable(SFML)


FetchContent_GetProperties(SFML)
if(NOT SFML_POPULATED)
  FetchContent_Populate(SFML)
  message(SFML_SOURCE_DIR="${SFML_SOURCE_DIR}")
  message(SFML_BINARY_DIR="${SFML_BINARY_DIR}")
  add_subdirectory(${SFML_SOURCE_DIR} ${SFML_BINARY_DIR})
endif()


FetchContent_Declare(
  box2d
  GIT_REPOSITORY https://github.com/erincatto/box2d.git
  GIT_TAG v3.0.0
)

FetchContent_MakeAvailable(box2d)


include_directories("include" "src/engine/" "src/engine/systems" "src/engine/components")

add_executable(Wormgine src/main.cpp
                        src/engine/game.cpp
                        src/engine/systems/renderer.cpp
                        src/engine/systems/event-system.cpp
                        src/engine/systems/physics-system.cpp)

target_link_libraries(Wormgine PRIVATE EnTT::EnTT sfml-audio sfml-graphics sfml-system sfml-window box2d)#${SFML_LIBRARIES}) #  