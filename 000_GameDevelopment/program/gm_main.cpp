#include <dxe.h>
#include "../GameController.h"

GameController game;

void gameStart()
{
    game.Init();
}

void gameMain(float deltaTime)
{
    game.Update(deltaTime);
    game.Draw();
}

void gameEnd()
{
    game.Finalize();
}
//色々バグ起こしてる
