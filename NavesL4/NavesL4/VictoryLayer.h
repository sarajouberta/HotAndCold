#pragma once
#include "Layer.h"
#include "Actor.h"

class Game;  // Declaración adelantada para evitar referencias cruzadas

class VictoryLayer : public Layer {
public:
    VictoryLayer(Game* game);
    void init();
    void processControls() override;
    void update() override;
    void draw() override;

private:
    Actor* background;
    Actor* buttonRetry;
    Actor* buttonExit;
};
