#include "soldado.h"

Soldado::Soldado(float px, float py)
    : Personaje(px, py, 32.0f, 48.0f, 999999),
    enSuelo(true),
    agachado(false),
    gravedad(900.0f),
    fuerzaSalto(-450.0f),
    velocidadHorizontal(200.0f),
    altoOriginal(alto),
    ySuelo(300.0f) // valor por defecto; luego lo puedes sobreescribir con setSuelo()
{
}

void Soldado::saltar() {
    if (!estaVivo()) return;
    if (enSuelo) {
        vy = fuerzaSalto;
        enSuelo = false;
    }
}

void Soldado::agachar(bool estado) {
    if (!estaVivo()) return;

    if (estado && !agachado) {
        agachado = true;

        // Guardamos la posición de los pies antes de cambiar la altura
        float piesY = y + alto;

        alto = altoOriginal / 2.0f;   // mitad de alto cuando está agachado
        y    = piesY - alto;          // recolocamos para que los pies se mantengan

    } else if (!estado && agachado) {
        agachado = false;

        float piesY = y + alto;

        alto = altoOriginal;          // volvemos a la altura completa
        y    = piesY - alto;          // y recalculamos la Y de la cabeza
    }
}

void Soldado::moverDerecha(float dt) {
    if (!estaVivo()) return;
    x += velocidadHorizontal * dt;
}

void Soldado::moverIzquierda(float dt) {
    if (!estaVivo()) return;
    x -= velocidadHorizontal * dt;
}

void Soldado::tomarDanio(int d) {
    recibirDanio(d);
    if (!estaVivo()) {
        // al morir, garantizamos que el hitbox vuelve a su tamaño original
        if (agachado) {
            y -= (altoOriginal - alto);
            alto = altoOriginal;
            agachado = false;
        }
    }
}

void Soldado::actualizar(float dt) {
    if (!estaVivo()) return;

    if (!enSuelo) {
        vy += gravedad * dt;
        y  += vy * dt;

        // Comprobamos colisión con el suelo usando los "pies"
        if (y + alto >= ySuelo) {
            y  = ySuelo - alto;  // cabeza justo donde corresponde
            vy = 0.0f;
            enSuelo = true;
        }
    }
}
bool Soldado::estaAgachado() const { return agachado; }
bool Soldado::estaEnSuelo() const { return enSuelo; }
