#pragma once

#include "Actor.h"
#include "Animation.h"

class CollectibleItem : public Actor
{
public:
	CollectibleItem(float x, float y, Game* game);
	void draw(float scrollX = 0, float scrollY = 0) override; // Va a sobrescribir
	void update();

	Animation* aMoving;
	Animation* animation; // Referencia a la animación mostrada

	bool collected;

	bool isCollected();
	void markAsCollected();
};
