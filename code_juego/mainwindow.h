#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <vector>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <vector>
#include "controlador_juego.h"
#include "nivel.h"
#include "animaciones.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *e) override;
    void keyReleaseEvent(QKeyEvent *e) override;

private slots:
    void actualizarJuego();

private:
    Ui::MainWindow *ui;

    QGraphicsScene *scene;
    QTimer         *timer;

    controlador_juego ctrl;
    Nivel           *nivelActual;

    // Hitbox lógica del jugador (invisible, útil para depurar)
    QGraphicsRectItem *rectJugador;

    // Sprites de los obstáculos del Nivel 1
    // Sprites ANIMADOS de los obstáculos del Nivel 1
    std::vector<animaciones*> spritesObstaculos;  // uno por obstáculo

    // --- HUD ---
    QGraphicsTextItem *textoTiempo;
    QGraphicsTextItem *textoVida;

    // Flash rojo cuando el jugador recibe daño
    QGraphicsRectItem *damageFlash;

    // Sprites del soldado (TODOS son de la clase genérica animaciones)
    animaciones *soldadoQuieto;
    animaciones *soldadoCorriendo;
    animaciones *soldadoSaltando;
    animaciones *soldadoAgachado;
    animaciones *soldadoMuriendo;
    animaciones *soldadoActual;

    // Obstáculos del Nivel1 (por ahora rectángulos)
    std::vector<QGraphicsRectItem*> rectObstaculos;

    float xJugadorAnterior; // para saber si se está moviendo
    void actualizarHUDNivel1();
    void mostrarDamageFlash();
    QString formatearTiempo(float segundos) const;
    QGraphicsPixmapItem *fondoItem; // único fondo desplazable
    float anchoFondo;               // ancho total del fondo
    void actualizarFondo(float xJugador);
    void ocultarYPararSpritesSoldado();
    void actualizarSpriteSoldado();
    void sincronizarObstaculosNivel1();
};

#endif // MAINWINDOW_H
