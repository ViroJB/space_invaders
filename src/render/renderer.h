#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fmt/core.h>

#include <array>
#include <string>
#include <vector>
#include <list>

#include "../game_object/game_object.h"
#include "../enemy/enemy.h"
#include "../managers/shader_manager.h"

class Renderer {
   public:
    Renderer();
    void initializeVertexData(std::vector<std::vector<float>> vertices);
    void render(GLFWwindow* window, std::list<Enemy>& enemies, GameObject* player, std::list<GameObject>& projectiles);
    void setShaderProgram(GLuint shaderProgram);
    GLuint getShaderProgram();
    // std::array<GLfloat, 21> getCombinedData(GameObject* obj);
    void printCurrentMatrix(GameObject* obj);

   private:
    ShaderManager m_shaderManager = ShaderManager();
    GLuint m_shaderProgram;
    GLuint m_playerVAO, m_playerVBO;
    GLuint m_enemyVAO, m_enemyVBO;
    GLuint m_projectileVAO, m_projectileVBO;
};