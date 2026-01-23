#include "BattleManager.h"

void BattleManager::Start() {
    playerHP = 10;
    enemyHP = 10;
    isFinished = false;
    selectedCommand = BattleCommand::FIGHT;
    result = BattleResult::NONE;
    resultTimer = 0.0f;
}

void BattleManager::Update(float delta_time) {
    if (result != BattleResult::NONE) {
        resultTimer += delta_time;
        if (resultTimer > 2.5f) isFinished = true;
        return;
    }

    // コマンド選択
    if (CheckHitKey(KEY_INPUT_UP) || CheckHitKey(KEY_INPUT_DOWN)) {
        selectedCommand = (selectedCommand == BattleCommand::FIGHT) ? BattleCommand::RUN : BattleCommand::FIGHT;
        WaitTimer(150);
    }

    // 決定
    if (CheckHitKey(KEY_INPUT_RETURN)) {
        if (selectedCommand == BattleCommand::FIGHT) {
            enemyHP -= attackDamage;
            if (enemyHP <= 0) { enemyHP = 0; result = BattleResult::WIN; }
            else {
                playerHP -= attackDamage;
                if (playerHP <= 0) { playerHP = 0; result = BattleResult::LOSE; }
            }
        }
        else {
            isFinished = true; // にげる
        }
        WaitTimer(150);
    }
}

void BattleManager::Draw() {
    // 背景
    DrawBox(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, GetColor(30, 30, 60), TRUE);
    DrawFormatString(20, 20, GetColor(255, 255, 255), "Battle Mode!");
    DrawFormatString(50, 300, GetColor(255, 255, 255), "Player HP: %d", playerHP);
    DrawFormatString(400, 100, GetColor(255, 255, 255), "Enemy HP: %d", enemyHP);

    // コマンド描画
    const char* f_mark = (selectedCommand == BattleCommand::FIGHT) ? "> " : "  ";
    const char* r_mark = (selectedCommand == BattleCommand::RUN) ? "> " : "  ";
    DrawFormatString(100, 400, GetColor(255, 255, 255), "%sたたかう", f_mark);
    DrawFormatString(100, 430, GetColor(255, 255, 255), "%sにげる", r_mark);

    // 結果表示
    if (result == BattleResult::WIN) DrawFormatString(200, 200, GetColor(255, 255, 255), "YOU WIN!");
    if (result == BattleResult::LOSE) DrawFormatString(200, 200, GetColor(255, 255, 255), "YOU LOSE...");
}