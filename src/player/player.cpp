#include "player.h"

Player::Player(std::vector<float>* vertices, Color color) {
    m_gameObject = new GameObject(0.875f, -1.75f, vertices);
    m_gameObject->calculateBoundingBox(vertices);
    m_gameObject->setSpeed(5.0f);
    m_gameObject->setColor(color);
}

GameObject* Player::getGameObject() {
    return m_gameObject;
}

BoundingBox* Player::getBoundingBox() {
    return m_gameObject->getBoundingBox();
}

void Player::moveLeft(float deltaTime) {
    if (m_gameObject->getBoundingBox()->x <= 0.0f) {
        return;
    }
    m_gameObject->moveLeft(deltaTime);
}

void Player::moveRight(float deltaTime) {
    if (m_gameObject->getBoundingBox()->x >= 2.0f - m_gameObject->getBoundingBox()->width) {
        return;
    }
    m_gameObject->moveRight(deltaTime);

}

void Player::moveUp(float deltaTime) {
    if (m_gameObject->getBoundingBox()->y >= 0.0f) {
        return;
    }
    m_gameObject->moveUp(deltaTime);
}

void Player::moveDown(float deltaTime) {
    if (m_gameObject->getBoundingBox()->y <= -2.0f + m_gameObject->getBoundingBox()->height) {
        return;
    }
    m_gameObject->moveDown(deltaTime);
}

void Player::updateShootingCooldown(float deltaTime) {
    if (m_shootingCooldown > 0.0f) {
        m_shootingCooldown -= deltaTime;
    }
}

float Player::getShootingCooldown() {
    return m_shootingCooldown;
}

void Player::triggerShootingCooldown() {
    m_shootingCooldown = m_shootingCooldownMax;
}

void Player::setColor(Color color) {
    m_gameObject->setColor(color);
}

Player::~Player() {
    delete m_gameObject;
}