#include "tanque.h"
#include <algorithm>

Tanque::Tanque(float px, float py)
    : Personaje(px, py, 64.0f, 48.0f, 200),
    velocidadMovimiento(180.0f),
    velocidadProyectil(300.0f),
    cooldown(0.8f),
    tiempoDesdeUltDisparo(0.0f),
    proyectiles(),
    limiteSuperiorY(0.0f),    // luego el nivel ajusta esto
    limiteInferiorY(600.0f)   // idem
{
}

void Tanque::moverArriba(float dt) {
    if (!estaVivo()) return;

    y -= velocidadMovimiento * dt;
    if (y < limiteSuperiorY) {
        y = limiteSuperiorY;
    }
}

void Tanque::moverAbajo(float dt) {
    if (!estaVivo()) return;

    y += velocidadMovimiento * dt;
    if (y + alto > limiteInferiorY) {
        y = limiteInferiorY - alto;
    }
}

void Tanque::disparar() {
    if (!estaVivo()) return;

    // cooldown
    if (tiempoDesdeUltDisparo < cooldown)
        return;

    float px = x + ancho;        // boca del tanque hacia la derecha
    float py = y + alto * 0.5f;  // centro vertical

    Proyectil p(px, py, velocidadProyectil, 0.0f, 25, true);
    // Si quieres, desde el nivel puedes llamar p.setLimitesEscena(...), usando anchoScene/altoScene
    proyectiles.push_back(p);

    tiempoDesdeUltDisparo = 0.0f;
}

void Tanque::tomarDanio(int d) {
    recibirDanio(d);
}

void Tanque::actualizar(float dt) {
    tiempoDesdeUltDisparo += dt;

    // Actualizar proyectiles
    for (auto& p : proyectiles) {
        p.actualizar(dt);
    }

    // Eliminar proyectiles inactivos
    proyectiles.erase(
        std::remove_if(proyectiles.begin(), proyectiles.end(),
                       [](const Proyectil& p){ return !p.estaActivo(); }),
        proyectiles.end()
        );
}

const std::vector<Proyectil>& Tanque::getProyectiles() const {
    return proyectiles;
}

std::vector<Proyectil>& Tanque::getProyectilesMutable() {
    return proyectiles;
}
