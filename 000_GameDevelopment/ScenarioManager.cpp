#include "ScenarioManager.h"
#include <fstream>
#include <sstream>

void ScenarioManager::LoadCSV(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        // ファイルが開けない時に警告を出す（DXライブラリ用）
        printfDx("Error: Cannot open %s\n", filename.c_str()); 
        return;
    }
    std::string lineStr;
    lines.clear();

    if (!file.is_open()) return;

    std::getline(file, lineStr);

    while (std::getline(file, lineStr)) {
        if (!lineStr.empty() && lineStr.back() == '\r') lineStr.pop_back();
        if (lineStr.empty()) continue;

        std::stringstream ss(lineStr);
        std::string item;
        std::vector<std::string> row;

        // カンマ区切りで分割
        while (std::getline(ss, item, ',')) {
            row.push_back(item);
        }

        if (row.size() >= 3) {
            ScenarioLine data;
            data.type = (row[0] == "TALK") ? CommandType::TALK : CommandType::EVENT;
            data.caption = row[1];
            data.body = row[2];
            data.nextEvent = (row.size() > 3) ? row[3] : "";
            if (row.size() >= 4) {
                data.power = std::stoi(row[3]);
            }
            else {
                data.power = 0;
            }            lines.push_back(data);
        }
    }
    file.close();

    if (!lines.empty()) {
        currentIndex = 0;
        charIndex = 0;
        typeTimer = 0.0f; // タイマー初期化漏れ修正
        state = MessageState::TYPING;
    }
}

void ScenarioManager::Update(float delta) {
    if (state == MessageState::IDLE || currentIndex >= lines.size()) return;

    auto& current = lines[currentIndex];

    if (state == MessageState::TYPING) {
        typeTimer += delta;
        if (typeTimer > 0.05f) {
            typeTimer = 0.0f;
            charIndex++;
            if (charIndex >= current.body.size()) {
                state = MessageState::WAIT_INPUT;
            }
        }
    }

    if (state == MessageState::WAIT_INPUT) {
        if (CheckHitKey(KEY_INPUT_RETURN)) {
            currentIndex++;
            charIndex = 0;
            if (currentIndex >= lines.size()) {
                state = MessageState::IDLE; // シナリオ終了
            }
            else {
                state = MessageState::TYPING;
            }
            WaitTimer(200);
        }
    }
}
void ScenarioManager::Draw() {
    if (state == MessageState::IDLE) return;

    auto& current = lines[currentIndex];

    int safeCharIndex = (charIndex > (int)current.body.size()) ? (int)current.body.size() : charIndex;

    std::string display = current.body.substr(0, safeCharIndex);
    DrawString(50, 370, display.c_str(), GetColor(255, 255, 255));

    // ウィンドウ
    DrawBox(30, 350, 610, 450, GetColor(0, 0, 0), TRUE);
    DrawBox(30, 350, 610, 450, GetColor(255, 255, 255), FALSE);

    // キャプション
    DrawString(40, 330, current.caption.c_str(), GetColor(255, 255, 255));

    // 本文（1文字送り対応）
    DrawString(50, 370, display.c_str(), GetColor(255, 255, 255));

    if (state == MessageState::WAIT_INPUT) {
        if ((GetNowCount() / 500) % 2 == 0) {
            DrawString(580, 420, "▼", GetColor(255, 255, 255));
        }
    }
}