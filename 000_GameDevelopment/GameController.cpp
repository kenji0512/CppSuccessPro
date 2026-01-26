#include "GameController.h"

void GameController::Init()
{
    state = GameState::FIELD;
    nextState = GameState::FIELD;

    field = std::make_unique<FieldManager>();
    battle = std::make_unique<BattleManager>();
    fade = std::make_unique<FadeManager>();
    scenarioMgr = std::make_unique<ScenarioManager>();
    battle->SetScenarioManager(scenarioMgr.get()); 
}

void GameController::Update(float delta)
{
    switch (state)
    {
    case GameState::FIELD:
        field->Update(delta);
        if (field->ShouldEncounter()) {
            fade->StartFadeOut(2.5f);
            nextState = GameState::BATTLE;
            state = GameState::FADE_OUT;
        }
        break;

    case GameState::FADE_OUT:
        fade->Update(delta);
        if (fade->IsFinished()) {
            state = nextState;

            if (nextState == GameState::BATTLE)
                battle->Start();

            fade->StartFadeIn(2.5f);
            state = GameState::FADE_IN;
        }
        break;

    case GameState::FADE_IN:
        fade->Update(delta);
        if (fade->IsFinished()) {
            state = nextState;
        }
        break;

    case GameState::BATTLE:
        battle->Update(delta);
        if (battle->IsFinished()) {
            field->ResetEncounter(); 
            fade->StartFadeOut(1.5f);
            nextState = GameState::FIELD;
            state = GameState::FADE_OUT;
        }
        break;
    }
}

void GameController::Draw()
{
    // 基本の描画
    if (state == GameState::FIELD) {
        field->Draw();
    }
    else if (state == GameState::BATTLE) {
        battle->Draw();
    }
    // フェードアウト中：遷移「前」の画面を出す
    else if (state == GameState::FADE_OUT) {
        if (nextState == GameState::BATTLE) field->Draw(); // バトルへ行く前はフィールドを出す
        else battle->Draw(); // フィールドへ戻る前はバトルを出す
    }
    // フェードイン中：遷移「後」の画面を出す
    else if (state == GameState::FADE_IN) {
        if (nextState == GameState::BATTLE) battle->Draw(); // バトル開始中
        else field->Draw(); // フィールド復帰中
    }

    // 最後にフェードを重ねる
    fade->Draw();
}

void GameController::Finalize()
{
    field.reset();
    battle.reset();
    fade.reset();
}
