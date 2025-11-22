#ifndef NIVEL2_H
#define NIVEL2_H

#include "nivel.h"
#include "tanque.h"
#include "enemigo.h"
#include "proyectil.h"
#include <vector>

class Nivel2 : public Nivel {
public:
    Nivel2(float anchoScene, float altoScene);

    void actualizar(float dt) override;
    void reiniciar() override;

    bool haTerminado() const override;
    bool haGanado() const override;

    Personaje*       getJugador()       override { return &jugador; }
    const Personaje* getJugador() const override { return &jugador; }

    // Controles que se usan en este nivel:
    // W / S para mover tanque en Y, I para disparar.
    void moverArribaPresionado(bool presionado) override;
    void moverAbajoPresionado(bool presionado) override;
    void dispararPresionado(bool presionado) override;

    // Los demás (izquierda, derecha, salto, agachar) no se usan aquí.

    // Métodos auxiliares para la parte gráfica
    const Enemigo& getJefe() const { return jefe; }
    const std::vector<Proyectil>& getBalasJugador() const { return jugador.getProyectiles(); }
    const std::vector<Proyectil>& getBalasEnemigo() const { return jefe.getProyectiles(); }

private:
    float anchoScene;
    float altoScene;

    Tanque  jugador;
    Enemigo jefe;

    bool gano;
    bool perdio;

    // Flags de entrada
    bool flagMoverArriba;
    bool flagMoverAbajo;
    bool flagDisparar;

    // Lógica interna
    void actualizarJugador(float dt);
    void actualizarEnemigo(float dt);
    void manejarColisiones();
};

#endif // NIVEL2_H
