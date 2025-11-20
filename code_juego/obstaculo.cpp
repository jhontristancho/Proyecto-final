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
    // Por defecto no hay límites definidos.
    // El nivel/controlador debe llamar a setLimitesEscena().
    limiteIzquierdoX(0.0f),
    limiteDerechoX(0.0f),
    limiteSuperiorY(0.0f),
    limiteInferiorY(0.0f)
{
}

void Obstaculo::actualizar(float dt) {
    if (!activo)
        return;

    x += vx * dt;
    y += vy * dt;

    // Solo comprobamos los límites si están bien definidos
    // (xMin < xMax y yMin < yMax). Así evitamos problemas
    // si alguien se olvida de llamar a setLimitesEscena().
    bool limitesDefinidos =
        (limiteIzquierdoX < limiteDerechoX) &&
        (limiteSuperiorY  < limiteInferiorY);

    if (limitesDefinidos) {
        // Si se sale completamente de la escena, lo desactivamos.
        if (x + ancho < limiteIzquierdoX ||
            x > limiteDerechoX ||
            y + alto < limiteSuperiorY ||
            y > limiteInferiorY) {
            activo = false;
        }
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
        (px + pw < x) ||       // personaje completamente a la izquierda
        (px > x + ancho) ||    // personaje completamente a la derecha
        (py + ph < y) ||       // personaje completamente arriba
        (py > y + alto);       // personaje completamente abajo

    return !noColision;
}
