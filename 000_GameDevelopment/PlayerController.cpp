#include "PlayerController.h"

PlayerController::PlayerController(float x, float y)
    : x_(x), y_(y), speed_(150.0f)
{
}

void PlayerController::Update(float delta_time) {
    if (CheckHitKey(KEY_INPUT_UP))    y_ -= speed_ * delta_time;
    if (CheckHitKey(KEY_INPUT_DOWN))  y_ += speed_ * delta_time;
    if (CheckHitKey(KEY_INPUT_LEFT))  x_ -= speed_ * delta_time;
    if (CheckHitKey(KEY_INPUT_RIGHT)) x_ += speed_ * delta_time;

    // 画面外に出ないように
    if (x_ < 0) x_ = 0;
    if (x_ > DXE_WINDOW_WIDTH) x_ = DXE_WINDOW_WIDTH;
    if (y_ < 0) y_ = 0;
    if (y_ > DXE_WINDOW_HEIGHT) y_ = DXE_WINDOW_HEIGHT;
}

void PlayerController::Draw() {
    DrawCircle((int)x_, (int)y_, 5, GetColor(255, 255, 255), TRUE);
}

bool PlayerController::IsInGrassArea() const {
    // 画面下100pxを草むらエリアとする
    return y_ > DXE_WINDOW_HEIGHT - 100;
}
