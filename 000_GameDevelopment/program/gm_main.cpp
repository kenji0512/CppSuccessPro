#include <dxe.h>
#include "../GameController.h"

GameController game;

void gameStart()
{
    game.Init(); // ‚±‚Ì’†‚Å battle ‚à scenarioMgr ‚à‰Šú‰»‚³‚ê‚Ü‚·
}

void gameMain(float deltaTime)
{
    // game.Update ‚Ì’†‚ÅAó‘Ô‚É‡‚í‚¹‚Ä field ‚© battle ‚©‚ğ©“®‚Å‘I‚ñ‚Å‚­‚ê‚Ü‚·
    game.Update(deltaTime);
    game.Draw();
}

void gameEnd()
{
    game.Finalize();
}