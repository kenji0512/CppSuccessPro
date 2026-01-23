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
    switch (step) {
    case BattleStep::COMMAND_SELECT:
        // --- コマンド選択処理 ---
        if (CheckHitKey(KEY_INPUT_UP) || CheckHitKey(KEY_INPUT_DOWN)) {
            selectedCommand = (selectedCommand == BattleCommand::FIGHT) ? BattleCommand::RUN : BattleCommand::FIGHT;
            WaitTimer(150);
        }

        if (CheckHitKey(KEY_INPUT_RETURN)) {
            if (selectedCommand == BattleCommand::FIGHT) {
                // 攻撃開始！メッセージをセットしてステップ移行
                battleMessage = "ピカチュウの こうげき！";
                enemyHP -= attackDamage; // ダメージはここで引いておく
                if (enemyHP < 0) enemyHP = 0;

                step = BattleStep::MESSAGE_DISPLAY;
                messageTimer = 0.0f;
            }
            else {
                isFinished = true; // にげる
            }
            WaitTimer(150);
        }
        break;

    case BattleStep::MESSAGE_DISPLAY:
        // --- メッセージ表示中 ---
        messageTimer += delta_time;
        if (messageTimer > 1.5f) { // 1.5秒ごとに次のアクションへ

            // 1. 敵のHPが0になった場合（Playerの勝ち）
            if (enemyHP <= 0) {
                battleMessage = "コラッタは たおれた！";
                result = BattleResult::WIN;
                // result が NONE 以外になると、Updateの冒頭でリザルトタイマーが動き出します
            }
            // 2. まだ敵が生きていて、かつ敵が攻撃していない場合（敵のターン）
            else if (battleMessage.find("コラッタ") == std::string::npos) {
                battleMessage = "コラッタの こうげき！";
                playerHP -= attackDamage;
                if (playerHP < 0) playerHP = 0;
                messageTimer = 0.0f;
            }
            // 3. どちらも倒れず、ターンが終了した場合
            else {
                if (playerHP <= 0) {
                    battleMessage = "ピカチュウは たおれてしまった...";
                    result = BattleResult::LOSE;
                }
                else {
                    step = BattleStep::COMMAND_SELECT; // コマンド選択に戻る
                }
            }
        }
        break;
    }
    // コマンド選択
    if (CheckHitKey(KEY_INPUT_UP) || CheckHitKey(KEY_INPUT_DOWN)) {
        selectedCommand = (selectedCommand == BattleCommand::FIGHT) ? BattleCommand::RUN : BattleCommand::FIGHT;
        WaitTimer(150);
    }
}

void BattleManager::Draw() {
    // 1. 背景（少しグラデーションっぽく暗くしてもいいですね）
    DrawBox(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, GetColor(20, 20, 40), TRUE);

    // --- 敵の情報（右上） ---
    int enemyBaseX = DXE_WINDOW_WIDTH - 250;
    int enemyBaseY = 50;
    SetFontSize(20); // 名前用に少し大きく
    DrawFormatString(enemyBaseX, enemyBaseY, GetColor(255, 255, 255), enemyName);
    // 敵HPバーの枠
    DrawBox(enemyBaseX, enemyBaseY + 30, enemyBaseX + 200, enemyBaseY + 45, GetColor(100, 0, 0), TRUE);
    // 敵HPバーの中身
    float enemyRate = (float)enemyHP / 10.0f;
    DrawBox(enemyBaseX, enemyBaseY + 30, enemyBaseX + (int)(200 * enemyRate), enemyBaseY + 45, GetColor(0, 255, 0), TRUE);
    // 敵の数字
    SetFontSize(16);
    DrawFormatString(enemyBaseX + 130, enemyBaseY + 10, GetColor(255, 255, 255), "HP: %d/10", enemyHP);

    // --- プレイヤーの情報（左下） ---
    int playerBaseX = 50;
    int playerBaseY = 250;
    DrawFormatString(playerBaseX, playerBaseY, GetColor(255, 255, 255), playerName);
    // プレイヤーHPバーの枠
    DrawBox(playerBaseX, playerBaseY + 30, playerBaseX + 200, playerBaseY + 45, GetColor(100, 0, 0), TRUE);
    // プレイヤーHPバーの中身
    float playerRate = (float)playerHP / 10.0f;
    DrawBox(playerBaseX, playerBaseY + 30, playerBaseX + (int)(200 * playerRate), playerBaseY + 45, GetColor(0, 255, 0), TRUE);
    // プレイヤーの数字
    SetFontSize(16);
    DrawFormatString(playerBaseX + 130, playerBaseY + 10, GetColor(255, 255, 255), "HP: %d/10", playerHP);


    // --- メッセージ/コマンド枠（画面最下部） ---
    DrawBox(10, 350, DXE_WINDOW_WIDTH - 10, DXE_WINDOW_HEIGHT - 10, GetColor(255, 255, 255), FALSE);

    if (step == BattleStep::COMMAND_SELECT) {
        // コマンド選択中は「たたかう」「にげる」を表示
        SetFontSize(40);
        unsigned int f_color = (selectedCommand == BattleCommand::FIGHT) ? GetColor(255, 255, 0) : GetColor(255, 255, 255);
        unsigned int r_color = (selectedCommand == BattleCommand::RUN) ? GetColor(255, 255, 0) : GetColor(255, 255, 255);

        const char* f_mark = (selectedCommand == BattleCommand::FIGHT) ? "> " : "  ";
        const char* r_mark = (selectedCommand == BattleCommand::RUN) ? "> " : "  ";

        DrawFormatString(80, 400, f_color, "%sたたかう", f_mark);
        DrawFormatString(380, 400, r_color, "%sにげる", r_mark);
    }
    else if (step == BattleStep::MESSAGE_DISPLAY) {
        // メッセージ表示中は、コマンドを消して文章だけを表示                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
        SetFontSize(30);
        DrawFormatString(50, 400, GetColor(255, 255, 255), battleMessage.c_str());
    }
    // 結果表示（resultがWINかLOSEになったら表示）
    if (result == BattleResult::WIN) {
        SetFontSize(80);
        // 文字の影を作ると豪華に見えます
        DrawFormatString(340, 162, GetColor(0, 0, 0), "YOU WIN!");
        DrawFormatString(160, 160, GetColor(255, 255, 0), "YOU WIN!");

        SetFontSize(60);
        DrawFormatString(340, 500, GetColor(255, 255, 255), "ピカチュウの かち！");
    }
    else if (result == BattleResult::LOSE) {
        SetFontSize(80);
        DrawFormatString(160, 160, GetColor(255, 0, 0), "LOSE...");
    }
    // 次の描画のためにフォントサイズをデフォルトに戻しておく
    SetFontSize(16);
}