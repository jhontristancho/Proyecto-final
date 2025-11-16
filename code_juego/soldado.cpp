#include "Soldado.h"
Soldado::Soldado(float px, float py)
    : Personaje(px, py, 32.0f, 48.0f, 100),
    enSuelo(true),
    agachado(false),
    gravedad(900.0f),
    fuerzaSalto(-450.0f),
    velocidadHorizontal(200.0f)
{
    altoOriginal = alto;
}
void Soldado::saltar() {
    if (enSuelo) {
        vy = fuerzaSalto;
        enSuelo = false;
    }
}
void Soldado::agachar(bool estado) {
    if (estado && !agachado) {
        agachado = true;
        alto = alto / 2.0f;
    }
    else if (!estado && agachado) {
        agachado = false;
        alto = altoOriginal;
    }
}
void Soldado::moverDerecha(float dt) {
    x += velocidadHorizontal * dt;
}
void Soldado::moverIzquierda(float dt) {
    x -= velocidadHorizontal * dt;
}
void Soldado::actualizar(float dt) {
    if (!enSuelo) {
        vy += gravedad * dt;
    }
    y += vy * dt;
    if (y >= 300.0f) {
        y = 300.0f;
        vy = 0.0f;
        enSuelo = true;
    }
}
