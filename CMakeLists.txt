cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

project(space_invaders)

find_package(GTest 1.14.0 CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

find_package(glm CONFIG REQUIRED)

add_executable(space_invaders 
    src/app/app.cpp
    src/game/game.cpp
    src/input/input.cpp
    src/config/config.cpp
    src/managers/buffer_manager.cpp
    src/managers/glew_manager.cpp
    src/managers/glfw_manager.cpp
    src/managers/shader_manager.cpp
    src/render/renderer.cpp
    src/player/player.cpp
    src/enemy/enemy.cpp
    src/game_object/game_object.cpp
    src/collision_detection/collision_detection.cpp
)

target_link_libraries(space_invaders PRIVATE fmt::fmt)
target_link_libraries(space_invaders PRIVATE GLEW::GLEW OpenGL::GL glfw glm::glm)

enable_testing()

add_executable(tests
    tests/main.cpp
    tests/collision_detection_test.cpp
    src/game/game.cpp
    src/input/input.cpp
    src/config/config.cpp
    src/managers/buffer_manager.cpp
    src/managers/glew_manager.cpp
    src/managers/glfw_manager.cpp
    src/managers/shader_manager.cpp
    src/render/renderer.cpp
    src/player/player.cpp
    src/enemy/enemy.cpp
    src/game_object/game_object.cpp
    src/collision_detection/collision_detection.cpp
)

target_link_libraries(tests PRIVATE GTest::gtest GTest::gtest_main fmt::fmt)
target_link_libraries(tests PRIVATE GLEW::GLEW OpenGL::GL glfw glm::glm)

# add all tests to one test
add_test(NAME all_tests COMMAND tests)