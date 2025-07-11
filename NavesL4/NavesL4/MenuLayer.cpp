#include "MenuLayer.h"
#include "GameLayer.h"


MenuLayer::MenuLayer(Game* game)
	: Layer(game) {
	init();
	gamePad = SDL_GameControllerOpen(0);
}

void MenuLayer::init() {
	// Fondo normal, sin velocidad:
	background = new Background("res/hotNcold_fondo.png", WIDTH * 0.5, HEIGHT * 0.5, game);
	//m�n: 1 bot�n:
	button = new Actor("res/boton_jugar.png", WIDTH * 0.5, HEIGHT * 0.7, 232, 72, game);
}

void MenuLayer::draw() {
	background->draw();
	button->draw();

	//importante: si no, no se actualiza render de pantalla
	SDL_RenderPresent(game->renderer); //NO PUEDE FALTAR
}

void MenuLayer::processControls() {
	// obtener controles
	SDL_Event event;
	//x si hay varios mecanismos de entrada:
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_CONTROLLERDEVICEADDED) {
			gamePad = SDL_GameControllerOpen(0);
			if (gamePad == NULL) {
				cout << "error en GamePad" << endl;
			}
			else {
				cout << "GamePad conectado" << endl;
			}
		}
		// Cambio autom�tico de input
		// PONER el GamePad:
		if (event.type == SDL_CONTROLLERBUTTONDOWN || event.type == SDL_CONTROLLERAXISMOTION) {
			game->input = game->inputGamePad;
		}
		if (event.type == SDL_KEYDOWN) {
			game->input = game->inputKeyboard;
		}
		if (event.type == SDL_MOUSEBUTTONDOWN) {
			game->input = game->inputMouse;
		}

		// Procesar Mando:
		if (game->input == game->inputGamePad) {  // gamePAD
			gamePadToControls(event);
		}
		//procesar Teclas:
		if (game->input == game->inputKeyboard) {
			keysToControls(event);
		}
		if (game->input == game->inputMouse) {
			mouseToControls(event);
		}
	}

	//procesar controles, solo tiene uno
	if (controlContinue) {
		// Cambia la capa: cambio a solo indicar que es la nextLayer, y que lo gestione Game para que no haya crash
		game->layer = game->gameLayer;
		//controlContinue = false;
		//game->gameLayer = nullptr;
		game->currentLevel = 0;  //reiniciar tambi�n el nivel
		//crear partida nueva: 
		game->gameLayer = new GameLayer(game);
		game->layer = game->gameLayer;

		controlContinue = false;
	}
}

void MenuLayer::keysToControls(SDL_Event event) {
	if (event.type == SDL_KEYDOWN) {
		int code = event.key.keysym.sym;
		// Pulsada
		switch (code) {
		case SDLK_ESCAPE: // derecha
			game->loopActive = false;
			break;
		case SDLK_1:
			game->scale();
			break;
		case SDLK_SPACE: // dispara
			controlContinue = true;
			break;
		}
	}
}

void MenuLayer::mouseToControls(SDL_Event event) {
	// Modificaci�n de coordenadas por posible escalado
	float motionX = event.motion.x / game->scaleLower;
	float motionY = event.motion.y / game->scaleLower;

	// Cada vez que hacen click
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (button->containsPoint(motionX, motionY)) {
			controlContinue = true;
		}
	}
}

void MenuLayer::gamePadToControls(SDL_Event event) {
	// Leer los botones
	bool buttonA = SDL_GameControllerGetButton(gamePad, SDL_CONTROLLER_BUTTON_A);

	if (buttonA) {
		controlContinue = true;
	}
}
