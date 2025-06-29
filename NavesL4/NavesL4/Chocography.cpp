#include "Chocography.h"

//inicializa la posición y el estado "no encontrada"
Chocography::Chocography(float x, float y, Game* game)
    : Actor("res/chocography_icon.png", x, y, 20, 20, game), encontrada(false) {
    //no se dibuja: ""
}

void Chocography::picar() {
    encontrada = true;
}

bool Chocography::isEncontrada() {
    return encontrada;
}

//no se dibuja porque es invisible: sobrecarga vacía
//void Chocography::draw(float scrollX, float scrollY) {
    //es invisible
//}

