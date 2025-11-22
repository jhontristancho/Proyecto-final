#ifndef AVION_H
#define AVION_H

#include "personaje.h"
#include "proyectil.h"
#include <vector>

class Avion : public Personaje {
private:
    float velocidadHorizontal;      // velocidad de movimiento en X
    float velocidadProyectil;       // velocidad de las balas
    float cooldown;                 // tiempo mínimo entre disparos
    float tiempoDesdeUltDisparo;    // acumulador

    std::vector<Proyectil> proyectiles;

    // límites horizontales donde puede moverse el avión
    float limiteIzquierdoX;
    float limiteDerechoX;

public:
    Avion(float px, float py);

    // El nivel configura los límites en X
    void setLimitesX(float xMin, float xMax) {
        limiteIzquierdoX = xMin;
        limiteDerechoX   = xMax;
    }

    void moverDerecha(float dt);
    void moverIzquierda(float dt);
    void disparar();
    void tomarDanio(int d);
    void actualizar(float dt) override;

    // Proyectiles del avión
    const std::vector<Proyectil>& getProyectiles() const;
    std::vector<Proyectil>&       getProyectilesMutable();
};

#endif
