#pragma once
#include "Layer.h"
#include "Text.h"
#include "Actor.h"
//igual que en Game: declaraciones adelantadas para no romper la copilación por bucle de ref cruzadas:
//#include "Game.h"

class Game;

class GameOverLayer : public Layer {
public:
	GameOverLayer(Game* game);
	void init();
	void processControls() override;
	void update() override;
	void draw() override;

	Actor* background;  //pongo imagen en lugar de text
	Actor* buttonRetry;
	Actor* buttonExit;
};

