#ifndef PROYECTIL_H
#define PROYECTIL_H

class Personaje; // Declaración anticipada para colisiones

class Proyectil {
private:
    float x, y;
    float vx, vy;
    int danio;
    bool activo;
    bool esDelJugador;

    // Límites de la escena (como en Obstaculo)
    float limiteIzquierdoX;
    float limiteDerechoX;
    float limiteSuperiorY;
    float limiteInferiorY;

public:
    Proyectil(float px, float py, float vX, float vY, int danio, bool delJugador);

    // Permite ajustar los límites según el tamaño de la escena (controlador/nivel)
    void setLimitesEscena(float xMin, float xMax, float yMin, float yMax);

    void actualizar(float dt);
    bool estaActivo() const;
    void desactivar();

    // Colisión simple: el proyectil se considera un punto que entra en el rectángulo del personaje
    bool colisionaCon(const Personaje& p) const;

    // Getters
    float getX() const;
    float getY() const;
    int   getDanio() const;
    bool  esAliado() const;
};

#endif
