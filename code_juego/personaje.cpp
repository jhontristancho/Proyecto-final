#include "personaje.h"
Personaje::Personaje(float px, float py, float w, float h, int vidaInicial)
    : x(px), y(py), vx(0), vy(0), ancho(w), alto(h), vida(vidaInicial) {}
void Personaje::mover(float dx, float dy) {
    x += dx;
    y += dy;
}

void Personaje::recibirDanio(int d) {
    vida -= d;
    if (vida < 0) vida = 0;
}
