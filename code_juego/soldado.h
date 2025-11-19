#ifndef SOLDADO_H
#define SOLDADO_H

#include "personaje.h"

class Soldado : public Personaje {
private:
    bool enSuelo;
    bool agachado;
    float gravedad;
    float fuerzaSalto;
    float velocidadHorizontal;
    float altoOriginal;

    float ySuelo;

public:
    Soldado(float px, float py);

    // el controlador/nivel configura el suelo
    void setSuelo(float y) { ySuelo = y; }

    void saltar();
    void agachar(bool estado);
    void moverDerecha(float dt);
    void moverIzquierda(float dt);
    void tomarDanio(int d);
    void actualizar(float dt) override;
};

#endif
