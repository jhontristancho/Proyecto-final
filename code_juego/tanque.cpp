#include "tanque.h"
#include <algorithm>

Tanque::Tanque(float px, float py)
    : Personaje(px, py, 64.0f, 48.0f, 200),
    velocidadMovimiento(180.0f),
    velocidadProyectil(300.0f),
    cooldown(0.8f),
    tiempoDesdeUltDisparo(0.0f),
    limiteSuperiorY(0.0f),   // por defecto: como lo tenías antes
    limiteInferiorY(600.0f)  // luego el controlador puede ajustarlo
{
}

void Tanque::moverArriba(float dt) {
    if (!estaVivo()) return;

    y -= velocidadMovimiento * dt;
    if (y < limiteSuperiorY)
        y = limiteSuperiorY;
}

void Tanque::moverAbajo(float dt) {
    if (!estaVivo()) return;

    y += velocidadMovimiento * dt;
    if (y > limiteInferiorY)
        y = limiteInferiorY;
}

void Tanque::disparar() {
    if (!estaVivo()) return;

    // Dispara solo si se cumplió el cooldown
    if (tiempoDesdeUltDisparo < cooldown)
        return;

    float px = x + ancho;        // desde el “cañón” (lado derecho)
    float py = y + alto / 2.0f;  // centro vertical

    proyectiles.emplace_back(px, py, velocidadProyectil, 0.0f, 20, true);
    tiempoDesdeUltDisparo = 0.0f;
}

void Tanque::tomarDanio(int d) {
    recibirDanio(d);
    if (!estaVivo()) {
        // Aquí podrías agregar lógica extra (animación, etc.)
    }
}

void Tanque::actualizar(float dt) {
    if (!estaVivo()) return;

    tiempoDesdeUltDisparo += dt;

    // Actualiza los proyectiles
    for (auto& p : proyectiles)
        p.actualizar(dt);

    // Limpia proyectiles inactivos
    proyectiles.erase(
        std::remove_if(proyectiles.begin(), proyectiles.end(),
                       [](const Proyectil& p){ return !p.estaActivo(); }),
        proyectiles.end());
}

const std::vector<Proyectil>& Tanque::getProyectiles() const {
    return proyectiles;
}
