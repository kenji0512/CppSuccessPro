#include <memory>
#include <vector>
#include <time.h>
#include <dxe.h>
#include "../PlayerController.h"

//草むらに戻った時、すぐバトル始まっちゃうからそこ直したい

//------------------------------------------------------
// 状態列挙
enum class GameState {
    FIELD,      // フィールド移動中
    FADE_OUT,   // フェードアウト中（暗転してバトルへ）
    BATTLE,     // 戦闘中
    FADE_IN,    // フェードイン中（バトル画面表示）
    RESULT      // 戦闘結果表示
};
enum class BattleCommand {
    FIGHT,
    RUN
};
enum class BattleResult {
    WIN,
    LOSE,
    NONE
};
//------------------------------------------------------
// グローバル変数
std::unique_ptr<PlayerController> player;
GameState gameState = GameState::FIELD;
BattleCommand selectedCommand = BattleCommand::FIGHT;
BattleResult battleResult = BattleResult::NONE;
GameState nextStateAfterFade = GameState::FIELD;
float encounterTimer = 0.0f;
float fadeAlpha = 0.0f;  // フェードの透明度(0-255)
const float fadeSpeed = 255.0f / 2.5f; // 約2.5秒で変化（255 / 2.5）
float resultTimer = 0.0f;
float encounterCooldown = 0.0f; // 戦闘後の無敵時間
int playerHP = 10;
int enemyHP = 10;
const int attackDamage = 3;


//------------------------------------------------------
// 初期化
void gameStart() {
    srand((unsigned)time(nullptr));
    player = std::make_unique<PlayerController>(DXE_WINDOW_WIDTH_HALF, DXE_WINDOW_HEIGHT_HALF);
}

