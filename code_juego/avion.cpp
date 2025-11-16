#include "Avion.h"
Avion::Avion(float px, float py)
    : Personaje(px, py, 48.0f, 32.0f, 100),
    velocidadHorizontal(250.0f),
    suavizado(0.1f)
{
}
void Avion::moverDerecha(float dt) {
    if (!estaVivo()) return;
    float objetivo = velocidadHorizontal;
    vx = vx + (objetivo - vx) * suavizado;
    x += vx * dt;
}
void Avion::moverIzquierda(float dt) {
    if (!estaVivo()) return;
    float objetivo = -velocidadHorizontal;
    vx = vx + (objetivo - vx) * suavizado;
    x += vx * dt;
}
void Avion::tomarDanio(int d) {
    if (!estaVivo()) return;
    recibirDanio(d);
    if (vida == 0) {
        vx = 0;
        vy = 0;
    }
}
void Avion::actualizar(float dt) {
    if (!estaVivo())
        return;
    vx *= 0.95f;
    x += vx * dt;
    if (x < 0) x = 0;
    if (x > 1100) x = 1100;
}
