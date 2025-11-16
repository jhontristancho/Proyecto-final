#ifndef AVION_H
#define AVION_H
#include "Personaje.h"
class Avion : public Personaje {
private:
    float velocidadHorizontal;
    float suavizado;//para que el movimieno se vea fluido
public:
    Avion(float px, float py);
    void moverDerecha(float dt);
    void moverIzquierda(float dt);
    void tomarDanio(int d);
    void actualizar(float dt) override;
};

#endif
