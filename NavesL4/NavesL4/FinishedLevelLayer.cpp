#include "FinishedLevelLayer.h"
#include "Game.h"
#include "GameLayer.h"

FinishedLevelLayer::FinishedLevelLayer(Game* game) : Layer(game) {
    init();
}

void FinishedLevelLayer::init() {
    background = new Actor("res/victory_menu1.png", WIDTH * 0.5, HEIGHT * 0.5, WIDTH, HEIGHT, game);
    buttonNext = new Actor("res/boton_siguiente_nivel.png", WIDTH * 0.35, HEIGHT * 0.6, 232, 72, game);
    buttonExit = new Actor("res/boton_salir.png", WIDTH * 0.65, HEIGHT * 0.6, 232, 72, game);
}

void FinishedLevelLayer::processControls() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            float x = event.button.x / game->scaleLower;
            float y = event.button.y / game->scaleLower;

            if (buttonNext->containsPoint(x, y)) {
                game->gameLayer = new GameLayer(game);  //necesit la declaración aquí
                game->layer = game->gameLayer;
            }
            else if (buttonExit->containsPoint(x, y)) {
                game->layer = game->menuLayer;
            }
        }
    }
}

void FinishedLevelLayer::update() {
    // pantalla estática
}

void FinishedLevelLayer::draw() {
    background->draw();
    buttonNext->draw();
    buttonExit->draw();
    SDL_RenderPresent(game->renderer);
}