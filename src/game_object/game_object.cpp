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
void GameObject::moveBy(glm::vec3 amount, float deltaTime) {
    // m_modelMatrix = glm::translate(m_modelMatrix, amount);
    // glm::vec3 position = glm::vec3(m_modelMatrix[3][0], m_modelMatrix[3][1], m_modelMatrix[3][2]);

    glm::vec3 position = glm::vec3(0.0f);
    position = m_velocity * deltaTime;

    m_modelMatrix = glm::translate(m_modelMatrix, position);
}

void GameObject::moveLeft(float deltaTime) {
    m_velocity = glm::vec3(-0.1f, 0.0f, 0.0f);
    glm::vec3 position = (m_velocity * m_speed) * deltaTime;
    m_modelMatrix = glm::translate(m_modelMatrix, position);
    m_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
}

void GameObject::moveRight(float deltaTime) {
    m_velocity = glm::vec3(0.1f, 0.0f, 0.0f);
    glm::vec3 position = (m_velocity * m_speed) * deltaTime;
    m_modelMatrix = glm::translate(m_modelMatrix, position);
    m_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
}

void GameObject::moveUp(float deltaTime) {
    m_velocity = glm::vec3(0.0f, 0.1f, 0.0f);
    glm::vec3 position = (m_velocity * m_speed) * deltaTime;
    m_modelMatrix = glm::translate(m_modelMatrix, position);
    m_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
}

void GameObject::moveDown(float deltaTime) {
    m_velocity = glm::vec3(0.0f, -0.1f, 0.0f);
    glm::vec3 position = (m_velocity * m_speed) * deltaTime;
    // check if y is below 0.0 and if so, reset to 0.0
    // this check stops the movement of enemies, because y is always < 0.0 once they moved for the first time
    // TODO normalize the positions of all objects. use x -1 and y 1 as starting point, and objects x and y also start top left
    // TODO but how do we make sure that the player cant leave the screen? at the point where y is bigger thatn 1.0, the object would be out of the screen

    // if (m_modelMatrix[3][1] < 0.0f) {
    //     position = glm::vec3(0.0f, 0.0f, 0.0f);
    // }

    m_modelMatrix = glm::translate(m_modelMatrix, position);
    m_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
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

void GameObject::setSpeed(float speed) {
    m_speed = speed;
}

void GameObject::setVelocity(glm::vec3 velocity) {
    m_velocity = velocity;
}

glm::vec3 GameObject::getVelocity() {
    return m_velocity;
}

glm::mat4 GameObject::getModelMatrix() {
    return m_modelMatrix;
}

BoundingBox GameObject::getBoundingBox() {
    return m_boundingBox;
}