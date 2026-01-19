#pragma once
#include <dxe.h>
#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

class PlayerController {
public:
    PlayerController(float x, float y);
    void Update(float delta_time);
    void Draw();
    bool IsInGrassArea() const;

private:
    float x_, y_;
    float speed_;
};
#endif