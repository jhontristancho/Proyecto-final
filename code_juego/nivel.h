#ifndef NIVEL_H
#define NIVEL_H

#include "personaje.h"

// Clase base abstracta para los niveles.
class Nivel {
public:
    virtual ~Nivel() = default;

    // Llamado cada tick de juego (dt en segundos)
    virtual void actualizar(float dt) = 0;

    // Reinicia el nivel
    virtual void reiniciar() = 0;

    // Estado del nivel
    virtual bool haTerminado() const = 0; // true si ganó o perdió
    virtual bool haGanado()   const = 0;  // true si ganó

    // Personaje principal (Soldado, Tanque o Avión)
    virtual Personaje*       getJugador()       = 0;
    virtual const Personaje* getJugador() const = 0;

    // ---- Hooks de entrada (lógicos, NO Qt) ----
    virtual void moverArribaPresionado(bool)    {}
    virtual void moverAbajoPresionado(bool)     {}
    virtual void moverIzquierdaPresionado(bool) {}
    virtual void moverDerechaPresionado(bool)   {}
    virtual void saltarPresionado(bool)         {}
    virtual void agacharPresionado(bool)        {}
    virtual void dispararPresionado(bool)       {}
};

#endif // NIVEL_H
