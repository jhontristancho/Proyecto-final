#ifndef CONTROLADOR_JUEGO_H
#define CONTROLADOR_JUEGO_H

#include <memory>
#include <string>
#include "nivel.h"

// Declaraciones adelantadas de los niveles concretos
class Nivel1;
class Nivel2;
class Nivel3;

class controlador_juego
{
private:
    // Tamaño lógico de la escena
    float anchoScene;
    float altoScene;

    // Nivel actual
    std::unique_ptr<Nivel> nivelActual;
    int  numeroNivelActual;   // 0 = ninguno, 1..3

    // true si está en modo historia (1→2→3), false si juega nivel suelto
    bool modoHistoria;

    // Estado del último update
    bool resultadoPendiente;          // ¿hubo fin de nivel en el último actualizar()?
    bool ultimoResultadoFueVictoria;  // true si fue victoria, false si fue derrota
    bool juegoCompletado;             // true si terminó el nivel 3 en historia

    std::string mensajeHistoriaActual;

public:
    // Puedes seguir llamando controlador_juego() sin parámetros
    controlador_juego(float ancho = 1200.0f, float alto = 800.0f);

    // --- Tamaño de la escena ---
    float getAnchoScene() const { return anchoScene; }
    float getAltoScene()  const { return altoScene;  }

    void setTamanoScene(float ancho, float alto);

    // --- Acceso al nivel actual ---
    Nivel*       getNivelActual()       { return nivelActual.get(); }
    const Nivel* getNivelActual() const { return nivelActual.get(); }

    int  getNumeroNivelActual() const { return numeroNivelActual; }

    bool hayNivelActual() const { return nivelActual != nullptr; }

    // --- Modo de juego ---
    bool esModoHistoria() const       { return modoHistoria; }
    bool esModoNivelIndividual() const { return !modoHistoria && nivelActual != nullptr; }

    // --- Opciones llamadas desde el menú de la GUI ---

    // "Jugar Nivel 1/2/3" (modo nivel suelto)
    void jugarNivel(int numeroNivel);  // 1, 2 o 3

    // "Jugar Historia" (1→2→3)
    void jugarHistoria();

    // Reiniciar nivel actual (por ejemplo, si el usuario elige "Reintentar")
    void reiniciarNivelActual();

    // Avanzar al siguiente nivel en historia (lo llama la GUI cuando el jugador pulse "Continuar")
    void avanzarAlSiguienteNivelHistoria();

    // --- Bucle principal de lógica ---
    void actualizar(float dt);

    // --- Consulta de resultado para la GUI ---

    bool hayResultadoPendiente() const { return resultadoPendiente; }
    bool jugadorGanoNivel()      const { return resultadoPendiente &&  ultimoResultadoFueVictoria; }
    bool jugadorPerdioNivel()    const { return resultadoPendiente && !ultimoResultadoFueVictoria; }
    bool seCompletoJuego()       const { return juegoCompletado; }

    const std::string& getMensajeHistoria() const { return mensajeHistoriaActual; }

    // Después de mostrar el mensaje, la GUI puede limpiar el resultado
    void limpiarResultado();

private:
    void cargarNivel(int numero);  // crea Nivel1/2/3 según el número

    void construirMensajeVictoria(int nivel);
    void construirMensajeDerrota(int nivel);
    void construirMensajeJuegoCompletado();
};

#endif // CONTROLADOR_JUEGO_H
