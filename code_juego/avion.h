#ifndef AVION_H
#define AVION_H

#include "personaje.h"
#include "proyectil.h"
#include <vector>

class Avion : public Personaje {
private:
    float velocidadHorizontal;      // Velocidad objetivo horizontal
    float suavizado;                // Para que el movimiento se vea fluido
    float velocidadProyectil;       // Velocidad de las balas
    float cooldown;                 // Tiempo mínimo entre disparos
    float tiempoDesdeUltDisparo;    // Acumulador de tiempo desde el último disparo
    std::vector<Proyectil> proyectiles;

    // Límites horizontales donde se puede mover el avión (en X)
    float limiteIzquierdoX;
    float limiteDerechoX;

public:
    Avion(float px, float py);

    // El controlador o el nivel usan esto para fijar límites según anchoScene
    void setLimitesX(float xMin, float xMax) {
        limiteIzquierdoX = xMin;
        limiteDerechoX   = xMax;
    }

    void moverDerecha(float dt);
    void moverIzquierda(float dt);
    void disparar();
    void tomarDanio(int d);
    void actualizar(float dt) override;

    const std::vector<Proyectil>& getProyectiles() const;
};

#endif
