#include "Player.h"

Player::Player(float x, float y, Game* game)         //PONER DATOS CORRECTOS A TODOE STOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
	: Actor("res/moguri.png", x, y, 35, 35, game) {

	speed = runningSpeed; // velocidad por defecto
	
	orientation = game->orientationRight;
	state = game->stateMoving;
	
	//audioPeck = new Audio("res/.wav", false);
	//audioPickingChocoGraphy = new Audio("res/.wav", false);

	aIdleRight = new Animation("res/choco_camina_dcha.png", width, height,
		89, 30, 10, 3, true, game);
	aIdleLeft = new Animation("res/choco_camina_izda.png", width, height,
		89, 30, 10, 3, true, game);
	aIdleUp = new Animation("res/choco_camina_arriba.png", width, height,
		43, 30, 10, 2, true, game);
	aIdleDown = new Animation("res/choco_camina_abajo.png", width, height,
		145, 30, 10, 5, true, game);
	aRunningRight = new Animation("res/choco_corre_dcha.png", width, height,
		147, 40, 10, 5, true, game);
	aRunningLeft = new Animation("res/choco_corre_izda.png", width, height,
		151, 30, 10, 5, true, game);
	//correr arriba/abajo igual que caminar (cambia la velocidad, de momento)    REVISAAAAAAR
	aRunningUp = new Animation("res/choco_camina_arriba.png", width, height,
		43, 30, 10, 2, true, game);
	aRunningDown = new Animation("res/choco_camina_abajo.png", width, height,
		43, 30, 10, 2, true, game);

	//posiciones estáticas: (bucle false y 1 frame)
	aStandingRight = new Animation("res/choco_quieto_dcha.png", width, height,
		27, 28, 0, 1, false, game);
	aStandingLeft = new Animation("res/choco_quieto_izda.png", width, height,
		29, 28, 0, 1, false, game);
	aStandingUp = new Animation("res/choco_quieto_arriba.png", width, height,
		25, 31, 0, 1, false, game);
	aStandingDown = new Animation("res/choco_quieto_abajo.png", width, height,
		28, 30, 0, 1, false, game);


	/*aPeckingDown = new Animation(".png", width, height,
		160, 40, 6, 4, false, game); */
	aPeckingRight = new Animation("res/choco_pica_dcha.png", width, height,                //AJUSTAR ANIMACIONES PICAR ARRIBA/ABAJO
		90, 30, 0, 3, false, game);
	aPeckingLeft = new Animation("res/choco_pica_izda.png", width, height,
		90, 30, 0, 3, false, game);


	animation = aStandingRight;
}

/*NOTA para mí: por qué resulta que si no trataba el cambio de la variable loop en las animaciones  de picar
* al picar se cambiaba al statePecking pero no regresaba a stateMoving al finalizar su animación:
* 
Conclusión final: el error viene de la clase Animation y su método 
Mi intuición era casi correcta desde el principio: asignar currentFrame = 0 debería bastar si la animación no se corrompe.
Pero en este caso:

	Al llegar a totalFrames, currentFrame queda fuera de rango

	update() deja de avanzar frames si loop == false y no se hace una limpieza completa

Por eso añadir loop = true temporalmente lo arregla, pero es un parche, no la raíz de la solución
(lo ideal es resetear completamente el estado de la animación antes de volverla a usar, aunque no tenga bucle)
* 
* Si  Animation hubiera gestionado bien su propio estado interno, incluyendo evitar que currentFrame se pase de totalFrames - 1
si loop == false, o bien reiniciar automáticamente cuando quieras reutilizar la animación, o tener un método tipo reset() que devuelva
currentFrame y updateTime a cero == el código original en Player habría funcionado bien, sin necesidad de tocar loop explícitamente en peck().
*/

