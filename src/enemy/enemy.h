#pragma once

#include "../game_object/game_object.h"

class Enemy {
   public:
    Enemy(float x, float y, std::vector<float>* vertices);
    ~Enemy();

    GameObject* getGameObject();
    BoundingBox* getBoundingBox();

    void setColor(Color color);

    void moveDown(float deltaTime);

   private:
    GameObject* m_gameObject;
};