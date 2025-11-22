#include "nivel_1.h"
#include <algorithm> // std::remove_if
#include <cstdlib>   // std::rand

Nivel1::Nivel1(float ancho, float alto)
    : anchoScene(ancho),
    altoScene(alto),
    ySuelo(alto - 150.0f),            // suelo un poco por encima del borde inferior
    jugador(100.0f, alto - 150.0f),   // soldado cerca del borde izquierdo
    obstaculos(),
    tiempoRestante(99999990.0f),            // 30 segundos de nivel
    tiempoTotal(99999990.0f),
    gano(false),
    perdio(false),
    flagMoverIzq(false),
    flagMoverDer(false),
    flagAgachar(false),
    intervaloSpawn(1.5f),             // nuevo obstáculo cada 1.5 s
    tiempoDesdeUltSpawn(0.0f)
{
    jugador.setSuelo(ySuelo);
}

void Nivel1::reiniciar() {
    // Volver a poner al soldado en la posición inicial
    jugador = Soldado(100.0f, ySuelo);
    jugador.setSuelo(ySuelo);

    obstaculos.clear();

    tiempoTotal      = 99999990.0f;
    tiempoRestante   = tiempoTotal;
    gano             = false;
    perdio           = false;

    flagMoverIzq     = false;
    flagMoverDer     = false;
    flagAgachar      = false;

    intervaloSpawn       = 1.5f;
    tiempoDesdeUltSpawn  = 0.0f;
}

// -------- hooks de entrada que Nivel1 sí usa --------

void Nivel1::moverIzquierdaPresionado(bool presionado) {
    flagMoverIzq = presionado;
}

void Nivel1::moverDerechaPresionado(bool presionado) {
    flagMoverDer = presionado;
}

void Nivel1::saltarPresionado(bool presionado) {
    // salto tipo “tap”: sólo cuando se pulsa Space
    if (presionado) {
        jugador.saltar();
    }
}

void Nivel1::agacharPresionado(bool presionado) {
    flagAgachar = presionado;
}

// -------- lógica principal --------

void Nivel1::actualizar(float dt) {
    if (haTerminado())
        return;

    // 1) Cronómetro de nivel
    tiempoRestante -= dt;
    if (tiempoRestante < 0.0f)
        tiempoRestante = 0.0f;

    // Si se acabó el tiempo, decidir victoria/derrota
    if (tiempoRestante <= 0.0f) {
        if (jugador.estaVivo())
            gano = true;
        else
            perdio = true;
        return;
    }

    // 2) Actualizar jugador y obstáculos
    actualizarJugador(dt);
    actualizarObstaculos(dt);
    verificarColisiones();

    // 3) ¿el jugador ha muerto?
    if (!jugador.estaVivo())
        perdio = true;
}

bool Nivel1::haTerminado() const {
    return gano || perdio;
}

bool Nivel1::haGanado() const {
    return gano;
}

// -------- auxiliares internos --------

void Nivel1::actualizarJugador(float dt) {
    // movimiento horizontal
    if (flagMoverIzq)
        jugador.moverIzquierda(dt);
    if (flagMoverDer)
        jugador.moverDerecha(dt);

    // agacharse
    jugador.agachar(flagAgachar);

    // física (gravedad + salto)
    jugador.actualizar(dt);

    // Limitar X dentro de [0, anchoScene]
    float x = jugador.getX();
    float anchoJugador = jugador.getAncho();

    if (x < 0.0f) {
        jugador.mover(-x, 0.0f);
    } else if (x + anchoJugador > anchoScene) {
        float exceso = (x + anchoJugador) - anchoScene;
        jugador.mover(-exceso, 0.0f);
    }
}

void Nivel1::actualizarObstaculos(float dt) {
    // Spawn de nuevos obstáculos
    tiempoDesdeUltSpawn += dt;
    if (tiempoDesdeUltSpawn >= intervaloSpawn) {
        generarObstaculo();
        tiempoDesdeUltSpawn = 0.0f;
    }

    // Actualizar todos
    for (auto &o : obstaculos) {
        o.actualizar(dt);
    }

    // Eliminar los que ya no estén activos
    obstaculos.erase(
        std::remove_if(obstaculos.begin(), obstaculos.end(),
                       [](const Obstaculo& o){ return !o.estaActivo(); }),
        obstaculos.end()
        );
}

void Nivel1::generarObstaculo() {
    // x siempre en el borde derecho de la escena
    float x = anchoScene;

    int tipo = std::rand() % 3;  // 0, 1 o 2

    float w, h, vX;
    int   danio;
    float y;

    switch (tipo) {
    case 0:
        // Obstáculo de suelo (para saltar)
        w    = 32.0f;
        h    = 32.0f;
        vX   = -220.0f;
        danio= 20;
        y    = ySuelo - h;
        break;

    case 1:
        // Obstáculo alto (para agacharse)
        w    = 32.0f;
        h    = 32.0f;
        vX   = -260.0f;
        danio= 15;
        {
            // Queremos que de pie choque y agachado no.
            // Altura de pie del soldado: 48 px (en tu constructor)
            float altoJugador = 48.0f;
            // delta entre 0.5*alto y 1.0*alto funciona bien; cogemos ~0.75
            float delta = altoJugador * 0.75f; // ~36 px

            // Esto coloca el obstáculo por encima de la cabeza agachada,
            // pero dentro de la altura de la cabeza de pie.
            y = ySuelo - h - delta;
        }
        break;

    default:
        // Obstáculo grande de suelo (más lento)
        w    = 48.0f;
        h    = 48.0f;
        vX   = -150.0f;
        danio= 30;
        y    = ySuelo - h;
        break;
    }

    Obstaculo o(x, y, w, h, vX, 0.0f, danio);
    o.setLimitesEscena(0.0f, anchoScene, 0.0f, altoScene);
    obstaculos.push_back(o);
}

void Nivel1::verificarColisiones() {
    for (auto &o : obstaculos) {
        if (!o.estaActivo())
            continue;

        if (o.colisionaCon(jugador)) {
            jugador.tomarDanio(o.getDanio());
            o.desactivar();
        }
    }
}
