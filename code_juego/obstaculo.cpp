#include "obstaculo.h"

Obstaculo::Obstaculo(float px, float py,
                     float w, float h,
                     float vX, float vY,
                     int d)
    : x(px),
    y(py),
    ancho(w),
    alto(h),
    vx(vX),
    vy(vY),
    danio(d),
    activo(true),
    // Valores por defecto de los límites; el nivel debe sobreescribirlos
    // con setLimitesEscena() usando anchoScene y altoScene.
    limiteIzquierdoX(0.0f),
    limiteDerechoX(1200.0f),
    limiteSuperiorY(0.0f),
    limiteInferiorY(800.0f)
{
}

void Obstaculo::setLimitesEscena(float xMin, float xMax, float yMin, float yMax) {
    limiteIzquierdoX = xMin;
    limiteDerechoX   = xMax;
    limiteSuperiorY  = yMin;
    limiteInferiorY  = yMax;
}

void Obstaculo::actualizar(float dt) {
    if (!activo)
        return;

    x += vx * dt;
    y += vy * dt;

    // Si se sale completamente de la escena, lo desactivamos.
    // Usamos el rectángulo del obstáculo (x, y, ancho, alto).
    if (x + ancho < limiteIzquierdoX ||
        x > limiteDerechoX ||
        y + alto < limiteSuperiorY ||
        y > limiteInferiorY) {
        activo = false;
    }
}

bool Obstaculo::colisionaCon(const Personaje& p) const {
    if (!activo || !p.estaVivo())
        return false;

    float px = p.getX();
    float py = p.getY();
    float pw = p.getAncho();
    float ph = p.getAlto();

    // Colisión AABB (Axis Aligned Bounding Box)
    bool noColision =
        (px + pw < x)        ||  // personaje completamente a la izquierda
        (px > x + ancho)     ||  // personaje completamente a la derecha
        (py + ph < y)        ||  // personaje completamente arriba
        (py > y + alto);         // personaje completamente abajo

    return !noColision;
}
