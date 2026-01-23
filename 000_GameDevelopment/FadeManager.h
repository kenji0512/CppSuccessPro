#pragma once

class FadeManager {
public:
    void StartFadeOut(float sec);
    void StartFadeIn(float sec);
    void Update(float delta);
    void Draw();
    bool IsFinished() const;

private:
    float alpha = 0.0f;
    float speed = 0.0f;
    bool finished = true;
};
