#pragma once
#include <dxe.h>

enum class BattleCommand { FIGHT, RUN };
enum class BattleResult { WIN, LOSE, NONE };
enum class BattleStep {
    COMMAND_SELECT, // コマンド選択中
    MESSAGE_DISPLAY, // メッセージ表示中
    CHECK_ALIVE      // 生死判定
};

class BattleManager {
public:
    void Start();                  // 戦闘開始（HPリセットなど）
    void Update(float delta_time); // 入力・ロジック更新
    void Draw();                   // 描画
    bool IsFinished() const { return isFinished; }

private:
    int playerHP;
    int enemyHP;
    const char* playerName = "ピカチュウ";
    const char* enemyName = "コラッタ";
    const int attackDamage = 3;
    bool isFinished;
    BattleCommand selectedCommand;
    BattleResult result;
    float resultTimer;
    BattleStep step = BattleStep::COMMAND_SELECT;
    std::string battleMessage = "";
    float messageTimer = 0.0f;
};