#include "GameOverLayer.h"
#include "Game.h"
#include "GameLayer.h"

GameOverLayer::GameOverLayer(Game* game) : Layer(game) {
	init();
}

void GameOverLayer::init() {
	//textGameOver = new Text("hola", WIDTH * 0.5, HEIGHT * 0.2, game);
	//textGameOver->content = "¡Oh no!¡Se acabó el tiempo!";
	background = new Actor("res/game_over_menu1.png", WIDTH * 0.5, HEIGHT * 0.5, WIDTH, HEIGHT, game);

	buttonRetry = new Actor("res/boton_reiniciar.png", WIDTH * 0.35, HEIGHT * 0.6, 232, 72, game);
	buttonExit = new Actor("res/boton_salir.png", WIDTH * 0.65, HEIGHT * 0.6, 232, 72, game);
}

void GameOverLayer::processControls() {  //sobreescrito para poder reiniciar la partida o salir 
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_MOUSEBUTTONDOWN) {
			float x = event.button.x / game->scaleLower;
			float y = event.button.y / game->scaleLower;
			//si se pulsa "reintentar"
			if (buttonRetry->containsPoint(x, y)) {
				game->nextLayer = new GameLayer(game); //reiniciar partida si se pulsa sobre reiniciar
				//game->state = "playing";
			}
			else if (buttonExit->containsPoint(x, y)) { //si se pulsa salir
				game->layer = game->menuLayer;
				//game->state = "menu";
												
			}
		}
	}
}

void GameOverLayer::update() {
	//no se hace nada, ya que es estática
}

void GameOverLayer::draw() {
	//textGameOver->draw();
	background->draw();
	buttonRetry->draw();
	buttonExit->draw();
	
	SDL_RenderPresent(game->renderer);
}
