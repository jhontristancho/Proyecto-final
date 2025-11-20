#ifndef NIVEL1_H
#define NIVEL1_H

#include "nivel.h"
#include "soldado.h"
#include "obstaculo.h"
#include <vector>

class Nivel1 : public Nivel {
public:
    Nivel1(float anchoScene, float altoScene);

    // --- interfaz de Nivel ---
    void actualizar(float dt) override;
    void reiniciar() override;

    bool haTerminado() const override;
    bool haGanado() const override;

    Personaje*       getJugador()       override { return &jugador; }
    const Personaje* getJugador() const override { return &jugador; }

    // --- hooks de entrada que SÍ usa el nivel 1 ---
    void moverIzquierdaPresionado(bool presionado) override;
    void moverDerechaPresionado(bool presionado) override;
    void saltarPresionado(bool presionado) override;
    void agacharPresionado(bool presionado) override;

    // Estos no se usan en Nivel1, así que no los sobreescribimos:
    // moverArribaPresionado(...)
    // moverAbajoPresionado(...)
    // dispararPresionado(...)

    // Para la parte gráfica (dibujar obstáculos como sprites)
    const std::vector<Obstaculo>& getObstaculos() const { return obstaculos; }

private:
    // Tamaño lógico de la escena (viene de controlador_juego)
    float anchoScene;
    float altoScene;
    float ySuelo;

    // Entidades
    Soldado jugador;
    std::vector<Obstaculo> obstaculos;

    // Estado del nivel (nivel cronometrado)
    float tiempoRestante;  // segundos
    float tiempoTotal;
    bool  gano;
    bool  perdio;

    // Flags de entrada
    bool flagMoverIzq;
    bool flagMoverDer;
    bool flagAgachar;

    // Spawn de obstáculos
    float intervaloSpawn;
    float tiempoDesdeUltSpawn;

    // Métodos internos
    void actualizarJugador(float dt);
    void actualizarObstaculos(float dt);
    void generarObstaculo();
    void verificarColisiones();
};

#endif // NIVEL1_H
