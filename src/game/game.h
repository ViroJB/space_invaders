#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fmt/core.h>

#include <list>
#include <random>
#include <thread>
#include <vector>

#include "../input/input.h"
#include "../managers/buffer_manager.h"
#include "../managers/glew_manager.h"
#include "../managers/glfw_manager.h"
#include "../managers/shader_manager.h"
#include "../render/renderer.h"
#include "../game_object/game_object.h"
#include "../player/player.h"
#include "../enemy/enemy.h"

struct GameState {};

class Game {
   public:
    Game();
    void run();
    void cleanup();
    void updateEnemies(float deltaTime);
    void updateProjectiles(float deltaTime);
    void spawnEnemy();
    void spawnProjectile();
    void checkForBoxCollisions();
    bool checkForPreciseCollision(GameObject& object1, GameObject& object2);

    std::vector<glm::vec2> transformVertices(const std::vector<float>& vertices, const glm::mat4& transform);
    bool checkForSATCollision(const std::vector<glm::vec2>& vertices1, const std::vector<glm::vec2>& vertices2);
    bool overlapOnAxis(const std::vector<glm::vec2>& vertices1, const std::vector<glm::vec2>& vertices2, const glm::vec2& axis);

    void updatePlayer(float deltaTime);

   private:
    GameState m_gameState;
    Input* m_input;
    GLFWwindow* m_window;
    GlfwManager m_glfwManager;
    GlewManager m_glewManager;
    Renderer* m_renderer;
    Player* m_player;
    std::list<Enemy> m_enemies;
    std::list<GameObject> m_projectiles;

    std::vector<std::vector<float>> m_vertices;
};