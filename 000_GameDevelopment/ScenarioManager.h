#pragma once
#include <dxe.h>
#include <string>
#include <vector>

enum class CommandType { TALK, EVENT };
enum class MessageState {
    IDLE,        // 何もしていない
    TYPING,      // 1文字ずつ表示中
    WAIT_INPUT,  // 全文表示され、ボタン押し待ち
    COMPLETED    // シナリオ終了
};
struct ScenarioLine {
    CommandType type;
    std::string caption;
    std::string body;
    std::string nextEvent;
    int power = 0;
};
class ScenarioManager
{
public:
    void LoadCSV(const std::string& filename); // ファイルから読み込み
    void Update(float delta);               // 非同期更新
    void Draw();                            // ウィンドウと文字の描画
    //void Next();                            // 次のページへ（入力があったら呼ぶ）

    // 外部（BattleManagerなど）からメッセージを開始するための関数
    // void StartScenario(const std::vector<ScenarioLine>& newLines);
    bool IsActive() const { return state != MessageState::IDLE; }
    int GetCurrentPower() const {
        if (currentIndex < lines.size()) {
            return lines[currentIndex].power;
        }
        return 0;
    }

private:
    std::vector<ScenarioLine> lines;     // シナリオデータのリスト
    int currentIndex = 0;                   // 現在のコマンド番号
    MessageState state = MessageState::IDLE;

    std::string displayBody;                // 現在画面に表示されている文字列
    float typeTimer = 0.0f;                 // 文字送り用タイマー
    int charIndex = 0;                      // 何文字目まで表示したか
};


