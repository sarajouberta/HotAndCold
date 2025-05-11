#include "DestructibleTile.h"

DestructibleTile::DestructibleTile(float x, float y, Game* game)
	: Actor("res/bloque_metal.png", x, y, 40, 32, game) {

	steppedOn = false;
	timer = 30; //prueba con 10 updates
}


bool DestructibleTile::isSteppedOn() {
	return steppedOn;
}

void DestructibleTile::markAsSteppedOn(){
	steppedOn = true;
}

void DestructibleTile::update() {
    // Check if the tile has been stepped on
    if (steppedOn) {
        // Decrement the timer if it has been activated
        if (timer > 0) {
            timer--;
        }
    }
}
