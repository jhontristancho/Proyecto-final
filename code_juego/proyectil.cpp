#include "proyectil.h"
#include "personaje.h"

Proyectil::Proyectil(float px, float py, float vX, float vY, int d, bool delJugador)
    : x(px), y(py), vx(vX), vy(vY), danio(d), activo(true), esDelJugador(delJugador)
{
}

void Proyectil::actualizar(float dt) {
    if (!activo) return;

    x += vx * dt;
    y += vy * dt;

    // Límites del mundo: AJUSTAR según resolución de la ventana/juego
    if (x < 0.0f || x > 1200.0f || y < 0.0f || y > 800.0f)
        activo = false;
}

bool Proyectil::estaActivo() const {
    return activo;
}

void Proyectil::desactivar() {
    activo = false;
}

bool Proyectil::colisionaCon(const Personaje& p) const {
    if (!activo || !p.estaVivo())
        return false;

    float px = p.getX();
    float py = p.getY();
    float pw = p.getAncho();
    float ph = p.getAlto();

    // Proyectil tratado como punto contra AABB del personaje
    return (x >= px && x <= px + pw &&
            y >= py && y <= py + ph);
}

float Proyectil::getX() const {
    return x;
}

float Proyectil::getY() const {
    return y;
}

int Proyectil::getDanio() const {
    return danio;
}

bool Proyectil::esAliado() const {
    return esDelJugador;
}
