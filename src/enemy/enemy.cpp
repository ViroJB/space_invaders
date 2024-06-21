#include "enemy.h"

Enemy::Enemy(float x, float y, std::vector<float>* vertices) {
    m_gameObject = new GameObject(x, y, vertices);
    m_gameObject->calculateBoundingBox(vertices);
    m_gameObject->setSpeed(1.0f);
}

GameObject* Enemy::getGameObject() {
    return m_gameObject;
}

BoundingBox* Enemy::getBoundingBox() {
    return m_gameObject->getBoundingBox();
}

void Enemy::setColor(Color color) {
    m_gameObject->setColor(color);
}

void Enemy::moveDown(float deltaTime) {
    m_gameObject->moveDown(deltaTime);
}

Enemy::~Enemy() {
    // delete m_gameObject;
}