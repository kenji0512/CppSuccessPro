// bullet.h
#pragma once
#include <dxe.h>

class Bullet {
public:
    Bullet(float x, float y, float speed);
    void Update(float deltaTime);
    void Draw() const;
    bool IsDead() const { return _isDead; }

private:
    float _x, _y, _speed;
    bool _isDead = false;
};
