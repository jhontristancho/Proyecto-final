#ifndef TANQUE_H
#define TANQUE_H
#include "Personaje.h"
#include <vector>
struct ProyectilData {
    float x;
    float y;
    float velocidad;
    bool activo;
};
class Tanque : public Personaje {
private:
    float velocidadMovimiento;
    std::vector<ProyectilData> proyectiles;
    float velocidadProyectil;
    float cooldown;
    float tiempoDesdeUltDisparo;
public:
    Tanque(float px, float py);
    void moverArriba(float dt);
    void moverAbajo(float dt);
    void disparar();
    void tomarDanio(int d);
    void actualizar(float dt) override;
    const std::vector<ProyectilData>& getProyectiles() const { return proyectiles; }
};

#endif
