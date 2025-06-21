#pragma once
#include "Actor.h"
class Chocography : public Actor
{
public:
    bool encontrada;

    float choco_width;
    float choco_height;

    Chocography(float x, float y, Game* game);

    void picar();
    bool isEncontrada();

    void draw(float scrollX, float scrollY
    ); //sobrecargado para que NO se dibuje (vacío)
};

