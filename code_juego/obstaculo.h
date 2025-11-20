#ifndef OBSTACULO_H
#define OBSTACULO_H

#include "personaje.h"

// Clase genérica de obstáculo para todos los niveles.
// Puede ser estático o moverse con una velocidad (vx, vy).
class Obstaculo {
private:
    float x, y;
    float ancho, alto;
    float vx, vy;    // velocidad del obstáculo (puede ser 0 en alguno de los ejes)
    int   danio;
    bool  activo;

    // Límites donde se considera "fuera de escena"
    float limiteIzquierdoX;
    float limiteDerechoX;
    float limiteSuperiorY;
    float limiteInferiorY;

public:
    // Constructor genérico. El nivel decide posición, tamaño, velocidad y daño.
    Obstaculo(float px, float py,
              float w, float h,
              float vX, float vY,
              int d);

    // El controlador o el nivel ajustan los límites según el tamaño de la escena.
    void setLimitesEscena(float xMin, float xMax, float yMin, float yMax) {
        limiteIzquierdoX = xMin;
        limiteDerechoX   = xMax;
        limiteSuperiorY  = yMin;
        limiteInferiorY  = yMax;
    }

    // Actualiza la posición y desactiva el obstáculo si sale de los límites.
    void actualizar(float dt);

    // Colisión caja contra caja con cualquier Personaje.
    bool colisionaCon(const Personaje& p) const;

    void desactivar()   { activo = false; }
    bool estaActivo() const { return activo; }

    // Getters básicos para la lógica y la parte gráfica (sprites).
    float getX() const    { return x; }
    float getY() const    { return y; }
    float getAncho() const{ return ancho; }
    float getAlto() const { return alto; }
    int   getDanio() const{ return danio; }
};

#endif // OBSTACULO_H
