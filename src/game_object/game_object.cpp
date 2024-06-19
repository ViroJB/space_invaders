#include "game_object.h"

GameObject::GameObject(float x, float y, Color color) {
    m_boundingBox.x = x;
    m_boundingBox.y = y;
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

void GameObject::calculateBoundingBox(std::vector<float> vertices) {
    // Initialize variables to find bounding box
    float minX = std::numeric_limits<float>::max();
    float minY = std::numeric_limits<float>::max();
    float maxX = -std::numeric_limits<float>::max();
    float maxY = -std::numeric_limits<float>::max();

    // Iterate through vertices
    for (size_t i = 0; i < vertices.size(); i += 7) { // Assuming each vertex has 7 elements (position + color)
        float x = vertices[i];
        float y = vertices[i + 1];
        
        // Update bounding box
        if (x < minX) minX = x;
        if (x > maxX) maxX = x;
        if (y < minY) minY = y;
        if (y > maxY) maxY = y;
    }

    // Calculate width and height
    float width = maxX - minX;
    float height = maxY - minY;

    m_boundingBox.width = width;
    m_boundingBox.height = height;
}

void GameObject::moveLeft(float deltaTime) {
    m_velocity = glm::vec3(-0.1f, 0.0f, 0.0f);
    glm::vec3 position = (m_velocity * m_speed) * deltaTime;
    m_modelMatrix = glm::translate(m_modelMatrix, position);
    m_velocity = glm::vec3(0.0f, 0.0f, 0.0f);

    m_boundingBox.x = m_modelMatrix[3][0];
}

void GameObject::moveRight(float deltaTime) {
    m_velocity = glm::vec3(0.1f, 0.0f, 0.0f);
    glm::vec3 position = (m_velocity * m_speed) * deltaTime;
    m_modelMatrix = glm::translate(m_modelMatrix, position);
    m_velocity = glm::vec3(0.0f, 0.0f, 0.0f);

    m_boundingBox.x = m_modelMatrix[3][0];
}

void GameObject::moveUp(float deltaTime) {
    m_velocity = glm::vec3(0.0f, 0.1f, 0.0f);
    glm::vec3 position = (m_velocity * m_speed) * deltaTime;
    m_modelMatrix = glm::translate(m_modelMatrix, position);
    m_velocity = glm::vec3(0.0f, 0.0f, 0.0f);

    m_boundingBox.y = m_modelMatrix[3][1]; 
}

void GameObject::moveDown(float deltaTime) {
    m_velocity = glm::vec3(0.0f, -0.1f, 0.0f);
    glm::vec3 position = (m_velocity * m_speed) * deltaTime;
    // check if y is below 0.0 and if so, reset to 0.0
    // this check stops the movement of enemies, because y is always < 0.0 once they moved for the first time
    // TODO normalize the positions of all objects. use x -1 and y 1 as starting point, and objects x and y also start top left
    // TODO but how do we make sure that the player cant leave the screen? at the point where y is bigger thatn 1.0, the object would be out
    // of the screen

    // if (m_modelMatrix[3][1] < 0.0f) {
    //     position = glm::vec3(0.0f, 0.0f, 0.0f);
    // }

    m_modelMatrix = glm::translate(m_modelMatrix, position);
    m_velocity = glm::vec3(0.0f, 0.0f, 0.0f);

    m_boundingBox.y = m_modelMatrix[3][1];
}

void GameObject::rotateBy(float amount) {
    m_rotation += amount;
    // m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(amount), glm::vec3(1.0f, 0.0f, 0.0f));
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
    // return glm::rotate(m_modelMatrix, glm::radians(m_rotation), glm::vec3(1.0f, 0.0f, 0.0f));
    // return m_modelMatrix;
    glm::mat4 model = glm::mat4(1.0f);

    // Wende die Translation an
    model = glm::translate(model, glm::vec3(m_modelMatrix[3][0], m_modelMatrix[3][1], m_modelMatrix[3][2]));
    model = glm::rotate(model, glm::radians(m_rotation), glm::vec3(0.0f, 1.0f, 0.0f));

    return model;
}

BoundingBox GameObject::getBoundingBox() {
    return m_boundingBox;
}