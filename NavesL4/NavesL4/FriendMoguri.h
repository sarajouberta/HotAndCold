#pragma once
#include "Actor.h"
#include "Animation.h" 


class FriendMoguri : public Actor
{
public:
	FriendMoguri(float x, float y, Game* game);
	void draw(float scrollX = 0, float scrollY = 0) override; // Va a sobrescribir
	void update();
											       //TEMPORALMENTE COMENTADO PARA PRUEBA ANINAMCIONES
	int state;
	Animation* aMoving;
	Animation* animation; // Referencia a la animación mostrada
};

