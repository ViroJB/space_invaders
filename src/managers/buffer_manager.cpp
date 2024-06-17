#include "buffer_manager.h"

void BufferManager::setupBuffers(GLuint& VAO, GLuint& VBO) {
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    // glBindVertexArray(VAO);
    // glBindBuffer(GL_ARRAY_BUFFER, VBO);
}

void BufferManager::cleanup(GLuint VAO, GLuint VBO) {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}