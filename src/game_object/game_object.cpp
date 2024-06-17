#include "game_object.h"

GameObject::GameObject(float x, float y, Color color) {
    glm::vec3 position = glm::vec3(x, y, 0.0f);
    m_modelMatrix = glm::translate(m_modelMatrix, position);
    m_color = color;
}

std::vector<glm::vec3> GameObject::calculateTriangleVertices() {
    std::vector<glm::vec3> vertices;
    // vertices.push_back(glm::vec3(m_currentPosition.x, m_currentPosition.y, 0.0f));
    // vertices.push_back(glm::vec3(m_currentPosition.x + 0.2f, m_currentPosition.y - 0.4f, 0.0f));
    // vertices.push_back(glm::vec3(m_currentPosition.x - 0.2f, m_currentPosition.y - 0.4f, 0.0f));
    return vertices;
}

// relative amount, not absolute position
void GameObject::moveBy(glm::vec3 amount) {
    m_modelMatrix = glm::translate(m_modelMatrix, amount);
}

void GameObject::rotateBy(float amount) {
    m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(amount), glm::vec3(0.0f, 0.0f, 1.0f));
}

bool GameObject::isInitialized() {
    return m_isInitialized;
}

void GameObject::setInitialized(bool initialized) {
    m_isInitialized = initialized;
}

void GameObject::setColor(Color color) {
    m_color = color;
}

glm::mat4 GameObject::getModelMatrix() {
    return m_modelMatrix;
}