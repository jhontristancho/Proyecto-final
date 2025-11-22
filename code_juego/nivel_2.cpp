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
    // Límites verticales para el tanque (que no se salga de la escena)
    float margenSuperior = 80.0f;
    float margenInferior = altoScene - 80.0f;
    jugador.setLimitesVerticales(margenSuperior, margenInferior);

    // Límites para el jefe: zona de la derecha
    float xMinJefe = anchoScene * 0.6f;
    float xMaxJefe = anchoScene - 80.0f;
    float yMinJefe = margenSuperior;
    float yMaxJefe = margenInferior;
    jefe.setLimitesEscena(xMinJefe, xMaxJefe, yMinJefe, yMaxJefe);

    // Si quisieras tunearlo distinto de los valores por defecto:
    // jefe.setVelocidadBase(0.0f, 0.0f);           // se moverá solo para esquivar
    // jefe.setParametrosDisparo(280.0f, 0.7f);     // velocidad bala, cooldown
}

void Nivel2::reiniciar() {
    jugador = Tanque(150.0f, altoScene / 2.0f);
    jugador.setLimitesVerticales(80.0f, altoScene - 80.0f);

    jefe = Enemigo(anchoScene - 200.0f, altoScene / 2.0f, true);
    jefe.setLimitesEscena(anchoScene * 0.6f, anchoScene - 80.0f,
                          80.0f, altoScene - 80.0f);

    gano   = false;
    perdio = false;

    flagMoverArriba = false;
    flagMoverAbajo  = false;
    flagDisparar    = false;
}

// ------- hooks de entrada -------

void Nivel2::moverArribaPresionado(bool presionado) {
    flagMoverArriba = presionado;
}

void Nivel2::moverAbajoPresionado(bool presionado) {
    flagMoverAbajo = presionado;
}

void Nivel2::dispararPresionado(bool presionado) {
    flagDisparar = presionado;
}

// ------- lógica principal del nivel -------

void Nivel2::actualizar(float dt) {
    if (haTerminado())
        return;

    actualizarJugador(dt);

    // PERCEPCIÓN: el jefe "ve" las balas del jugador
    jefe.percibirProyectiles(jugador.getProyectiles());

    actualizarEnemigo(dt);

    manejarColisiones();

    if (!jugador.estaVivo())
        perdio = true;
    else if (!jefe.estaVivo())
        gano = true;
}

bool Nivel2::haTerminado() const {
    return gano || perdio;
}

bool Nivel2::haGanado() const {
    return gano;
}

// ------- auxiliares internos -------

void Nivel2::actualizarJugador(float dt) {
    if (!jugador.estaVivo())
        return;

    if (flagMoverArriba)
        jugador.moverArriba(dt);
    if (flagMoverAbajo)
        jugador.moverAbajo(dt);

    if (flagDisparar)
        jugador.disparar();   // Tanque respeta su propio cooldown

    jugador.actualizar(dt);   // actualiza balas y limpia inactivas
}

void Nivel2::actualizarEnemigo(float dt) {
    jefe.actualizar(dt);      // razonar + actuar + disparar + balas (según tu Enemigo)
}

void Nivel2::manejarColisiones() {
    // 1) Balas del jugador contra el jefe
    const auto& balasJugadorConst = jugador.getProyectiles();
    for (const auto& balaConst : balasJugadorConst) {
        auto& bala = const_cast<Proyectil&>(balaConst); // solo para poder desactivarla

        if (!bala.estaActivo()) continue;
        if (!bala.esAliado())   continue;  // por claridad

        if (bala.colisionaCon(jefe)) {
            jefe.tomarDanio(bala.getDanio());
            bala.desactivar();
        }
    }

    // 2) Balas del jefe contra el jugador
    auto& balasJefe = jefe.getProyectilesMutable();
    for (auto& bala : balasJefe) {
        if (!bala.estaActivo()) continue;
        if (bala.esAliado())    continue;  // estas son balas del enemigo

        if (bala.colisionaCon(jugador)) {
            jugador.tomarDanio(bala.getDanio());
            bala.desactivar();
        }
    }
}
