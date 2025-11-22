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

    // Límites verticales donde se puede mover el tanque (coordenadas Y)
    float limiteSuperiorY;
    float limiteInferiorY;

public:
    Tanque(float px, float py);

    // El nivel configura los límites verticales
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
    std::vector<Proyectil>&       getProyectilesMutable();
};

#endif
