#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <fmt/core.h>

#include "../render/color.h"

struct BoundingBox {
    float x, y, width, height;
};

class GameObject {
   public:
    GameObject(float x, float y, Color color);

    std::vector<glm::vec3> calculateTriangleVertices();
    void rotateBy(float rotation);
    void moveLeft(float deltaTime);
    void moveRight(float deltaTime);
    void moveUp(float deltaTime);
    void moveDown(float deltaTime);

    bool isInitialized();

    void setInitialized(bool initialized);
    void setColor(Color color);
    void setVelocity(glm::vec3 velocity);
    void setSpeed(float speed);

    glm::mat4 getModelMatrix();
    glm::vec3 getVelocity();
    BoundingBox getBoundingBox();
    void calculateBoundingBox(std::vector<float> vertices);

    // public variables
    Color m_color;

   private:
    BoundingBox m_boundingBox;
    bool m_isInitialized = false;

    float m_speed = 1.0f;
    float m_rotation = 0.0f;
    glm::mat4 m_modelMatrix = glm::mat4(1.0f);
    glm::vec3 m_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
};