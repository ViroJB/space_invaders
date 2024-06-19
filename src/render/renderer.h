#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fmt/core.h>

#include <array>
#include <string>
#include <vector>

#include "../game_object/game_object.h"
#include "../managers/buffer_manager.h"
#include "../managers/shader_manager.h"

class Renderer {
   public:
    Renderer();
    void initializeVertexData();
    void render(GLFWwindow* window, std::vector<GameObject>& enemies, GameObject* player);
    void setShaderProgram(GLuint shaderProgram);
    GLuint getShaderProgram();
    std::array<GLfloat, 21> getCombinedData(GameObject* obj);
    void printCurrentMatrix(GameObject* obj);

   private:
    ShaderManager m_shaderManager = ShaderManager();
    GLuint m_shaderProgram;
    GameObject m_gameObject = GameObject(0.1f, 0.1f, {0.0f, 1.0f, 0.0f, 1.0f});
    BufferManager m_bufferManager;
    GLuint m_VAO, m_VBO;
    GLuint m_playerVAO, m_playerVBO;
    GLuint m_enemyVAO, m_enemyVBO;
};