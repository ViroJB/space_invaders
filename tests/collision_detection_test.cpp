#include <gtest/gtest.h>

#include "../src/collision_detection/collision_detection.h"

TEST(CollisionDetectionTest, BoxCollision) {
    BoundingBox box1 = {0, 0, 2, 2};
    BoundingBox box2 = {1, 1, 2, 2};
    BoundingBox box3 = {3, 3, 2, 2};

    CollisionDetection cd;
    EXPECT_TRUE(cd.hasBoxCollision(&box1, &box2));
    EXPECT_FALSE(cd.hasBoxCollision(&box1, &box3));
}

TEST(CollisionDetectionTest, SATCollision) {
    std::vector<glm::vec2> vertices1 = {glm::vec2(0.0f, 0.0f), glm::vec2(2.0f, 0.0f),
                                        glm::vec2(2.0f, 2.0f), glm::vec2(0.0f, 2.0f)};
    std::vector<glm::vec2> vertices2 = {glm::vec2(1.0f, 1.0f), glm::vec2(3.0f, 1.0f),
                                        glm::vec2(3.0f, 3.0f), glm::vec2(1.0f, 3.0f)};
    std::vector<glm::vec2> vertices3 = {glm::vec2(3.0f, 3.0f), glm::vec2(5.0f, 3.0f),
                                        glm::vec2(5.0f, 5.0f), glm::vec2(3.0f, 5.0f)};

    CollisionDetection cd;
    EXPECT_TRUE(cd.hasSATCollision(&vertices1, &vertices2));
    EXPECT_FALSE(cd.hasSATCollision(&vertices1, &vertices3));
}

TEST(CollisionDetectionTest, NoCollision) {
    std::vector<glm::vec2> vertices1 = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};
    std::vector<glm::vec2> vertices2 = {{1.1f, 1.1f}, {2.1f, 1.1f}, {2.1f, 2.1f}, {1.1f, 2.1f}};

    CollisionDetection collisionDetection;
    bool collision = collisionDetection.hasSATCollision(&vertices1, &vertices2);

    EXPECT_FALSE(collision);
}

TEST(CollisionDetectionTest, OverlapOnAxis) {
    std::vector<glm::vec2> vertices1 = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};
    std::vector<glm::vec2> vertices2 = {{0.5f, 0.5f}, {1.5f, 0.5f}, {1.5f, 1.5f}, {0.5f, 1.5f}};

    glm::vec2 axis = {1.0f, 0.0f};

    CollisionDetection collisionDetection;
    bool overlap = collisionDetection.overlapOnAxis(&vertices1, &vertices2, axis);

    EXPECT_TRUE(overlap);
}

TEST(CollisionDetectionTest, NoOverlapOnAxis) {
    std::vector<glm::vec2> vertices1 = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};
    std::vector<glm::vec2> vertices2 = {{1.1f, 1.1f}, {2.1f, 1.1f}, {2.1f, 2.1f}, {1.1f, 2.1f}};

    glm::vec2 axis = {1.0f, 0.0f};

    CollisionDetection collisionDetection;
    bool overlap = collisionDetection.overlapOnAxis(&vertices1, &vertices2, axis);

    EXPECT_FALSE(overlap);
}

// Tests for transformVertices function
TEST(CollisionDetectionTest, TransformVertices) {
    std::vector<float> vertices = {1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                                   -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                                   -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                                   1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f};

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 0.0f));
    CollisionDetection cd;
    std::vector<glm::vec2> transformedVertices = cd.transformVertices(vertices, transform);

    EXPECT_EQ(transformedVertices.size(), 4);
    EXPECT_EQ(transformedVertices[0], glm::vec2(2.0f, 2.0f));
    EXPECT_EQ(transformedVertices[1], glm::vec2(0.0f, 2.0f));
    EXPECT_EQ(transformedVertices[2], glm::vec2(0.0f, 0.0f));
    EXPECT_EQ(transformedVertices[3], glm::vec2(2.0f, 0.0f));
}

TEST(CollisionDetectionTest, IsColliding_Two) {
    // Object 1 vertices forming a square
    std::vector<float> vertices1 = {0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                                    2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
                                    2.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
                                    0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f};

    // Object 2 vertices forming another square
    std::vector<float> vertices2 = {1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                                    3.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
                                    3.0f, 3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
                                    1.0f, 3.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f};

    // Initialize GameObjects with bounding boxes
    GameObject object1(0, 0, &vertices1);
    GameObject object2(1, 1, &vertices2);

    // Calculate bounding boxes
    object1.calculateBoundingBox(&vertices1);
    object2.calculateBoundingBox(&vertices2);

    // Initialize collision detection
    CollisionDetection cd;

    // Check for collision
    EXPECT_TRUE(cd.isColliding(&object1, &object2));
}

TEST(CollisionDetectionTest, ComplexCollision) {
    // Object 1 vertices forming a square
    std::vector<float> vertices1 = {0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                                    2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
                                    2.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
                                    0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f};

    // Object 2 vertices forming another square
    std::vector<float> vertices2 = {1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                                    3.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
                                    3.0f, 3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
                                    1.0f, 3.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f};

    // Initialize GameObjects with bounding boxes
    GameObject object1(0, 0, &vertices1);
    GameObject object2(1, 1, &vertices2);

    // Calculate bounding boxes
    object1.calculateBoundingBox(&vertices1);
    object2.calculateBoundingBox(&vertices2);

    // Print bounding boxes for debugging
    BoundingBox* bb1 = object1.getBoundingBox();
    BoundingBox* bb2 = object2.getBoundingBox();
    std::cout << "Bounding Box 1: (" << bb1->x << ", " << bb1->y << ", " << bb1->width << ", " << bb1->height << ")" << std::endl;
    std::cout << "Bounding Box 2: (" << bb2->x << ", " << bb2->y << ", " << bb2->width << ", " << bb2->height << ")" << std::endl;

    // Initialize collision detection
    CollisionDetection cd;

    // Check for collision with additional debugging
    bool isColliding = cd.isColliding(&object1, &object2);
    std::cout << "Collision detected: " << isColliding << std::endl;
    EXPECT_TRUE(isColliding);
}