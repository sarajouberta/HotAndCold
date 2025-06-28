#include "AnnoyingEnemy.h"
#include <cmath>

AnnoyingEnemy::AnnoyingEnemy(float x, float y, Game* game)
	: Actor("res/enemigo_molesto.png", x, y, 40, 40, game) {
	// Imagen fija: sin animaciones
	vx = 0;  //usar el mov que usa también space: no puedo sobrescribir con x e y porque hay conflicto
	vy = 0;
}

void AnnoyingEnemy::update(Actor* target) {
	if (!target) return;

	// Calcular vector dirección hacia el jugador:
	float dx = target->x - x;
	float dy = target->y - y;
	float distance = sqrt(dx * dx + dy * dy);

	//si está demasiado cerca: no busca estrellarse directamente:
	if ( distance > minDistance) {
		//normalizar dirección
		float dirX = dx / distance;
		float dirY = dy / distance;

		float speed = 1.2f;  //ajustar velocidad

		//mover al enemigo hacia el jugador
		x += dirX * speed;
		y += dirY * speed;  //y cambiado x baseY xq debaja de perseguir al jugador
	}


}

void AnnoyingEnemy::draw(float scrollX, float scrollY) {
	Actor::draw(scrollX, scrollY);
}
