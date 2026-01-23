#include "FieldManager.h"
#include <dxe.h>

// コンストラクタ：ここでプレイヤーの実体を作る
FieldManager::FieldManager() {
    player = std::make_unique<PlayerController>(320.0f, 240.0f);
}
void FieldManager::Update(float delta)
{
    if (!player) return; // 安全策
    player->Update(delta);

    if (player->IsInGrassArea() && player->IsMoving()) {
        encounterTimer += delta;
    }
}

// 戦闘終了後に呼ぶためのリセット関数
void FieldManager::ResetEncounter() {
    encounterTimer = 0.0f;
}

void FieldManager::Draw() {
    // 地面（濃い緑）
    DrawBox(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, GetColor(0, 60, 0), TRUE);
    // 草むら（明るい緑）
    DrawBox(0, DXE_WINDOW_HEIGHT - 100, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, GetColor(0, 120, 0), TRUE);

    if (player) player->Draw();
}

bool FieldManager::ShouldEncounter() const {
    return encounterTimer > 2.0f;
}
