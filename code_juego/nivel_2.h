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
    bool haGanado()   const override;

    Personaje*       getJugador()       override { return &jugador; }
    const Personaje* getJugador() const override { return &jugador; }

    // Hooks de entrada que este nivel SÍ usa
    void moverArribaPresionado(bool presionado) override;
    void moverAbajoPresionado(bool presionado) override;
    void dispararPresionado(bool presionado) override;

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

    bool colisionRect(const Personaje& a, const Personaje& b) const;
};

#endif // NIVEL2_H
