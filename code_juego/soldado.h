#ifndef SOLDADO_H
#define SOLDADO_H
#include "Personaje.h"
class Soldado : public Personaje {
private:
    bool enSuelo;
    bool agachado;
    float gravedad;
    float fuerzaSalto;
    float velocidadHorizontal;
    float altoOriginal;
public:
    Soldado(float px, float py);
    void saltar();
    void agachar(bool estado);
    void moverDerecha(float dt);
    void moverIzquierda(float dt);
    void tomarDanio(int d);
    void actualizar(float dt) override;
};

#endif
