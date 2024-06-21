#pragma once

#include "../game_object/game_object.h"
#include "../render/color.h"

class Player {
   public:
    Player(std::vector<float>* vertices, Color color);
    ~Player();

    GameObject* getGameObject();
    
    void setColor(Color color);

    void moveLeft(float deltaTime);
    void moveRight(float deltaTime);
    void moveUp(float deltaTime);
    void moveDown(float deltaTime);

    void triggerShootingCooldown();
    void updateShootingCooldown(float deltaTime);
    float getShootingCooldown();
    BoundingBox* getBoundingBox();

   private:
    GameObject* m_gameObject;
    float m_shootingCooldown = 0.0f;
    float m_shootingCooldownMax = 0.5f;
};