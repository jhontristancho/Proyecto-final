#include "nivel_3.h"
#include <algorithm> // std::remove_if
#include <cstdlib>   // std::rand

Nivel3::Nivel3(float ancho, float alto)
    : anchoScene(ancho),
    altoScene(alto),
    // El avión empieza hacia la izquierda y centrado en Y
    jugador(ancho * 0.25f, alto * 0.5f),
    obstaculos(),
    xMinJugador(50.0f),
    xMaxJugador(ancho * 0.8f), // que no llegue hasta el borde derecho
    tiempoTotal(40.0f),        // 40 segundos de nivel (ajusta a gusto)
    tiempoRestante(tiempoTotal),
    gano(false),
    perdio(false),
    puntuacion(0),
    puntuacionObjetivo(200),   // por ejemplo: 200 puntos para ganar
    puntosPorObstaculo(10),    // 10 puntos por obstáculo destruido
    obstaculosDestruidos(0),
    obstaculosObjetivo(20),    // por ejemplo: el nivel genera 20 obstáculos
    obstaculosGenerados(0),
    intervaloSpawn(1.2f),      // cada 1.2 segundos aparece un obstáculo
    tiempoDesdeUltSpawn(0.0f),
    flagMoverIzq(false),
    flagMoverDer(false),
    flagDisparar(false)
{
    // Limitamos el movimiento horizontal del avión
    jugador.setLimitesX(xMinJugador, xMaxJugador);
}

void Nivel3::reiniciar() {
    jugador = Avion(anchoScene * 0.25f, altoScene * 0.5f);
    jugador.setLimitesX(xMinJugador, xMaxJugador);

    obstaculos.clear();

    tiempoRestante       = tiempoTotal;
    gano                 = false;
    perdio               = false;
    puntuacion           = 0;
    obstaculosDestruidos = 0;
    obstaculosGenerados  = 0;

    intervaloSpawn       = 1.2f;
    tiempoDesdeUltSpawn  = 0.0f;

    flagMoverIzq   = false;
    flagMoverDer   = false;
    flagDisparar   = false;
}

// -------- hooks de entrada (A, D, I) --------

void Nivel3::moverIzquierdaPresionado(bool presionado) {
    flagMoverIzq = presionado;
}

void Nivel3::moverDerechaPresionado(bool presionado) {
    flagMoverDer = presionado;
}

void Nivel3::dispararPresionado(bool presionado) {
    flagDisparar = presionado;
}

// -------- lógica principal --------

void Nivel3::actualizar(float dt) {
    if (haTerminado())
        return;

    // 1) Actualizar el timer del nivel
    tiempoRestante -= dt;
    if (tiempoRestante < 0.0f)
        tiempoRestante = 0.0f;

    // 2) Actualizar entidades
    actualizarJugador(dt);
    actualizarObstaculos(dt);
    manejarColisiones();

    // 3) Revisar condiciones de victoria / derrota
    if (!jugador.estaVivo()) {
        perdio = true;
        return;
    }

    if (puntuacion >= puntuacionObjetivo &&
        obstaculosDestruidos >= obstaculosObjetivo) {
        // Alcanzó la puntuación y destruyó todos los objetivos
        gano = true;
        return;
    }

    // Si se acabó el tiempo y no alcanzó la puntuación / no destruyó los objetivos
    if (tiempoRestante <= 0.0f &&
        (puntuacion < puntuacionObjetivo || obstaculosDestruidos < obstaculosObjetivo)) {
        perdio = true;
    }
}

bool Nivel3::haTerminado() const {
    return gano || perdio;
}

bool Nivel3::haGanado() const {
    return gano;
}

// -------- auxiliares internos --------

void Nivel3::actualizarJugador(float dt) {
    if (!jugador.estaVivo())
        return;

    // Movimiento horizontal con A / D usando Avion::mover...
    if (flagMoverIzq)
        jugador.moverIzquierda(dt);
    if (flagMoverDer)
        jugador.moverDerecha(dt);

    // Disparar con I (respetando cooldown interno del Avion)
    if (flagDisparar)
        jugador.disparar();

    // Actualiza su lógica (proyectiles, etc.)
    jugador.actualizar(dt);
}

