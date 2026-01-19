// bullet.cpp
#include "bullet.h"

Bullet::Bullet(float x, float y, float speed)
    : _x(x), _y(y), _speed(speed) {
}

void Bullet::Update(float deltaTime) {
    _y -= _speed * deltaTime;
    if (_y < 0) _isDead = true;
}

void Bullet::Draw() const {
    DrawCircle(_x, _y, 5, GetColor(255, 255, 0), TRUE);
}
