#pragma once
#include "Actor.h"
#include "Animation.h" 


class FriendMoguri : public Actor
{
public:
	FriendMoguri(float x, float y, Game* game);
	void draw(float scrollX = 0, float scrollY = 0) override; // Va a sobrescribir
	void update();
											      
	int state;
	Animation* aMoving;
	Animation* animation; // Referencia a la animación mostrada

	//limitar interacciones con player:
	void use();
	int usesLeft = 4; //por defecto, 4 de momento
	bool deleted = false; // para marcarlo para borrar

};