void Nivel3::actualizarObstaculos(float dt) {
    // Spawn de nuevos obstáculos mientras no se haya alcanzado el máximo planificado
    if (obstaculosGenerados < obstaculosObjetivo) {
        tiempoDesdeUltSpawn += dt;
        if (tiempoDesdeUltSpawn >= intervaloSpawn) {
            generarObstaculo();
            tiempoDesdeUltSpawn = 0.0f;
        }
    }

    // Actualizar todos los obstáculos existentes
    for (auto& o : obstaculos) {
        o.actualizar(dt);
    }

    // Eliminar los que ya no estén activos
    obstaculos.erase(
        std::remove_if(obstaculos.begin(), obstaculos.end(),
                       [](const Obstaculo& o){ return !o.estaActivo(); }),
        obstaculos.end()
        );
}

void Nivel3::generarObstaculo() {
    // x en el borde derecho de la escena
    float x = anchoScene + 20.0f;

    // Distintos tipos de obstáculos (para sprites diferentes)
    int tipo = std::rand() % 3;  // 0, 1 o 2

    float w, h, vX;
    int   danio;

    switch (tipo) {
    case 0:
        // Obstáculo pequeño y rápido
        w    = 32.0f;
        h    = 32.0f;
        vX   = -300.0f;
        danio= 20;
        break;
    case 1:
        // Obstáculo mediano
        w    = 48.0f;
        h    = 32.0f;
        vX   = -230.0f;
        danio= 25;
        break;
    default:
        // Obstáculo grande y lento
        w    = 64.0f;
        h    = 40.0f;
        vX   = -180.0f;
        danio= 30;
        break;
    }

    // Altura aleatoria dentro de un rango central
    float margenY = 100.0f;
    float yMin = margenY;
    float yMax = altoScene - margenY - h;
    float y = yMin;
    if (yMax > yMin) {
        float rango = yMax - yMin;
        y += static_cast<float>(std::rand() % static_cast<int>(rango));
    }

    Obstaculo o(x, y, w, h, vX, 0.0f, danio);
    o.setLimitesEscena(-100.0f, anchoScene + 100.0f, 0.0f, altoScene);
    obstaculos.push_back(o);

    obstaculosGenerados++;
}

void Nivel3::manejarColisiones() {
    // 1) Balas del jugador contra obstáculos
    const auto& balasJugadorConst = jugador.getProyectiles();

    for (const auto& balaConst : balasJugadorConst) {
        auto& bala = const_cast<Proyectil&>(balaConst); // para poder desactivarlo

        if (!bala.estaActivo()) continue;
        if (!bala.esAliado())   continue; // por claridad: balas del jugador

        for (auto& o : obstaculos) {
            if (!o.estaActivo()) continue;

            if (colisionProyectilObstaculo(bala, o)) {
                o.desactivar();
                bala.desactivar();
                obstaculosDestruidos++;
                puntuacion += puntosPorObstaculo;
                break; // esta bala ya no sigue chocando con más obstáculos
            }
        }
    }

    // 2) Obstáculos contra el avión (colisión directa)
    for (auto& o : obstaculos) {
        if (!o.estaActivo()) continue;

        if (o.colisionaCon(jugador)) {
            jugador.tomarDanio(o.getDanio());
            o.desactivar();
        }
    }
}

bool Nivel3::colisionProyectilObstaculo(const Proyectil& p, const Obstaculo& o) const {
    // Consideramos el proyectil como un punto en (x, y)
    float px = p.getX();
    float py = p.getY();

    float ox1 = o.getX();
    float oy1 = o.getY();
    float ox2 = ox1 + o.getAncho();
    float oy2 = oy1 + o.getAlto();

    bool dentroX = (px >= ox1) && (px <= ox2);
    bool dentroY = (py >= oy1) && (py <= oy2);

    return dentroX && dentroY;
}
