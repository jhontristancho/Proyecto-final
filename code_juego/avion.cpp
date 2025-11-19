#include "avion.h"
#include <algorithm>

Avion::Avion(float px, float py)
    : Personaje(px, py, 48.0f, 32.0f, 100),
    velocidadHorizontal(250.0f),
    suavizado(0.1f),
    velocidadProyectil(400.0f),
    cooldown(0.4f),
    tiempoDesdeUltDisparo(0.0f),
    limiteIzquierdoX(0.0f),    // por defecto como lo tenías
    limiteDerechoX(1100.0f)    // luego el controlador puede ajustarlo usando anchoScene
{
}

void Avion::moverDerecha(float dt) {
    if (!estaVivo()) return;

    float objetivo = velocidadHorizontal;         // queremos ir hacia la derecha
    vx += (objetivo - vx) * suavizado;           // interpola para que se vea suave
}

void Avion::moverIzquierda(float dt) {
    if (!estaVivo()) return;

    float objetivo = -velocidadHorizontal;       // queremos ir hacia la izquierda
    vx += (objetivo - vx) * suavizado;
}

void Avion::disparar() {
    if (!estaVivo()) return;
    if (tiempoDesdeUltDisparo < cooldown)
        return;

    float px = x + ancho;
    float py = y + alto / 2.0f;

    proyectiles.emplace_back(px, py, velocidadProyectil, 0.0f, 15, true);
    tiempoDesdeUltDisparo = 0.0f;
}

void Avion::tomarDanio(int d) {
    recibirDanio(d);
    if (!estaVivo()) {
        // aquí podrías agregar lógica extra cuando el avión muere
    }
}

void Avion::actualizar(float dt) {
    if (!estaVivo()) return;

    tiempoDesdeUltDisparo += dt;

    // Inercia / fricción simple
    vx *= 0.95f;
    x  += vx * dt;

    // Limites horizontales configurables
    if (x < limiteIzquierdoX) x = limiteIzquierdoX;
    if (x > limiteDerechoX)   x = limiteDerechoX;

    // Actualizar proyectiles
    for (auto& p : proyectiles)
        p.actualizar(dt);

    // Limpiar proyectiles inactivos
    proyectiles.erase(
        std::remove_if(proyectiles.begin(), proyectiles.end(),
                       [](const Proyectil& p){ return !p.estaActivo(); }),
        proyectiles.end());
}

const std::vector<Proyectil>& Avion::getProyectiles() const {
    return proyectiles;
}
