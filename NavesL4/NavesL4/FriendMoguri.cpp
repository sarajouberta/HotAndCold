#include "FriendMoguri.h"

FriendMoguri::FriendMoguri(float x, float y, Game* game)
	: Actor("res/moguri.png", x, y, 36, 40, game) {

	state = game->stateMoving;

	aMoving = new Animation("res/moguri_movimiento.png", width, height,
		46, 31, 5, 2, true, game);
	animation = aMoving;

	vx = 0;  //quiero que se mueva solo en el sitio
	/*vxIntelligence = -1;
	vx = vxIntelligence;*/

}

void FriendMoguri::update() {
	//actualizar la animación:
	bool endAnimation = animation->update();

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
//se limitan las interacciones con el moguri amigo: tras 3 veces, desaparecen
void FriendMoguri::use() {
	usesLeft--;
	if (usesLeft <= 0) {
		deleted = true;
	}
}

