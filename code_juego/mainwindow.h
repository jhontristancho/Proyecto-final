#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>            // NUEVO
#include "nivel.h"           // NUEVO
#include "controlador_juego.h" // NUEVO :contentReference[oaicite:1]{index=1}

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:                              // NUEVO
    void keyPressEvent(QKeyEvent *e) override;    // NUEVO
    void keyReleaseEvent(QKeyEvent *e) override;  // NUEVO

private slots:                         // NUEVO
    void actualizarJuego();            // NUEVO

private:
    Ui::MainWindow *ui;
    QTimer *timer;                     // NUEVO
    controlador_juego ctrl;            // NUEVO
    Nivel *nivelActual;                // NUEVO (Nivel1, Nivel2, etc. más adelante)
};

#endif // MAINWINDOW_H
