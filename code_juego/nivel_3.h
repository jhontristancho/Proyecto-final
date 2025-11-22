#ifndef NIVEL3_H
#define NIVEL3_H

#include "nivel.h"
#include "avion.h"
#include "obstaculo.h"
#include "proyectil.h"
#include <vector>

// Nivel 3:
// - El jugador controla un avión que se mueve SOLO en X (A/D).
// - Dispara con I.
// - Aparecen obstáculos/enemigos desde la derecha.
// - Gana si alcanza cierta puntuación destruyendo obstáculos antes de que se acabe el tiempo.
// - Pierde si se acaba el tiempo sin llegar a la puntuación o si el avión muere.
class Nivel3 : public Nivel {
public:
    Nivel3(float anchoScene, float altoScene);

    void actualizar(float dt) override;
    void reiniciar() override;

    bool haTerminado() const override;
    bool haGanado() const override;

    Personaje*       getJugador()       override { return &jugador; }
    const Personaje* getJugador() const override { return &jugador; }

    // --- Hooks de entrada que SÍ usa este nivel ---
    void moverIzquierdaPresionado(bool presionado) override;
    void moverDerechaPresionado(bool presionado) override;
    void dispararPresionado(bool presionado) override;

    // W / S / Space / C no se usan en este nivel, así que no se sobreescriben.

    // --- Consultas útiles para la parte gráfica/HUD ---
    const std::vector<Obstaculo>& getObstaculos() const { return obstaculos; }
    const std::vector<Proyectil>& getBalasJugador() const { return jugador.getProyectiles(); }

    int   getPuntuacion() const { return puntuacion; }
    int   getPuntuacionObjetivo() const { return puntuacionObjetivo; }
    float getTiempoRestante() const { return tiempoRestante; }

private:
    // Tamaño de la escena (copiado del controlador)
    float anchoScene;
    float altoScene;

    // Entidades
    Avion jugador;
    std::vector<Obstaculo> obstaculos;

    // Movimiento permitido del avión en X
    float xMinJugador;
    float xMaxJugador;

    // Estado del nivel (timer + puntuación)
    float tiempoTotal;       // duración total del nivel en segundos
    float tiempoRestante;
    bool  gano;
    bool  perdio;

    int   puntuacion;
    int   puntuacionObjetivo;
    int   puntosPorObstaculo;

    int   obstaculosDestruidos;
    int   obstaculosObjetivo;     // cuántos obstáculos totales se planea generar
    int   obstaculosGenerados;

    // Spawn de obstáculos
    float intervaloSpawn;
    float tiempoDesdeUltSpawn;

    // Flags de entrada
    bool flagMoverIzq;
    bool flagMoverDer;
    bool flagDisparar;

    // --- Lógica interna ---
    void actualizarJugador(float dt);
    void actualizarObstaculos(float dt);
    void manejarColisiones();

    void generarObstaculo();
    bool colisionProyectilObstaculo(const Proyectil& p, const Obstaculo& o) const;
};

#endif // NIVEL3_H
