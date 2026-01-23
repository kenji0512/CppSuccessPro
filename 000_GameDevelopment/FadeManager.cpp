#include "FadeManager.h"
#include <dxe.h>

void FadeManager::StartFadeOut(float sec)
{
    alpha = 0;
    speed = 255.0f / sec;
    finished = false;
}

void FadeManager::StartFadeIn(float sec)
{
    alpha = 255;
    speed = -255.0f / sec;
    finished = false;
}

void FadeManager::Update(float delta)
{
    if (finished) return;

    alpha += speed * delta;
    if (alpha <= 0 || alpha >= 255) {
        alpha = (alpha < 0) ? 0 : 255;
        finished = true;
    }
}

void FadeManager::Draw()
{
    if (!finished) {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)alpha);
        DrawBox(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT,
            GetColor(0, 0, 0), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }
}

bool FadeManager::IsFinished() const
{
    return finished;
}
