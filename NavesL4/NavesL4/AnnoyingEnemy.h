#pragma once

#include "Actor.h"

class AnnoyingEnemy : public Actor {
public:
	AnnoyingEnemy(float x, float y, Game* game);
	void draw(float scrollX = 0, float scrollY = 0) override;
	void update(Actor* target); //recibe el jugador como objetivo

private:
	float speed = 1.0f;              //velocidad (bastante lento)
	float minDistance = 50.0f;       //distancia mínima a mantener (no choque directo con jugador)
	//float triggerDistance = 200.0f;  // distancia min. para perseguir al jugador
};

