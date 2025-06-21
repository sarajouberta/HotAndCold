#include "FriendMoguri.h"

FriendMoguri::FriendMoguri(float x, float y, Game* game)
	: Actor("res/moguri.png", x, y, 36, 40, game) {

	state = game->stateMoving;

	aMoving = new Animation("res/moguri_movimiento.png", width, height,
		108, 40, 6, 3, true, game);
	animation = aMoving;

	vx = 1;
	/*vxIntelligence = -1;
	vx = vxIntelligence;*/

}

void FriendMoguri::update() {
	// Actualizar la animación
	bool endAnimation = animation->update();


	// Acabo la animación, no sabemos cual
	/*if (endAnimation) {
		// Estaba muriendo
		if (state == game->stateDying) {
			state = game->stateDead;
		}
	}*/

	if (state == game->stateMoving) {
		animation = aMoving;
	}

	else {
		vx = 0;
	}

}


void FriendMoguri::draw(float scrollX, float scrollY) {
	animation->draw(x - scrollX, y - scrollY);
}
