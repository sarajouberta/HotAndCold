#include "VictoryLayer.h"
#include "Game.h"
#include "GameLayer.h"

VictoryLayer::VictoryLayer(Game* game) : Layer(game) {
    init();
}

void VictoryLayer::init() {
    background = new Actor("res/victory_menu1.png", WIDTH * 0.5, HEIGHT * 0.5, WIDTH, HEIGHT, game);
    buttonRetry = new Actor("res/boton_reiniciar.png", WIDTH * 0.35, HEIGHT * 0.6, 232, 72, game);
    buttonExit = new Actor("res/boton_salir.png", WIDTH * 0.65, HEIGHT * 0.6, 232, 72, game);
}

void VictoryLayer::processControls() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            float x = event.button.x / game->scaleLower;
            float y = event.button.y / game->scaleLower;

            if (buttonRetry->containsPoint(x, y)) {
                game->gameLayer = new GameLayer(game);  //necesit la declaración aquí
                game->layer = game->gameLayer;
            }
            else if (buttonExit->containsPoint(x, y)) {
                game->layer = game->menuLayer;
            }
        }
    }
}

void VictoryLayer::update() {
    // pantalla estática
}

void VictoryLayer::draw() {
    background->draw();
    buttonRetry->draw();
    buttonExit->draw();
    SDL_RenderPresent(game->renderer);
}
