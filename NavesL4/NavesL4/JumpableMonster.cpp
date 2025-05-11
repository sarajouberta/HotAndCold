#include "JumpableMonster.h"

JumpableMonster::JumpableMonster(float x, float y, Game* game)
	: Actor("res/enemigo_saltable.png", x, y, 27, 35, game) {
	
	jumpedOn = false;
}


void JumpableMonster::jumpOnIt() {
	jumpedOn = true;
}

bool JumpableMonster::isJumpedOn() {
	return jumpedOn;
}

bool JumpableMonster::isBeingJumpedOnByPlayer(Player* player) {
	//player está sobre monstruo si:
	//1. la y del jugador está en la y superior del monstruo
	//2. la x del jugador está entre borde izdo(x-ancho/2) y borde dcho(x+ancho/2)
	if ((player->y + player->height / 2) == (y - height / 2) &&
		((player->x + player->width / 2) >= (x - width / 2) &&
			(player->x - player->width / 2) <= (x + width / 2))) {
		return true;
	}
}