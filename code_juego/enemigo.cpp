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
    amenazaDetectada(false),
    yAmenaza(0.0f),
    golpesRecibidos(0),
    velocidadEsquiva(80.0f),     // velocidad base de esquiva
    limiteIzquierdoX(0.0f),
    limiteDerechoX(0.0f),
    limiteSuperiorY(0.0f),
    limiteInferiorY(0.0f)
{
}

// PERCEPCIÓN:
// Revisa las balas del jugador que estén cerca y guarda la más "peligrosa"
// (la más cercana y con diferencia en Y pequeña).
void Enemigo::percibirProyectiles(const std::vector<Proyectil>& balasJugador) {
    amenazaDetectada = false;
    float mejorDistancia = 1e9f;

    float centroY = y + alto / 2.0f;

    for (const auto& b : balasJugador) {
        if (!b.esAliado()) continue;  // solo balas del jugador
        if (!b.estaActivo()) continue;

        float bx = b.getX();
        float by = b.getY();

        // Suponemos que el jugador está a la izquierda y dispara hacia la derecha:
        float dx = x - bx;       // si dx > 0, la bala viene hacia el enemigo
        if (dx < 0.0f) continue; // la bala ya pasó

        if (dx > 250.0f) continue; // fuera del rango horizontal de percepción

        float dy = std::fabs(by - centroY);
        if (dy > 80.0f) continue;  // muy lejos verticalmente

        if (dx < mejorDistancia) {
            mejorDistancia   = dx;
            amenazaDetectada = true;
            yAmenaza         = by;
        }
    }
}

// RAZONAMIENTO:
// Si hay amenaza, decide si subir o bajar para alejarse de la bala.
// Si no hay amenaza, no añade movimiento extra en Y.
void Enemigo::razonar() {
    if (!estaVivo()) {
        velY = 0.0f;
        return;
    }

    if (amenazaDetectada) {
        float centroY = y + alto / 2.0f;

        // Si la bala está por encima, me muevo hacia abajo.
        // Si la bala está por debajo, me muevo hacia arriba.
        if (yAmenaza < centroY) {
            velY = velocidadEsquiva;     // hacia abajo (y aumenta)
        } else {
            velY = -velocidadEsquiva;    // hacia arriba (y disminuye)
        }
    } else {
        // Sin amenaza: por ahora no hace movimiento vertical extra.
        velY = 0.0f;
    }
}

// ACCIÓN: aplicar movimiento según velX / velY y respetar límites
void Enemigo::actuar(float dt) {
    if (!estaVivo()) return;

    x += velX * dt;
    y += velY * dt;

    // Limitar dentro de la escena si los límites están definidos
    bool limitesDefinidos =
        (limiteIzquierdoX < limiteDerechoX) &&
        (limiteSuperiorY  < limiteInferiorY);

    if (limitesDefinidos) {
        if (x < limiteIzquierdoX) x = limiteIzquierdoX;
        if (x + ancho > limiteDerechoX) x = limiteDerechoX - ancho;
        if (y < limiteSuperiorY) y = limiteSuperiorY;
        if (y + alto > limiteInferiorY) y = limiteInferiorY - alto;
    }
}

// APRENDIZAJE: ajusta comportamiento según golpesRecibidos
void Enemigo::aprender() {
    // Cada golpe aumenta la velocidad de esquiva un poco.
    velocidadEsquiva = 80.0f + 20.0f * static_cast<float>(golpesRecibidos);
}

// Cambio de fase si es jefe (afecta patrón de disparo)
void Enemigo::actualizarFase() {
    if (!esJefe) return;

    // Fases según vida (ejemplo simple)
    if (vida <= 200 && fase < 2) {
        fase = 2;
        cooldown *= 0.8f;
        velocidadProyectil *= 1.1f;
    }
    if (vida <= 120 && fase < 3) {
        fase = 3;
        cooldown *= 0.8f;
        velocidadProyectil *= 1.1f;
    }
}

// ACCIÓN: disparo hacia la izquierda
void Enemigo::disparar() {
    if (!estaVivo()) return;
    if (tiempoDesdeUltDisparo < cooldown) return;

    float pxDisparo = x;
    float pyDisparo = y + alto / 2.0f;

    proyectiles.emplace_back(
        pxDisparo,
        pyDisparo,
        -velocidadProyectil,   // hacia la izquierda
        0.0f,
        esJefe ? 25 : 10,
        false                  // esDelJugador = false
        );

    tiempoDesdeUltDisparo = 0.0f;
}

// Percibe daño + actualiza aprendizaje + fase
void Enemigo::tomarDanio(int d) {
    if (!estaVivo()) return;

    recibirDanio(d);

    if (!estaVivo()) {
        // al morir puedes añadir efectos extra si quieres
        return;
    }

    // APRENDIZAJE: cada golpe incrementa el conteo
    golpesRecibidos++;
    aprender();         // actualiza velocidadEsquiva
    actualizarFase();   // para jefe
}

void Enemigo::limpiarProyectilesInactivos() {
    proyectiles.erase(
        std::remove_if(
            proyectiles.begin(),
            proyectiles.end(),
            [](const Proyectil& p){ return !p.estaActivo(); }
            ),
        proyectiles.end()
        );
}

// Bucle de actualización del enemigo: Razonamiento + Acción + Disparo + Balas
void Enemigo::actualizar(float dt) {
    // Actualizar temporizador de disparo
    tiempoDesdeUltDisparo += dt;

    if (!estaVivo()) {
        limpiarProyectilesInactivos();
        return;
    }

    // b) RAZONAMIENTO: decide movimiento según lo percibido
    razonar();

    // c) ACCIÓN: aplica movimiento y mantiene en límites
    actuar(dt);

    // Acción de ataque: disparar
    disparar();

    // Actualizar proyectiles
    for (auto& p : proyectiles) {
        p.actualizar(dt);
    }

    limpiarProyectilesInactivos();
}
