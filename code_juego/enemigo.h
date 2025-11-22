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

    bool esJefe;
    int  fase;
    int  golpesRecibidos;

    // Rango vertical permitido
    float limiteSuperiorY;
    float limiteInferiorY;

    // Parámetros de "IA"
    float velocidadEsquiva;
    bool  quiereMoverArriba;
    bool  quiereMoverAbajo;

public:
    Enemigo(float px, float py, bool jefe);

    void setLimitesVerticales(float yMin, float yMax) {
        limiteSuperiorY = yMin;
        limiteInferiorY = yMax;
    }

    // PERCEPCIÓN: recibe balas del jugador y decide si hay amenaza
    void percibirProyectiles(const std::vector<Proyectil>& balasJugador);

    // El jugador le hace daño
    void tomarDanio(int d);

    // Actualización general (incluye razonamiento, acción, disparo)
    void actualizar(float dt) override;

    const std::vector<Proyectil>& getProyectiles() const;
    std::vector<Proyectil>&       getProyectilesMutable();

private:
    // RAZONAMIENTO: decide hacia dónde moverse
    void razonar();

    // ACCIÓN: aplica el movimiento
    void actuar(float dt);

    // APRENDIZAJE: ajusta parámetros según golpes recibidos
    void aprender();
    void actualizarFase();

    void disparar();
    void limpiarProyectilesInactivos();
};

#endif // ENEMIGO_H
