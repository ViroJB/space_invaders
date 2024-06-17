#include "renderer.h"

Renderer::Renderer() {
    m_bufferManager.setupBuffers(m_VAO, m_VBO);
    m_shaderProgram = m_shaderManager.compileAndLinkShaders();
    glUseProgram(m_shaderProgram);
}

void Renderer::setShaderProgram(GLuint shaderProgram) {
    m_shaderProgram = shaderProgram;
}

GLuint Renderer::getShaderProgram() {
    return m_shaderProgram;
}

void Renderer::printCurrentMatrix(GameObject* obj) {
    auto modelMatrix = obj->getModelMatrix();
    fmt::print("Model matrix:\n");
    for (int i = 0; i < 4; ++i) {
        fmt::print("{}, {}, {}, {}\n", modelMatrix[i][0], modelMatrix[i][1], modelMatrix[i][2], modelMatrix[i][3]);
    }
}

std::array<GLfloat, 21> Renderer::getCombinedData(GameObject* obj) {
    auto vertices = obj->calculateTriangleVertices();
    std::array<GLfloat, 21> combinedData;

    for (int i = 0; i < 3; ++i) {
        combinedData[i * 7 + 0] = vertices[i].x;   // x
        combinedData[i * 7 + 1] = vertices[i].y;   // y
        combinedData[i * 7 + 2] = vertices[i].z;   // z
        combinedData[i * 7 + 3] = obj->m_color.r;  // r
        combinedData[i * 7 + 4] = obj->m_color.g;  // g
        combinedData[i * 7 + 5] = obj->m_color.b;  // b
        combinedData[i * 7 + 6] = obj->m_color.a;  // a
    }

    return combinedData;
}

void Renderer::initializeVertexData() {
    // player geometry
    float playerVertices[] = {
        // position            // color + alpha
        0.0f,    -0.75f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // top, red
        -0.125f, -0.99f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // bottom left, green
        0.125f,  -0.99f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f   // bottom right, blue
    };

    float enemyVertices[] = {
        // position            // color + alpha
        0.0f,    0.750f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // bottom, rot
        -0.125f, 1.0f,   0.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // oben links, grün
        0.125f,  1.0f,   0.0f, 1.0f, 0.0f, 0.0f, 1.0f   // oben rechts, blau
    };

    // player vao and vbo
    glGenVertexArrays(1, &m_playerVAO);
    glGenBuffers(1, &m_playerVBO);
    glBindVertexArray(m_playerVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_playerVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(playerVertices), playerVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // enemy vao and vbo
    glGenVertexArrays(1, &m_enemyVAO);
    glGenBuffers(1, &m_enemyVBO);
    glBindVertexArray(m_enemyVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_enemyVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(enemyVertices), enemyVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::render(GLFWwindow* window, std::vector<GameObject>& enemies, GameObject* player) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // draw enemies
    for (auto& obj : enemies) {
        m_shaderManager.transform(obj.getModelMatrix(), &obj.m_color);
        glBindVertexArray(m_enemyVAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    // draw player
    glBindVertexArray(m_playerVAO);
    m_shaderManager.transform(player->getModelMatrix(), &player->m_color);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
}