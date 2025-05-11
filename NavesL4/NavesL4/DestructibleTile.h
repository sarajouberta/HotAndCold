#pragma once
#include "Actor.h"

class DestructibleTile : public Actor
{
public:
	DestructibleTile(float x, float y, Game* game);

	int timer;  //ampli: 10 updates tras contacto con jugador
	bool steppedOn;   //marcar si tile pisado
	
	void update();

	bool isSteppedOn();
	void markAsSteppedOn();
};

