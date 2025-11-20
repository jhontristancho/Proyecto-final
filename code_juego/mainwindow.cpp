#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>   // <-- NUEVO

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , timer(new QTimer(this))   // <-- NUEVO
    , nivelActual(nullptr)      // <-- NUEVO
{
    ui->setupUi(this);

    // Más adelante: nivelActual = new Nivel1(ctrl.getAnchoScene(), ctrl.getAltoScene());

    connect(timer, SIGNAL(timeout()), this, SLOT(actualizarJuego()));
    timer->start(16); // 16 ms ~ 60 FPS
}

MainWindow::~MainWindow()
{
    // delete nivelActual;  // Por ahora NO lo usas, así que puedes dejarlo comentado
    delete ui;
}

// SLOT del QTimer
void MainWindow::actualizarJuego()
{
    if (!nivelActual) return;

    const float dt = 0.016f; // por ahora fijo, luego se puede mejorar
    nivelActual->actualizar(dt);

    // Aquí después actualizarás los sprites con setPos(x, y)
}
void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if (!nivelActual) {
        QMainWindow::keyPressEvent(e);
        return;
    }

    switch (e->key()) {
    case Qt::Key_W:
        nivelActual->moverArribaPresionado(true);
        break;
    case Qt::Key_S:
        nivelActual->moverAbajoPresionado(true);
        break;
    case Qt::Key_A:
        nivelActual->moverIzquierdaPresionado(true);
        break;
    case Qt::Key_D:
        nivelActual->moverDerechaPresionado(true);
        break;
    case Qt::Key_Space:
        nivelActual->saltarPresionado(true);
        break;
    case Qt::Key_C:
        nivelActual->agacharPresionado(true);
        break;
    case Qt::Key_I:
        nivelActual->dispararPresionado(true);
        break;
    default:
        QMainWindow::keyPressEvent(e);
        break;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *e)
{
    if (!nivelActual) {
        QMainWindow::keyReleaseEvent(e);
        return;
    }

    switch (e->key()) {
    case Qt::Key_W:
        nivelActual->moverArribaPresionado(false);
        break;
    case Qt::Key_S:
        nivelActual->moverAbajoPresionado(false);
        break;
    case Qt::Key_A:
        nivelActual->moverIzquierdaPresionado(false);
        break;
    case Qt::Key_D:
        nivelActual->moverDerechaPresionado(false);
        break;
    case Qt::Key_Space:
        nivelActual->saltarPresionado(false);
        break;
    case Qt::Key_C:
        nivelActual->agacharPresionado(false);
        break;
    case Qt::Key_I:
        nivelActual->dispararPresionado(false);
        break;
    default:
        QMainWindow::keyReleaseEvent(e);
        break;
    }
}