//------------------------------------------------------
// メインループ
void gameMain(float delta_time) {
    ClearDrawScreen();

    switch (gameState) {

        //--------------------------------------------------
        // フィールド状態
    case GameState::FIELD:
        if (encounterCooldown > 0.0f) {
            encounterCooldown -= delta_time;
            if (encounterCooldown < 0.0f)
                encounterCooldown = 0.0f;
        }

        player->Update(delta_time);

        // 背景（必要なら描画）
        //ClearDrawScreen(); // 背景色クリア（DxLib の仕様で不要なら省略）

        // 草むら描画（フィールド要素）
        DrawBox(0, DXE_WINDOW_HEIGHT - 100, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT,
            GetColor(0, 180, 0), TRUE);
        DrawBox(0, DXE_WINDOW_HEIGHT - 100, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT,
            GetColor(0, 100, 0), FALSE);

        // プレイヤーを一番前に
        player->Draw();

        // 草むらエリアにいる場合の疑似エンカウント
        encounterTimer += delta_time;
        if (encounterCooldown <= 0.0f &&
            player->IsInGrassArea() &&
            encounterTimer > 1.0f) {

            if (rand() % 100 < 5) {
                gameState = GameState::FADE_OUT;
                fadeAlpha = 0.0f;
                encounterTimer = 0.0f;
                nextStateAfterFade = GameState::FADE_IN;

            }
        }
        if (!player->IsInGrassArea()) {
            encounterTimer = 0.0f;
        }
        break;
        //--------------------------------------------------
    case GameState::FADE_OUT:
        // フィールド描画を維持しながら暗転していく
        //player->Draw();
        DrawBox(0, DXE_WINDOW_HEIGHT - 100, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT,
            GetColor(0, 180, 0), TRUE);
        DrawBox(0, DXE_WINDOW_HEIGHT - 100, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT,
            GetColor(0, 100, 0), FALSE);
        player->Draw();

        fadeAlpha += fadeSpeed * delta_time;
        if (fadeAlpha >= 255.0f) {
            fadeAlpha = 255.0f;
            gameState = nextStateAfterFade;
        }
        break;

        //--------------------------------------------------
                // フェードイン（バトル画面に切り替え）
    case GameState::FADE_IN:
        // まずバトル背景を描画
        DrawBox(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, GetColor(30, 30, 60), TRUE);
        DrawFormatString(20, 20, GetColor(255, 255, 255), "Battle Mode!");

        fadeAlpha -= fadeSpeed * delta_time;
        if (fadeAlpha <= 0.0f) {
            fadeAlpha = 0.0f;
            gameState = GameState::BATTLE; // フェードイン完了

            // ★ここで初期化
            playerHP = 10;
            enemyHP = 10;
            selectedCommand = BattleCommand::FIGHT;
        }

        break;
        //--------------------------------------------------
    case GameState::BATTLE:
        // バトル画面（背景＋文字）

        DrawBox(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, GetColor(30, 30, 60), TRUE);

        DrawFormatString(20, 20, GetColor(255, 255, 255), "Battle Mode!");
        DrawFormatString(50, 300, GetColor(255, 255, 255), "Player HP: %d", playerHP);
        DrawFormatString(400, 100, GetColor(255, 255, 255), "Enemy HP: %d", enemyHP);

        if (selectedCommand == BattleCommand::FIGHT) {
            DrawFormatString(100, 400, GetColor(255, 255, 255), "> たたかう");
            DrawFormatString(100, 430, GetColor(255, 255, 255), "  にげる");
        }
        else {
            DrawFormatString(100, 400, GetColor(255, 255, 255), "  たたかう");
            DrawFormatString(100, 430, GetColor(255, 255, 255), "> にげる");
        }

        // コマンド選択（↑↓）
        if (CheckHitKey(KEY_INPUT_UP) || CheckHitKey(KEY_INPUT_DOWN)) {
            selectedCommand =
                (selectedCommand == BattleCommand::FIGHT)
                ? BattleCommand::RUN
                : BattleCommand::FIGHT;
            WaitTimer(150);
        }

        // Enterで決定
        if (CheckHitKey(KEY_INPUT_RETURN)) {
            if (selectedCommand == BattleCommand::FIGHT) {
                enemyHP -= attackDamage;
                if (enemyHP < 0) enemyHP = 0;

                playerHP -= attackDamage;
                if (playerHP < 0) playerHP = 0;
            }
            else {
                // にげる
                gameState = GameState::FADE_OUT;
                nextStateAfterFade = GameState::FIELD;
                fadeAlpha = 0.0f;
                encounterTimer = 0.0f;
                encounterCooldown = 2.0f;
            }
            WaitTimer(150);
        }

        // 勝敗判定
        if (enemyHP <= 0) {
            gameState = GameState::RESULT;
            battleResult = BattleResult::WIN;
        }
        else if (playerHP <= 0) {
            gameState = GameState::RESULT;
            battleResult = BattleResult::LOSE;
        }
        break;

    case GameState::RESULT:
        DrawBox(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT,
            GetColor(0, 0, 0), TRUE);

        if (battleResult == BattleResult::WIN) {
            DrawFormatString(200, 200, GetColor(255, 255, 255), "YOU WIN!");
        }
        else {
            DrawFormatString(200, 200, GetColor(255, 255, 255), "YOU LOSE...");
        }

        resultTimer += delta_time;
        if (resultTimer > 2.5f) {
            resultTimer = 0.0f;
            battleResult = BattleResult::NONE;
            gameState = GameState::FADE_OUT;
            nextStateAfterFade = GameState::FIELD;
            fadeAlpha = 0.0f;
            encounterCooldown = 2.0f; // 2秒間エンカウント無効
            encounterTimer = 0.0f;    
        }
        break;

    }

    // フェード用黒幕
    if (fadeAlpha > 0.0f) {
        int col = GetColor(0, 0, 0);
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)fadeAlpha);
        DrawBox(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, col, TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }

    dxe::DrawFpsIndicator({ 10, DXE_WINDOW_HEIGHT - 10 });
    ScreenFlip();
}

//------------------------------------------------------
// 終了処理
void gameEnd() {
    player.reset();
}
