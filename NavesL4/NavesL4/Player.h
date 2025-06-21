#pragma once

#include "Actor.h"
#include "Audio.h"
#include "Animation.h" // incluir animacion 

class Player : public Actor
{
public:
	Player(float x, float y, Game* game);
	//Projectile* shoot();
	void update();
	void peck();  //picar el suelo

	void moveX(float axis);
	void moveY(float axis);
	void draw(float scrollX = 0, float scrollY = 0) override; // Va a sobrescribir
	
	//En lugar de vidas, habrá un temporizador en el juego

	int orientation;
	int state;

	//velocidades para caminar/correr:
	//int walkingSpeed = 3;
	float speed;
	int runningSpeed = 6;

	Animation* aIdleRight;
	Animation* aIdleLeft;
	Animation* aIdleUp;
	Animation* aIdleDown;
	
	Animation* aRunningRight;
	Animation* aRunningLeft;
	Animation* aRunningUp;
	Animation* aRunningDown;

	//Animation* aPeckingDown;  //picotazo hacia abajo
	Animation* aPeckingRight;
	Animation* aPeckingLeft;

	//posiciones estáticas:
	Animation* aStandingRight;
	Animation* aStandingLeft;
	Animation* aStandingUp;
	Animation* aStandingDown;

	Animation* animation; //referencia a la animación mostrada
	//Audio* audioPeck;
	//Audio* audioPickingChocoGraphy;
	
};

