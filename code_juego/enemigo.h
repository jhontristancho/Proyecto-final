#ifndef ENEMIGO_H
#define ENEMIGO_H

#include "personaje.h"
#include "proyectil.h"
#include <vector>

// Enemigo genérico. Para el jefe del nivel 2 se crea con jefe = true.
class Enemigo : public Personaje {
private:
    // Movimiento base
    float velX;
    float velY;

    // Disparo
    float velocidadProyectil;
    float cooldown;
    float tiempoDesdeUltDisparo;
    std::vector<Proyectil> proyectiles;

    // Inteligencia (para jefe nivel 2)
    bool  esJefe;
    int   fase;               // 1, 2, 3 según la vida
    bool  amenazaDetectada;   // PERCEPCIÓN: ¿hay bala peligrosa cerca?
    float yAmenaza;           // Y de la bala más peligrosa
    int   golpesRecibidos;    // APRENDIZAJE: cuántas veces lo han golpeado
    float velocidadEsquiva;   // APRENDIZAJE: qué tan rápido esquiva

    // Límites de escena (opcionales)
    float limiteIzquierdoX;
    float limiteDerechoX;
    float limiteSuperiorY;
    float limiteInferiorY;

public:
    Enemigo(float px, float py, bool jefe = false);

    // --- PERCEPCIÓN ---
    // El nivel le pasa las balas del jugador:
    void percibirProyectiles(const std::vector<Proyectil>& balasJugador);

    // --- CONFIGURACIÓN GENERAL ---
    void setVelocidadBase(float vX, float vY) { velX = vX; velY = vY; }

    void setLimitesEscena(float xMin, float xMax, float yMin, float yMax) {
        limiteIzquierdoX = xMin;
        limiteDerechoX   = xMax;
        limiteSuperiorY  = yMin;
        limiteInferiorY  = yMax;
    }

    void setParametrosDisparo(float velProj, float cd) {
        velocidadProyectil = velProj;
        cooldown           = cd;
    }

    // --- ACCIÓN + APRENDIZAJE ---
    void disparar();           // Acción de atacar
    void tomarDanio(int d);    // Percibe daño y actualiza aprendizaje
    void actualizar(float dt) override; // Razonamiento + acción + disparo

    const std::vector<Proyectil>& getProyectiles() const { return proyectiles; }
    std::vector<Proyectil>& getProyectilesMutable()      { return proyectiles; }

    bool esJefeFinal() const        { return esJefe; }
    int  getFase() const            { return fase; }
    int  getGolpesRecibidos() const { return golpesRecibidos; }

private:
    // --- RAZONAMIENTO ---
    void razonar();          // interpreta amenaza y decide movimiento en Y

    // --- ACCIÓN ---
    void actuar(float dt);   // aplica movimiento según la decisión

    // --- APRENDIZAJE ---
    void aprender();         // ajusta velocidadEsquiva según golpesRecibidos
    void actualizarFase();   // cambia de fase si es jefe

    // --- SOPORTE ---
    void limpiarProyectilesInactivos();
};

#endif // ENEMIGO_H
