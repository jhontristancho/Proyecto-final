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
        alto = alto / 2.0f;
    }
    else if (!estado && agachado) {
        agachado = false;
        alto = altoOriginal;
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
    if (!estaVivo()) return;
    recibirDanio(d);
    if (vida == 0) {//efecto especifico cuando recibe el daño pero solo para soldado
        vx = 0;
        vy = 0;
        agachado = false;
    }
}
void Soldado::actualizar(float dt) {
    if (!estaVivo()) {
        return;
    }
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
