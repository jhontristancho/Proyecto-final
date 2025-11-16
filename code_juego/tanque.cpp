#include "Tanque.h"

Tanque::Tanque(float px, float py)
    : Personaje(px, py, 64.0f, 48.0f, 200),
    velocidadMovimiento(180.0f),
    velocidadProyectil(300.0f),
    cooldown(0.8f),
    tiempoDesdeUltDisparo(0.0f)
{
}
void Tanque::moverArriba(float dt) {
    if (!estaVivo()) return;
    y -= velocidadMovimiento * dt;
}
void Tanque::moverAbajo(float dt) {
    if (!estaVivo()) return;
    y += velocidadMovimiento * dt;
}
void Tanque::disparar() {
    if (!estaVivo()) return;
    if (tiempoDesdeUltDisparo < cooldown)//esto e para que espere un tiempo para que no dispare como loco
        return;
    ProyectilData p;
    p.x = x + ancho;
    p.y = y + alto / 2;
    p.velocidad = velocidadProyectil;
    p.activo = true;

    proyectiles.push_back(p);
    tiempoDesdeUltDisparo = 0.0f;
}

void Tanque::tomarDanio(int d) {
    if (!estaVivo()) return;
    recibirDanio(d);
    if (vida == 0) {
        vx = 0;
        vy = 0;
        for (auto& p : proyectiles)
            p.activo = false;
    }
}
void Tanque::actualizar(float dt) {
    if (!estaVivo())
        return;
    tiempoDesdeUltDisparo += dt;
    for (auto& p : proyectiles) {
        if (!p.activo)
            continue;
        p.x += p.velocidad * dt;
        if (p.x > 1200) {
            p.activo = false;
        }
    }
}
