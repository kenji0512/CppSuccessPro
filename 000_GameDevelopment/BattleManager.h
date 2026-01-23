#pragma once
#include <dxe.h>

enum class BattleCommand { FIGHT, RUN };
enum class BattleResult { WIN, LOSE, NONE };

class BattleManager {
public:
    void Start();                  // 戦闘開始（HPリセットなど）
    void Update(float delta_time); // 入力・ロジック更新
    void Draw();                   // 描画
    bool IsFinished() const { return isFinished; }

private:
    int playerHP;
    int enemyHP;
    const int attackDamage = 3;
    bool isFinished;
    BattleCommand selectedCommand;
    BattleResult result;
    float resultTimer;
};