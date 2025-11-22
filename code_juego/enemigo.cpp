#include "enemigo.h"
#include <algorithm>
#include <cmath>

Enemigo::Enemigo(float px, float py, bool jefe)
    : Personaje(
          px,
          py,
          jefe ? 80.0f : 40.0f,   // ancho
          jefe ? 60.0f : 32.0f,   // alto
          jefe ? 300     : 60     // vida inicial
          ),
    velX(jefe ? 0.0f : -120.0f),  // jefe casi estático en X
    velY(0.0f),
    velocidadProyectil(jefe ? 280.0f : 180.0f),
    cooldown(jefe ? 0.7f : 1.2f),
    tiempoDesdeUltDisparo(0.0f),
    proyectiles(),
    esJefe(jefe),
    fase(1),
    golpesRecibidos(0),
    limiteSuperiorY(0.0f),
    limiteInferiorY(600.0f),
    velocidadEsquiva(jefe ? 160.0f : 120.0f),
    quiereMoverArriba(false),
    quiereMoverAbajo(false)
{
}

void Enemigo::percibirProyectiles(const std::vector<Proyectil>& balasJugador) {
    // Reset de intención
    quiereMoverArriba = false;
    quiereMoverAbajo  = false;

    if (!estaVivo())
        return;

    float mejorDistX = 999999.0f;
    float margenAmenazaY = alto * 1.5f;

    for (const auto& b : balasJugador) {
        if (!b.estaActivo()) continue;
        if (!b.esAliado())   continue;

        float dx = x - b.getX();
        if (dx <= 0.0f)  continue;    // bala ya pasó o está detrás
        if (dx > 400.0f) continue;    // muy lejos, sin amenaza

        float dyCentro = (y + alto * 0.5f) - b.getY();
        if (std::fabs(dyCentro) > margenAmenazaY)
            continue;

        if (dx < mejorDistX) {
            mejorDistX = dx;
            // Si la bala viene "por arriba", nos movemos hacia abajo, y viceversa
            if (b.getY() < y + alto * 0.5f) {
                quiereMoverAbajo = true;
                quiereMoverArriba = false;
            } else {
                quiereMoverArriba = true;
                quiereMoverAbajo  = false;
            }
        }
    }
}

void Enemigo::tomarDanio(int d) {
    if (!estaVivo())
        return;

    recibirDanio(d);
    golpesRecibidos++;
    aprender();
}

void Enemigo::aprender() {
    if (!esJefe)
        return;

    int nuevaFase = 1 + golpesRecibidos / 3;
    if (nuevaFase > fase) {
        fase = nuevaFase;
        // Aumenta capacidad de esquivar y dispara un poco más rápido
        velocidadEsquiva += 20.0f;
        cooldown *= 0.9f;
        actualizarFase();
    }
}

void Enemigo::actualizarFase() {
    // Aquí podrías cambiar comportamiento según fase (más agresivo, etc.)
    // De momento lo dejamos como placeholder.
}

void Enemigo::razonar() {
    // Por ahora la "percepción" ya cargó quiereMoverArriba/quiereMoverAbajo.
    // Aquí podrías combinar otras reglas, pero lo mantenemos simple.
}

void Enemigo::actuar(float dt) {
    if (!estaVivo())
        return;

    // Movimiento en X (sólo si no es jefe)
    x += velX * dt;

    // Movimiento vertical de esquiva
    if (quiereMoverArriba) {
        y -= velocidadEsquiva * dt;
    } else if (quiereMoverAbajo) {
        y += velocidadEsquiva * dt;
    }

    // Clamp vertical
    if (y < limiteSuperiorY)
        y = limiteSuperiorY;
    if (y + alto > limiteInferiorY)
        y = limiteInferiorY - alto;
}

void Enemigo::disparar() {
    if (!estaVivo())
        return;

    if (tiempoDesdeUltDisparo < cooldown)
        return;

    float px = x;               // dispara hacia la izquierda
    float py = y + alto * 0.5f;

    Proyectil p(px, py, -velocidadProyectil, 0.0f, esJefe ? 25 : 15, false);
    proyectiles.push_back(p);

    tiempoDesdeUltDisparo = 0.0f;
}

void Enemigo::limpiarProyectilesInactivos() {
    proyectiles.erase(
        std::remove_if(proyectiles.begin(), proyectiles.end(),
                       [](const Proyectil& p){ return !p.estaActivo(); }),
        proyectiles.end()
        );
}

void Enemigo::actualizar(float dt) {
    tiempoDesdeUltDisparo += dt;

    // b) RAZONAMIENTO (usa flags marcados en percibirProyectiles)
    razonar();

    // c) ACCIÓN: movimiento
    actuar(dt);

    // Disparo
    disparar();

    // Actualizar proyectiles
    for (auto& p : proyectiles) {
        p.actualizar(dt);
    }

    limpiarProyectilesInactivos();
}

const std::vector<Proyectil>& Enemigo::getProyectiles() const {
    return proyectiles;
}

std::vector<Proyectil>& Enemigo::getProyectilesMutable() {
    return proyectiles;
}
