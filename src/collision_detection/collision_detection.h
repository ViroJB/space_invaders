#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "../game_object/game_object.h"


class CollisionDetection {
   public:
    CollisionDetection();
    ~CollisionDetection();

    bool isColliding(GameObject* object1, GameObject* object2);
    bool hasBoxCollision(const BoundingBox* box1, const BoundingBox* box2);
    bool hasSATCollision(std::vector<glm::vec2>* vertices1, std::vector<glm::vec2>* vertices2);
    bool overlapOnAxis(const std::vector<glm::vec2>* vertices1, const std::vector<glm::vec2>* vertices2, const glm::vec2 axis);
    std::vector<glm::vec2> transformVertices(const std::vector<float>& vertices, const glm::mat4& translation);

   private:
};