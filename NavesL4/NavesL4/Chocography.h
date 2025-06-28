#pragma once
#include "Actor.h"
class Chocography : public Actor
{
public:
    bool encontrada;

    float choco_width = 20.0;   //.png de 20x20 vacío
    float choco_height = 20.0;

    Chocography(float x, float y, Game* game);

    void picar();
    bool isEncontrada();

    void draw(float scrollX, float scrollY
    ); //sobrecargado para que NO se dibuje (vacío)
};