void Player::update() {
	//si está picando:
	if (state == game->statePecking) {
		if (orientation == game->orientationLeft || orientation == game->orientationDown) {
			animation = aPeckingLeft;
			aPeckingLeft->update();
			cout << "tras statePecking (debería ser 2): " << state << endl;
			if (aPeckingLeft->hasEnded()) {
				state = game->stateMoving;
				aPeckingLeft->loop = true;  // Habilita el bucle solo para las animaciones de movimiento

				cout << "vuelta a stateMoving (debería ser 1): " << state << endl;
			}
		}
		else {
			animation = aPeckingRight;
			aPeckingRight->update();
			if (aPeckingRight->hasEnded()) {
				aPeckingRight->loop = true; // Habilita el bucle solo para las animaciones de movimiento
				state = game->stateMoving;
			}
		}
	}
	//si se mueve:
	else if (state == game->stateMoving) {
		cout << "movimiento stateMoving: (debería ser 1): " << state << endl;
		if (vx > 0) orientation = game->orientationRight;
		else if (vx < 0) orientation = game->orientationLeft;
		else if (vy < 0) orientation = game->orientationUp;
		else if (vy > 0) orientation = game->orientationDown;
		//si corre (por defecto):
		if (abs(vx) >= runningSpeed || abs(vy) >= runningSpeed) {    
			switch (orientation) {
			case 1: animation = aRunningRight; break;
			case 2: animation = aRunningLeft; break;
			case 3: animation = aRunningUp; break;
			case 4: animation = aRunningDown; break;
			}
		}
		//si camina (caminará cuando se ralentice x overlap con enemigo molesto)
		else if (vx != 0 || vy != 0) {
			switch (orientation) {
			case 1: animation = aIdleRight; break;
			case 2: animation = aIdleLeft; break;
			case 3: animation = aIdleUp; break;
			case 4: animation = aIdleDown; break;
			}
		}
		else {  //si está quieto:
			switch (orientation) {
			case 1: animation = aStandingRight; break;
			case 2: animation = aStandingLeft; break;
			case 3: animation = aStandingUp; break;
			case 4: animation = aStandingDown; break;
			}
		}
	}

}

void Player::moveX(float axis) {
	vx = axis * speed;            //CORRE X DEFECTOOOOOOOO
}

void Player::moveY(float axis) {
	vy = axis * speed;  //ya no usa runningSpeed al añadir animación de caminar(colisión con molesto)
}

void Player::peck() {
	if (state != game->statePecking) {
		state = game->statePecking;

		if (orientation == game->orientationLeft || orientation == game->orientationDown) {
			animation = aPeckingLeft;
			aPeckingLeft->currentFrame = 0;
			aPeckingLeft->loop = false; //no debería repetirse
		}
		else {
			//en otro caso, picar hacia la derecha
			animation = aPeckingRight;
			aPeckingRight->currentFrame = 0;
			aPeckingRight->loop = false; //no debería repetirse
		}
		//audioPeck->play();                            //COMPROBAR SI SE EJECUTA 1 VEZ O MÁS !!!
	}
}


void Player::draw(float scrollX, float scrollY) {
	animation->draw(x - scrollX, y - scrollY);

}


/*mi código inicialmente, porque sigo sin entender por qué no funcionaba antes de ponerle el control de loop manual:*/
/*
void Player::update() {

	if (state == game->statePecking) {
		if (orientation == game->orientationLeft || orientation == game->orientationDown) {
			animation = aPeckingLeft;
			aPeckingLeft->update();
			cout << "tras statePecking (debería ser 2): " << state << endl;
			if (aPeckingLeft->hasEnded()) {
				state = game->stateMoving;
				cout << "vuelta a stateMoving (debería ser 1): " << state << endl;
			}
		}
		else {
			animation = aPeckingRight;
			aPeckingRight->update();
			if (aPeckingRight->hasEnded()) {
				state = game->stateMoving;
			}
		}
	}
	else if (state == game->stateMoving) {
		cout << "movimiento stateMoving: (debería ser 1): " << state << endl;
		if (vx > 0) orientation = game->orientationRight;
		else if (vx < 0) orientation = game->orientationLeft;
		else if (vy < 0) orientation = game->orientationUp;
		else if (vy > 0) orientation = game->orientationDown;

		if (abs(vx) >= runningSpeed || abs(vy) >= runningSpeed) {    //si corre (por defecto)                       //MODIFICAR VELOCIDAD ENTRE CAMINAR Y CORRER !!!!
			switch (orientation) {
			case 1: animation = aRunningRight; break;
			case 2: animation = aRunningLeft; break;
			case 3: animation = aRunningUp; break;
			case 4: animation = aRunningDown; break;
			}
		}
		else if (vx != 0 || vy != 0) { //si camina (caminará cuando se ralentice x chocar con npc malo !!!!!!)
			switch (orientation) {
			case 1: animation = aIdleRight; break;
			case 2: animation = aIdleLeft; break;
			case 3: animation = aIdleUp; break;
			case 4: animation = aIdleDown; break;
			}
		}
		else {  //si está quieto:
			switch (orientation) {
			case 1: animation = aStandingRight; break;
			case 2: animation = aStandingLeft; break;
			case 3: animation = aStandingUp; break;
			case 4: animation = aStandingDown; break;
			}
		}
	}

}

void Player::peck() {
	if (state != game->statePecking) {
		state = game->statePecking;

		if (orientation == game->orientationLeft || orientation == game->orientationDown) {
			animation = aPeckingLeft;
			aPeckingLeft->currentFrame = 0;
		}
		else {
			//en otro caso, picar hacia la derecha
			animation = aPeckingRight;
			aPeckingRight->currentFrame = 0;
		}
		//audioPeck->play();                            //COMPROBAR SI SE EJECUTA 1 VEZ O MÁS !!!
	}
}
*/
