#pragma once
#include <memory>

#include "FieldManager.h"
#include "BattleManager.h"
#include "FadeManager.h"

enum class GameState {
    FIELD,
    FADE_OUT,
    FADE_IN,
    BATTLE
};

class GameController {
public:
    void Init();
    void Update(float delta);
    void Draw();
    void Finalize();

private:
    GameState state;
    GameState nextState;

    std::unique_ptr<FieldManager> field;
    std::unique_ptr<BattleManager> battle;
    std::unique_ptr<FadeManager> fade;
};
