#include "animaciones.h"

#include <QDebug>
#include <QTransform>
#include <algorithm> // std::min

animaciones::animaciones(const QString &rutaSpriteSheet,
                         int columnas_,
                         int filas_,
                         QObject *parent)
    : QObject(parent),
    QGraphicsPixmapItem(),
    hojaSprites(QPixmap(rutaSpriteSheet)),
    offsetPersonaje(0, 0),
    columnas(columnas_),
    filas(filas_),
    frameActual(0),
    filaActual(0),
    anchoFrame(0),
    altoFrame(0),
    factorEscala(1.0f),
    invertidoHorizontal(false)
{
    if (hojaSprites.isNull()) {
        qWarning() << "No se pudo cargar el spritesheet:" << rutaSpriteSheet;
        return;
    }

    if (columnas <= 0) columnas = 1;
    if (filas    <= 0) filas    = 1;

    anchoFrame = hojaSprites.width()  / columnas;
    altoFrame  = hojaSprites.height() / filas;

    // Primer frame por defecto
    setPixmap(hojaSprites.copy(0, 0, anchoFrame, altoFrame));
    setTransformOriginPoint(anchoFrame / 2.0, altoFrame / 2.0);

    connect(&temporizador, &QTimer::timeout,
            this, &animaciones::actualizarFrame);
}

void animaciones::configurarFilaAnimacion(int fila)
{
    if (fila >= 0 && fila < filas) {
        filaActual = fila;
    }
}

void animaciones::iniciar(int intervaloMs)
{
    temporizador.start(intervaloMs);
}

void animaciones::detener()
{
    temporizador.stop();
}

void animaciones::configurarIndicePersonaje(int indice)
{
    if (anchoFrame <= 0 || altoFrame <= 0)
        return;

    // Suponemos que cada personaje ocupa un bloque de (columnas x filas) frames
    int personajesPorFila = hojaSprites.width() / (anchoFrame * columnas);
    if (personajesPorFila <= 0)
        personajesPorFila = 1;

    int charX = (indice % personajesPorFila) * columnas * anchoFrame;
    int charY = (indice / personajesPorFila) * filas    * altoFrame;

    offsetPersonaje = QPoint(charX, charY);
}

void animaciones::ajustarATamanoLogico(float anchoLogico, float altoLogico)
{
    if (anchoFrame <= 0 || altoFrame <= 0)
        return;

    float sx = anchoLogico / static_cast<float>(anchoFrame);
    float sy = altoLogico  / static_cast<float>(altoFrame);

    factorEscala = std::min(sx, sy); // mantenemos proporción
}

void animaciones::mirarHaciaLaIzquierda(bool haciaLaIzquierda)
{
    invertidoHorizontal = haciaLaIzquierda;
}

void animaciones::actualizarFrame()
{
    if (hojaSprites.isNull())
        return;

    int x = offsetPersonaje.x() + (frameActual % columnas) * anchoFrame;
    int y = offsetPersonaje.y() + filaActual              * altoFrame;

    QPixmap frame = hojaSprites.copy(x, y, anchoFrame, altoFrame);

    // Espejo horizontal si hace falta
    if (invertidoHorizontal) {
        frame = frame.transformed(QTransform().scale(-1, 1));
    }

    // Escalado al tamaño lógico
    if (factorEscala != 1.0f) {
        frame = frame.scaled(anchoFrame * factorEscala,
                             altoFrame  * factorEscala,
                             Qt::KeepAspectRatio,
                             Qt::SmoothTransformation);
    }

    setPixmap(frame);
    setTransformOriginPoint(boundingRect().width() / 2.0,
                            boundingRect().height() / 2.0);

    frameActual = (frameActual + 1) % columnas;
}
