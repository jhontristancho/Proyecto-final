#include "soldado.h"

Soldado::Soldado(float px, float py)
    : Personaje(px, py, 32.0f, 48.0f, 100),
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
        float nuevoAlto = alto / 2.0f;
        // mantener los "pies" en el suelo
        y += (alto - nuevoAlto);
        alto = nuevoAlto;
    } else if (!estado && agachado) {
        // volver a la altura original manteniendo pies
        y -= (altoOriginal - alto);
        alto = altoOriginal;
        agachado = false;
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
    }

    y += vy * dt;

    // Usar el suelo configurable en vez de 300.0f
    if (y >= ySuelo) {
        y = ySuelo;
        vy = 0.0f;
        enSuelo = true;
    }
}
