#pragma once

#include "Game.h"

class Text
{
public:
	Text(string content, float x, float y, Game* game);
	void draw();
	string content; // texto
	int x;
	int y;
	int width;
	int height;
	Game* game; // referencia al juego

	//añado variable para cambiar el color del texto de las pistas:
	SDL_Color color;
};

