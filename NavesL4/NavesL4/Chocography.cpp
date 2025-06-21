#include "Chocography.h"

//inicializa la posición y el estado "no encontrada"
Chocography::Chocography(float x, float y, Game* game)
    : Actor("", x, y, choco_width, choco_height, game), encontrada(false) {
    //no se dibuja: ""
}

void Chocography::picar() {
    encontrada = true;
}

bool Chocography::isEncontrada() {
    return encontrada;
}

//no se dibuja porque es invisible: sobrecarga vacía
void Chocography::draw(float scrollX, float scrollY) {
    //es invisible
}

