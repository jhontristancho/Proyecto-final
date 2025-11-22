#include "controlador_juego.h"

// Ajusta los nombres de los headers según tu proyecto
#include "nivel_1.h"
#include "nivel_2.h"
#include "nivel_3.h"

controlador_juego::controlador_juego(float ancho, float alto)
    : anchoScene(ancho),
    altoScene(alto),
    nivelActual(nullptr),
    numeroNivelActual(0),
    modoHistoria(false),
    resultadoPendiente(false),
    ultimoResultadoFueVictoria(false),
    juegoCompletado(false),
    mensajeHistoriaActual("")
{
}

void controlador_juego::setTamanoScene(float ancho, float alto)
{
    anchoScene = ancho;
    altoScene  = alto;

    // Si quieres, podrías recargar el nivel actual con el nuevo tamaño
    // if (nivelActual) {
    //     cargarNivel(numeroNivelActual);
    // }
}

// ----------------- Menú: seleccionar modos -----------------

void controlador_juego::jugarNivel(int numeroNivel)
{
    if (numeroNivel < 1 || numeroNivel > 3)
        return;

    modoHistoria        = false;
    numeroNivelActual   = numeroNivel;
    resultadoPendiente  = false;
    ultimoResultadoFueVictoria = false;
    juegoCompletado     = false;
    mensajeHistoriaActual.clear();

    cargarNivel(numeroNivelActual);
}

void controlador_juego::jugarHistoria()
{
    modoHistoria        = true;
    numeroNivelActual   = 1;
    resultadoPendiente  = false;
    ultimoResultadoFueVictoria = false;
    juegoCompletado     = false;
    mensajeHistoriaActual.clear();

    cargarNivel(numeroNivelActual);
}

void controlador_juego::reiniciarNivelActual()
{
    if (nivelActual) {
        nivelActual->reiniciar();
        resultadoPendiente          = false;
        ultimoResultadoFueVictoria  = false;
        mensajeHistoriaActual.clear();
        // juegoCompletado lo dejamos como está: si estaba en true, será porque
        // acabó toda la historia; normalmente no reiniciarías en ese punto.
    }
}

void controlador_juego::avanzarAlSiguienteNivelHistoria()
{
    if (!modoHistoria)
        return;
    if (!nivelActual)
        return;
    if (numeroNivelActual >= 3)
        return; // ya no hay siguientes niveles

    numeroNivelActual++;
    cargarNivel(numeroNivelActual);

    // Limpiamos el resultado anterior, empezamos nuevo nivel
    resultadoPendiente          = false;
    ultimoResultadoFueVictoria  = false;
    mensajeHistoriaActual.clear();
}

// ----------------- Carga de niveles -----------------

void controlador_juego::cargarNivel(int numero)
{
    switch (numero) {
    case 1:
        nivelActual = std::make_unique<Nivel1>(anchoScene, altoScene);
        break;
    case 2:
        nivelActual = std::make_unique<Nivel2>(anchoScene, altoScene);
        break;
    case 3:
        nivelActual = std::make_unique<Nivel3>(anchoScene, altoScene);
        break;
    default:
        nivelActual.reset();
        break;
    }
}

// ----------------- Bucle principal -----------------

void controlador_juego::actualizar(float dt)
{
    // Por cada tick, asumimos que no ha pasado nada nuevo
    resultadoPendiente          = false;
    ultimoResultadoFueVictoria  = false;
    mensajeHistoriaActual.clear();
    // juegoCompletado se mantiene si ya se completó la historia

    if (!nivelActual)
        return;

    nivelActual->actualizar(dt);

    if (!nivelActual->haTerminado())
        return;

    // Si llegó aquí, el nivel terminó
    bool ganoNivel = nivelActual->haGanado();

    resultadoPendiente         = true;
    ultimoResultadoFueVictoria = ganoNivel;

    if (ganoNivel) {
        if (modoHistoria) {
            // En historia, revisar si acabó la campaña
            if (numeroNivelActual < 3) {
                construirMensajeVictoria(numeroNivelActual);
                // OJO: NO cambiamos de nivel aquí, la GUI llamará a avanzarAlSiguienteNivelHistoria()
            } else {
                // Ganó el nivel 3 en historia
                construirMensajeJuegoCompletado();
                juegoCompletado = true;
            }
        } else {
            // Nivel individual
            construirMensajeVictoria(numeroNivelActual);
        }
    } else {
        // Derrota
        construirMensajeDerrota(numeroNivelActual);
    }
}

void controlador_juego::limpiarResultado()
{
    resultadoPendiente         = false;
    ultimoResultadoFueVictoria = false;
    mensajeHistoriaActual.clear();
    // juegoCompletado no se limpia aquí; se queda a true hasta que empieces otra partida
}

// ----------------- Mensajes de historia -----------------

void controlador_juego::construirMensajeVictoria(int nivel)
{
    switch (nivel) {
    case 1:
        mensajeHistoriaActual =
            "¡Victoria en el Nivel 1!\n"
            "Has asegurado la cabeza de playa bajo intenso fuego enemigo.";
        break;
    case 2:
        mensajeHistoriaActual =
            "¡Victoria en el Nivel 2!\n"
            "El frente terrestre ha resistido y el avance aliado continúa.";
        break;
    case 3:
        mensajeHistoriaActual =
            "¡Victoria en el Nivel 3!\n"
            "Dominas el cielo y has debilitado la capacidad aérea enemiga.";
        break;
    default:
        mensajeHistoriaActual = "¡Nivel completado!";
        break;
    }
}

void controlador_juego::construirMensajeDerrota(int nivel)
{
    switch (nivel) {
    case 1:
        mensajeHistoriaActual =
            "Has caído en el Nivel 1.\n"
            "Las fuerzas enemigas han recuperado terreno. Inténtalo de nuevo.";
        break;
    case 2:
        mensajeHistoriaActual =
            "Has sido derrotado en el Nivel 2.\n"
            "El frente se ha roto, pero aún hay esperanza.";
        break;
    case 3:
        mensajeHistoriaActual =
            "Has perdido en el Nivel 3.\n"
            "El control del cielo vuelve a manos enemigas. No te rindas.";
        break;
    default:
        mensajeHistoriaActual = "Has sido derrotado.";
        break;
    }
}

void controlador_juego::construirMensajeJuegoCompletado()
{
    mensajeHistoriaActual =
        "¡Has completado la campaña!\n"
        "Tus acciones han sido decisivas en la victoria aliada durante la Segunda Guerra Mundial.\n"
        "Gracias por jugar.";
}
