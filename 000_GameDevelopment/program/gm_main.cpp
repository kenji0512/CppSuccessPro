#include <dxe.h>
#include "../GameController.h"

GameController game;
BattleManager battle;
ScenarioManager scenarioMgr;

void gameStart()
{
    game.Init();
    battle.SetScenarioManager(&scenarioMgr);
    battle.Start();
}

void gameMain(float deltaTime)
{
    game.Update(deltaTime);
    game.Draw();
    battle.Update(deltaTime);
    battle.Draw();
}

void gameEnd()
{
    game.Finalize();
}