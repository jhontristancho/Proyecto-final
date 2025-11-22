#include "nivel_2.h"
#include <algorithm>

Nivel2::Nivel2(float ancho, float alto)
    : anchoScene(ancho),
    altoScene(alto),
    // Tanque del jugador, cerca del borde izquierdo
    jugador(150.0f, alto / 2.0f),
    // Jefe final, enemigo en el lado derecho (jefe = true)
    jefe(ancho - 200.0f, alto / 2.0f, true),
    gano(false),
    perdio(false),
    flagMoverArriba(false),
    flagMoverAbajo(false),
    flagDisparar(false)
{
    // Configurar límites verticales para tanque y jefe
    float margen = 80.0f;
    float yMin = margen;
    float yMax = altoScene - margen;
    jugador.setLimitesVerticales(yMin, yMax);
    jefe.setLimitesVerticales(yMin, yMax);
}

void Nivel2::reiniciar() {
    jugador = Tanque(150.0f, altoScene / 2.0f);
    jefe    = Enemigo(anchoScene - 200.0f, altoScene / 2.0f, true);

    float margen = 80.0f;
    float yMin = margen;
    float yMax = altoScene - margen;
    jugador.setLimitesVerticales(yMin, yMax);
    jefe.setLimitesVerticales(yMin, yMax);

    gano  = false;
    perdio= false;

    flagMoverArriba = false;
    flagMoverAbajo  = false;
    flagDisparar    = false;
}

bool Nivel2::haTerminado() const {
    return gano || perdio;
}

bool Nivel2::haGanado() const {
    return gano;
}

// ---- Hooks de entrada ----
void Nivel2::moverArribaPresionado(bool presionado) {
    flagMoverArriba = presionado;
}

void Nivel2::moverAbajoPresionado(bool presionado) {
    flagMoverAbajo = presionado;
}

void Nivel2::dispararPresionado(bool presionado) {
    flagDisparar = presionado;
}

// ---- Lógica principal ----
void Nivel2::actualizar(float dt) {
    if (haTerminado())
        return;

    actualizarJugador(dt);
    actualizarEnemigo(dt);
    manejarColisiones();

    if (!jugador.estaVivo())
        perdio = true;
    else if (!jefe.estaVivo())
        gano = true;
}

void Nivel2::actualizarJugador(float dt) {
    if (!jugador.estaVivo())
        return;

    if (flagMoverArriba)
        jugador.moverArriba(dt);
    if (flagMoverAbajo)
        jugador.moverAbajo(dt);

    if (flagDisparar)
        jugador.disparar();

    jugador.actualizar(dt);
}

void Nivel2::actualizarEnemigo(float dt) {
    // PERCEPCIÓN: el jefe ve las balas del jugador
    jefe.percibirProyectiles(jugador.getProyectiles());
    jefe.actualizar(dt);
}

void Nivel2::manejarColisiones() {
    // 1) Balas del jugador contra el jefe
    auto& balasJugador = jugador.getProyectilesMutable();
    for (auto& bala : balasJugador) {
        if (!bala.estaActivo()) continue;
        if (!bala.esAliado())   continue; // balas del jugador

        if (bala.colisionaCon(jefe)) {
            jefe.tomarDanio(bala.getDanio());
            bala.desactivar();
        }
    }

    // 2) Balas del jefe contra el jugador
    auto& balasJefe = jefe.getProyectilesMutable();
    for (auto& bala : balasJefe) {
        if (!bala.estaActivo()) continue;
        if (bala.esAliado())    continue;  // balas enemigas

        if (bala.colisionaCon(jugador)) {
            jugador.tomarDanio(bala.getDanio());
            bala.desactivar();
        }
    }

    // 3) Colisión directa tanque ↔ jefe
    if (colisionRect(jugador, jefe)) {
        jugador.tomarDanio(40);
        jefe.tomarDanio(40);
    }
}

bool Nivel2::colisionRect(const Personaje& a, const Personaje& b) const {
    float ax1 = a.getX();
    float ay1 = a.getY();
    float ax2 = ax1 + a.getAncho();
    float ay2 = ay1 + a.getAlto();

    float bx1 = b.getX();
    float by1 = b.getY();
    float bx2 = bx1 + b.getAncho();
    float by2 = by1 + b.getAlto();

    bool noColision =
        (ax2 < bx1) ||
        (ax1 > bx2) ||
        (ay2 < by1) ||
        (ay1 > by2);

    return !noColision;
}
