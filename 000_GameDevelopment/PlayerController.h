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

    // 移動中かどうかを返す関数
    bool IsMoving() const {
        // 上下左右のいずれかのキーが押されているかチェック
        // ※お使いの入力検知システムに合わせて書き換えてください
        return (CheckHitKey(KEY_INPUT_UP) ||
            CheckHitKey(KEY_INPUT_DOWN) ||
            CheckHitKey(KEY_INPUT_LEFT) ||
            CheckHitKey(KEY_INPUT_RIGHT));
    }
private:
    float x_, y_;
    float speed_;
};
#endif