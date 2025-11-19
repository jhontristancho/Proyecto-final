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

public:
    Proyectil(float px, float py, float vX, float vY, int danio, bool delJugador);

    void actualizar(float dt);
    bool estaActivo() const;
    void desactivar();

    bool colisionaCon(const Personaje& p) const;

    // Getters
    float getX() const;
    float getY() const;
    int getDanio() const;
    bool esAliado() const;
};

#endif
