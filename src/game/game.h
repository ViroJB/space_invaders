#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fmt/core.h>

#include <thread>
#include <vector>
#include <random>

#include "../input/input.h"
#include "../managers/buffer_manager.h"
#include "../managers/glew_manager.h"
#include "../managers/glfw_manager.h"
#include "../managers/shader_manager.h"
#include "../render/renderer.h"


struct GameState {};

class Game {
   public:
    Game();
    void run();
    void cleanup();
    void updateEnemies(float deltaTime);

   private:
    GameState m_gameState;
    Input* m_input;
    GLFWwindow* m_window;
    GlfwManager m_glfwManager;
    GlewManager m_glewManager;
    Renderer* m_renderer;
    GameObject* m_player;
    std::vector<GameObject> m_enemies;
};