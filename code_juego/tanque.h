#ifndef TANQUE_H
#define TANQUE_H

#include "personaje.h"
#include "proyectil.h"
#include <vector>

class Tanque : public Personaje {
private:
    float velocidadMovimiento;      // Movimiento vertical (eje Y)
    float velocidadProyectil;       // Velocidad horizontal del proyectil
    float cooldown;                 // Tiempo entre disparos
    float tiempoDesdeUltDisparo;    // Acumulador de tiempo
    std::vector<Proyectil> proyectiles; // Balas disparadas por el tanque

    // Límites verticales donde se puede mover el tanque (en coordenadas Y)
    float limiteSuperiorY;          // valor mínimo de y
    float limiteInferiorY;          // valor máximo de y

public:
    Tanque(float px, float py);

    // El controlador o el nivel usarán esto para fijar límites
    void setLimitesVerticales(float yMin, float yMax) {
        limiteSuperiorY = yMin;
        limiteInferiorY = yMax;
    }

    void moverArriba(float dt);
    void moverAbajo(float dt);
    void disparar();
    void tomarDanio(int d);
    void actualizar(float dt) override;

    const std::vector<Proyectil>& getProyectiles() const;
};

#endif
