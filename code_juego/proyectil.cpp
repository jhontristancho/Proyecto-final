#include "proyectil.h"
#include "personaje.h"

Proyectil::Proyectil(float px, float py, float vX, float vY, int d, bool delJugador)
    : x(px),
    y(py),
    vx(vX),
    vy(vY),
    danio(d),
    activo(true),
    esDelJugador(delJugador),
    // Valores por defecto; luego el nivel puede ajustarlos
    limiteIzquierdoX(0.0f),
    limiteDerechoX(1200.0f),
    limiteSuperiorY(0.0f),
    limiteInferiorY(800.0f)
{
}

void Proyectil::setLimitesEscena(float xMin, float xMax, float yMin, float yMax) {
    limiteIzquierdoX = xMin;
    limiteDerechoX   = xMax;
    limiteSuperiorY  = yMin;
    limiteInferiorY  = yMax;
}

void Proyectil::actualizar(float dt) {
    if (!activo) return;

    x += vx * dt;
    y += vy * dt;

    // Si sale fuera de los límites, se desactiva
    if (x < limiteIzquierdoX || x > limiteDerechoX ||
        y < limiteSuperiorY  || y > limiteInferiorY) {
        activo = false;
    }
}

bool Proyectil::estaActivo() const {
    return activo;
}

void Proyectil::desactivar() {
    activo = false;
}

bool Proyectil::colisionaCon(const Personaje& p) const {
    if (!activo) return false;

    float px = p.getX();
    float py = p.getY();
    float pw = p.getAncho();
    float ph = p.getAlto();

    float x1 = px;
    float y1 = py;
    float x2 = px + pw;
    float y2 = py + ph;

    bool dentroX = (x >= x1) && (x <= x2);
    bool dentroY = (y >= y1) && (y <= y2);

    return dentroX && dentroY;
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
