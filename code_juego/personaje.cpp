#include "Personaje.h"
Personaje::Personaje(float px, float py, float w, float h, int vidaInicial)
    : x(px), y(py), ancho(w), alto(h), vida(vidaInicial), vx(0), vy(0)
{
}
void Personaje::mover(float dx, float dy) {
    x += dx;
    y += dy;
}

void Personaje::recibirDanio(int d) {
    vida -= d;
    if (vida < 0) vida = 0;
}
