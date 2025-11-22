#ifndef ANIMACIONES_H
#define ANIMACIONES_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QTimer>

// Clase genérica para animar cualquier sprite de tu juego
// (soldado, tanque, avión, enemigos, obstáculos, proyectiles, etc.)
class animaciones : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    // rutaSpriteSheet: PNG del spritesheet
    // columnas / filas: cómo está dividido el sheet
    explicit animaciones(const QString &rutaSpriteSheet,
                         int columnas = 1,
                         int filas    = 1,
                         QObject *parent = nullptr);

    // Cambia la fila de animación (0..filas-1) dentro del mismo spritesheet
    void configurarFilaAnimacion(int fila);

    // Inicia / detiene la animación (temporizador interno)
    void iniciar(int intervaloMs = 100);
    void detener();

    // Si el spritesheet tiene varios personajes uno al lado de otro,
    // selecciona cuál usar (por defecto 0).
    void configurarIndicePersonaje(int indice);

    // Ajusta la escala del sprite para que encaje con el tamaño lógico
    // de tu entidad (ancho/alto de la hitbox en el mundo).
    void ajustarATamanoLogico(float anchoLogico, float altoLogico);

    // Voltea el sprite horizontalmente (útil para mirar a la izquierda)
    void mirarHaciaLaIzquierda(bool haciaLaIzquierda);

    // Por si quieres consultar el tamaño original de los frames
    int getAnchoFrame() const { return anchoFrame; }
    int getAltoFrame()  const { return altoFrame;  }

private slots:
    void actualizarFrame();

private:
    QPixmap hojaSprites;        // imagen completa
    QPoint  offsetPersonaje;    // desplazamiento si hay varios personajes en una hoja

    int columnas;
    int filas;
    int frameActual;            // índice de frame actual (columna)
    int filaActual;             // fila actual (animación seleccionada)
    int anchoFrame;             // ancho de cada frame
    int altoFrame;              // alto de cada frame

    QTimer temporizador;        // controla la velocidad de animación
    float factorEscala;         // escala del sprite respecto al tamaño original
    bool  invertidoHorizontal;  // true si está espejado horizontalmente
};

#endif // ANIMACIONES_H
