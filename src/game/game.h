#pragma once

#include <GL/glew.h>
#include <fmt/core.h>

#include <list>
#include <random>
#include <thread>
#include <vector>

#include "../timer/timer.h"
#include "../input/input.h"
#include "../managers/glew_manager.h"
#include "../managers/glfw_manager.h"
#include "../managers/shader_manager.h"
#include "../render/renderer.h"
#include "../game_object/game_object.h"
#include "../player/player.h"
#include "../enemy/enemy.h"
#include "../collision_detection/collision_detection.h"

struct GameState {};

class Game {
   public:
    Game();
    void run();
    void cleanup();

    std::vector<std::vector<float>> getVertices();
    float getRandomNumber(float min, float max);

    void updatePlayer(float deltaTime);
    void updateEnemies(float deltaTime);
    void updateProjectiles(float deltaTime);

    void spawnEnemy();
    void spawnProjectile();

   private:
    GameState m_gameState;
    std::vector<std::vector<float>> m_vertices;
    GLFWwindow* m_window;
    GlfwManager m_glfwManager;
    GlewManager m_glewManager;

    Input* m_input;
    CollisionDetection m_collisionDetection;
    Renderer* m_renderer;

    Player* m_player;
    std::list<Enemy> m_enemies;
    std::list<GameObject> m_projectiles;

};