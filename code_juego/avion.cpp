#include "avion.h"
#include <algorithm>

Avion::Avion(float px, float py)
    : Personaje(px, py, 64.0f, 32.0f, 150),
    velocidadHorizontal(260.0f),
    velocidadProyectil(400.0f),
    cooldown(0.4f),
    tiempoDesdeUltDisparo(0.0f),
    proyectiles(),
    limiteIzquierdoX(0.0f),
    limiteDerechoX(1200.0f)  // valor por defecto, el nivel lo puede ajustar
{
}

void Avion::moverDerecha(float dt) {
    if (!estaVivo()) return;

    x += velocidadHorizontal * dt;
    if (x + ancho > limiteDerechoX) {
        x = limiteDerechoX - ancho;
    }
}

void Avion::moverIzquierda(float dt) {
    if (!estaVivo()) return;

    x -= velocidadHorizontal * dt;
    if (x < limiteIzquierdoX) {
        x = limiteIzquierdoX;
    }
}

void Avion::disparar() {
    if (!estaVivo()) return;

    // respetar cooldown
    if (tiempoDesdeUltDisparo < cooldown)
        return;

    float px = x + ancho;        // punta del avión
    float py = y + alto * 0.5f;  // centro vertical

    Proyectil p(px, py, velocidadProyectil, 0.0f, 20, true);
    // Si quieres, en Nivel3 puedes llamar p.setLimitesEscena(...),
    // pero con los límites por defecto también funciona.
    proyectiles.push_back(p);

    tiempoDesdeUltDisparo = 0.0f;
}

void Avion::tomarDanio(int d) {
    recibirDanio(d);
}

void Avion::actualizar(float dt) {
    // aunque el avión esté muerto, podemos seguir actualizando balas
    tiempoDesdeUltDisparo += dt;

    for (auto& p : proyectiles) {
        p.actualizar(dt);
    }

    // eliminar balas inactivas
    proyectiles.erase(
        std::remove_if(proyectiles.begin(), proyectiles.end(),
                       [](const Proyectil& p) { return !p.estaActivo(); }),
        proyectiles.end()
        );
}

const std::vector<Proyectil>& Avion::getProyectiles() const {
    return proyectiles;
}

std::vector<Proyectil>& Avion::getProyectilesMutable() {
    return proyectiles;
}
