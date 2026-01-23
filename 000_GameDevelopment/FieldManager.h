#pragma once
#include <memory>
#include "PlayerController.h"

class FieldManager {
public:
    FieldManager();           // コンストラクタ
    void Update(float delta);
    void Draw();
    bool ShouldEncounter() const;
    void ResetEncounter();    

private:
    float encounterTimer = 0.0f;
    std::unique_ptr<PlayerController> player; // プレイヤーの保持
};