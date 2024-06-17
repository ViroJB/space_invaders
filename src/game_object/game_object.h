#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>   

#include "../render/color.h"

class GameObject {
   public:
    GameObject(float x, float y, Color color);

    std::vector<glm::vec3> calculateTriangleVertices();
    void moveBy(glm::vec3 amount);
    void rotateBy(float rotation);

    bool isInitialized();

    void setInitialized(bool initialized);
    void setColor(Color color);

    glm::mat4 getModelMatrix();

    // public variables
    Color m_color;

   private:
    bool m_isInitialized = false;

    glm::mat4 m_modelMatrix = glm::mat4(1.0f);

    glm::vec3 m_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
};