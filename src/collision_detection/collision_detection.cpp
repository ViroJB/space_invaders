#include "collision_detection.h"

CollisionDetection::CollisionDetection() {}

CollisionDetection::~CollisionDetection() {}

bool CollisionDetection::isColliding(GameObject* object1, GameObject* object2) {
    bool collision = false;
    if (hasBoxCollision(object1->getBoundingBox(), object2->getBoundingBox())) {
        glm::mat4 transform1 = object1->getModelMatrix();
        glm::mat4 transform2 = object2->getModelMatrix();

        // get and transform vertices
        auto object1Vertices = object1->getVertices();
        auto object2Vertices = object2->getVertices();

        std::vector<glm::vec2> transformedVertices1 = transformVertices(*object1Vertices, transform1);
        std::vector<glm::vec2> transformedVertices2 = transformVertices(*object2Vertices, transform2);

        collision = hasSATCollision(&transformedVertices1, &transformedVertices2);
    }
    return collision;
}

bool CollisionDetection::hasBoxCollision(const BoundingBox* box1, const BoundingBox* box2) {
    return box1->x < box2->x + box2->width && box1->x + box1->width > box2->x && box1->y < box2->y + box2->height &&
           box1->y + box1->height > box2->y;
}

bool CollisionDetection::hasSATCollision(std::vector<glm::vec2>* vertices1, std::vector<glm::vec2>* vertices2) {
    std::vector<glm::vec2> axes;

    // get the axes from the first polygon
    for (size_t i = 0; i < vertices1->size(); ++i) {
        glm::vec2 p1 = vertices1->at(i);
        glm::vec2 p2 = vertices1->at((i + 1) % vertices1->size());
        glm::vec2 edge = p2 - p1;
        glm::vec2 axis(-edge.y, edge.x);  // perpendicular to edge
        axis = glm::normalize(axis);
        axes.push_back(axis);
    }

    // get the axes from the second polygon
    for (size_t i = 0; i < vertices2->size(); ++i) {
        glm::vec2 p1 = vertices2->at(i);
        glm::vec2 p2 = vertices2->at((i + 1) % vertices2->size());
        glm::vec2 edge = p2 - p1;
        glm::vec2 axis(-edge.y, edge.x);  // perpendicular to edge
        axis = glm::normalize(axis);
        axes.push_back(axis);
    }

    // check for overlap on all axes
    for (const auto& axis : axes) {
        if (!overlapOnAxis(vertices1, vertices2, axis)) {
            return false;  // no collision
        }
    }

    return true;  // collision detected
}

bool CollisionDetection::overlapOnAxis(const std::vector<glm::vec2>* vertices1, const std::vector<glm::vec2>* vertices2,
                                       const glm::vec2 axis) {
    float min1 = FLT_MAX, max1 = -FLT_MAX;
    float min2 = FLT_MAX, max2 = -FLT_MAX;

    for (const auto& vertex : *vertices1) {
        float projection = glm::dot(vertex, axis);
        min1 = std::min(min1, projection);
        max1 = std::max(max1, projection);
    }

    for (const auto& vertex : *vertices2) {
        float projection = glm::dot(vertex, axis);
        min2 = std::min(min2, projection);
        max2 = std::max(max2, projection);
    }

    return !(max1 < min2 || max2 < min1);
}

std::vector<glm::vec2> CollisionDetection::transformVertices(const std::vector<float>& vertices, const glm::mat4& transform) {
    std::vector<glm::vec2> transformedVertices;
    for (size_t i = 0; i < vertices.size(); i += 7) {  // assuming vertices are stored as x, y, z, r, g, b, a
        glm::vec4 vertex(vertices[i], vertices[i + 1], vertices[i + 2], 1.0f);
        glm::vec4 transformedVertex = transform * vertex;
        transformedVertices.push_back(glm::vec2(transformedVertex));
    }
    return transformedVertices;
}

// bool CollisionDetection::hasBoxCollision(std::list<GameObject>& enemies, std::list<GameObject>& projectiles) {
//     // check for collisions between enemies and projectiles
//     for (auto enemy = enemies.begin(); enemy != enemies.end();) {
//         bool enemyErased = false;
//         for (auto projectile = projectiles.begin(); projectile != projectiles.end();) {
//             if (enemy->getBoundingBox()->x < projectile->getBoundingBox()->x + projectile->getBoundingBox()->width &&
//                 enemy->getBoundingBox()->x + enemy->getBoundingBox()->width > projectile->getBoundingBox()->x &&
//                 enemy->getBoundingBox()->y < projectile->getBoundingBox()->y + projectile->getBoundingBox()->height &&
//                 enemy->getBoundingBox()->y + enemy->getBoundingBox()->height > projectile->getBoundingBox()->y) {
//                 // fmt::print("Collision detected\n");
//                 if (checkForPreciseCollision(*enemy, *projectile)) {
//                     // fmt::print("Precise collision detected\n");
//                     enemy = enemies.erase(enemy);
//                     projectile = projectiles.erase(projectile);
//                     enemyErased = true;
//                 }
//                 break;  // exit inner loop since the current enemy is erased
//             } else {
//                 ++projectile;
//             }
//         }
//         if (!enemyErased) {
//             ++enemy;
//         }
//     }
// }