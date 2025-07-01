#pragma once
#include "Layer.h"
#include "Actor.h"

class Game;  // Declaración adelantada para evitar referencias cruzadas

class FinishedLevelLayer : public Layer {
public:
    FinishedLevelLayer(Game* game);
    void init();
    void processControls() override;
    void update() override;
    void draw() override;

private:
    Actor* background;
    Actor* buttonNext;
    Actor* buttonExit;
};