#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "nivel_1.h"
#include "soldado.h"
#include "obstaculo.h"

#include <QKeyEvent>
#include <QBrush>
#include <QPen>
#include <cmath>
#include <algorithm>

namespace {
constexpr float ANCHO_MUNDO = 1280.0f;
constexpr float ALTO_MUNDO  = 720.0f;

// Ajusta estos valores al nº real de frames de cada PNG
constexpr int COLS_IDLE   = 7; // ej. 1 frame quieto
constexpr int COLS_RUN    = 8; // ej. 6 frames correr
constexpr int COLS_JUMP   = 6; // ej. 6 frames salto
constexpr int COLS_CROUCH = 3; // ej. 3 frames agachado
constexpr int COLS_DEAD   = 4; // ej. 4 frames muerte

// Ajusta estas rutas a donde tengas las imágenes (o usa :/ si tienes .qrc)
const char* RUTA_SOLDADO_IDLE   = ":/nivel1/soldado/Idle.png";
const char* RUTA_SOLDADO_RUN    = ":/nivel1/soldado/Run.png";
const char* RUTA_SOLDADO_JUMP   = ":/nivel1/soldado/saltar.png";
const char* RUTA_SOLDADO_CROUCH = ":/nivel1/soldado/agacharse.png";
const char* RUTA_SOLDADO_DEAD   = ":/nivel1/soldado/Dead.png";
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , scene(new QGraphicsScene(this))
    , timer(new QTimer(this))
    , ctrl(ANCHO_MUNDO, ALTO_MUNDO)
    , nivelActual(nullptr)
    , rectJugador(nullptr)
    , soldadoQuieto(nullptr)
    , soldadoCorriendo(nullptr)
    , soldadoSaltando(nullptr)
    , soldadoAgachado(nullptr)
    , soldadoMuriendo(nullptr)
    , soldadoActual(nullptr)
    , xJugadorAnterior(0.0f)
{
    ui->setupUi(this);

    // Escena y vista
    scene->setSceneRect(0, 0, ANCHO_MUNDO, ALTO_MUNDO);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setFixedSize(static_cast<int>(ANCHO_MUNDO),
                                   static_cast<int>(ALTO_MUNDO));
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // --- Arrancar directamente Nivel 1 para probar ---
    ctrl.jugarNivel(1);
    nivelActual = ctrl.getNivelActual();

    // Rectángulo lógico del jugador (invisible, pero útil para debug)
    if (nivelActual && nivelActual->getJugador()) {
        Personaje *p = nivelActual->getJugador();
        rectJugador = scene->addRect(
            0, 0,
            p->getAncho(),
            p->getAlto(),
            QPen(Qt::NoPen),
            QBrush(Qt::NoBrush)
            );
        rectJugador->setPos(p->getX(), p->getY());
        xJugadorAnterior = p->getX();
    }

    // --- Crear sprites del soldado usando la clase genérica animaciones ---
    soldadoQuieto    = new animaciones(RUTA_SOLDADO_IDLE,   COLS_IDLE,   1, this);
    soldadoCorriendo = new animaciones(RUTA_SOLDADO_RUN,    COLS_RUN,    1, this);
    soldadoSaltando  = new animaciones(RUTA_SOLDADO_JUMP,   COLS_JUMP,   1, this);
    soldadoAgachado  = new animaciones(RUTA_SOLDADO_CROUCH, COLS_CROUCH, 1, this);
    soldadoMuriendo  = new animaciones(RUTA_SOLDADO_DEAD,   COLS_DEAD,   1, this);

    // Añadir todos a la escena
    scene->addItem(soldadoQuieto);
    scene->addItem(soldadoCorriendo);
    scene->addItem(soldadoSaltando);
    scene->addItem(soldadoAgachado);
    scene->addItem(soldadoMuriendo);

    // Ajustar escala y posición inicial respecto al tamaño lógico del soldado
    if (nivelActual && nivelActual->getJugador()) {
        Personaje *p = nivelActual->getJugador();
        float w = p->getAncho();
        float h = p->getAlto();
        float x = p->getX();
        float y = p->getY();

        soldadoQuieto->ajustarATamanoLogico(w, h);
        soldadoCorriendo->ajustarATamanoLogico(w, h);
        soldadoSaltando->ajustarATamanoLogico(w, h);
        soldadoAgachado->ajustarATamanoLogico(w, h);
        soldadoMuriendo->ajustarATamanoLogico(w, h);

        soldadoQuieto->setPos(x, y);
        soldadoCorriendo->setPos(x, y);
        soldadoSaltando->setPos(x, y);
        soldadoAgachado->setPos(x, y);
        soldadoMuriendo->setPos(x, y);
    }

    ocultarYPararSpritesSoldado();
    soldadoActual = soldadoQuieto;
    soldadoQuieto->setVisible(true);
    soldadoQuieto->iniciar(150); // ms entre frames

    // Timer principal del juego
    connect(timer, SIGNAL(timeout()), this, SLOT(actualizarJuego()));
    timer->start(16); // ~60 FPS

    setFocusPolicy(Qt::StrongFocus);
    setFocus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ----------------------------------------------
// Auxiliares de sprites
// ----------------------------------------------

void MainWindow::ocultarYPararSpritesSoldado()
{
    animaciones* sprites[] = {
        soldadoQuieto, soldadoCorriendo, soldadoSaltando,
        soldadoAgachado, soldadoMuriendo
    };

    for (animaciones *s : sprites) {
        if (!s) continue;
        s->detener();
        s->setVisible(false);
    }
}

void MainWindow::actualizarSpriteSoldado()
{
    if (!nivelActual)
        return;

    Personaje *p = nivelActual->getJugador();
    Soldado *sold = dynamic_cast<Soldado*>(p);
    if (!sold || !soldadoQuieto)
        return;

    float x = sold->getX();
    float y = sold->getY();

    // Mover todos los sprites a la misma posición lógica
    animaciones* sprites[] = {
        soldadoQuieto, soldadoCorriendo, soldadoSaltando,
        soldadoAgachado, soldadoMuriendo
    };
    for (animaciones *s : sprites) {
        if (!s) continue;
        s->setPos(x, y);
    }

    // Elegir animación según el estado lógico del soldado
    if (!sold->estaVivo()) {
        if (soldadoActual != soldadoMuriendo) {
            ocultarYPararSpritesSoldado();
            soldadoActual = soldadoMuriendo;
            soldadoMuriendo->setVisible(true);
            soldadoMuriendo->iniciar(120);
        }
    } else if (!sold->estaEnSuelo()) {
        if (soldadoActual != soldadoSaltando) {
            ocultarYPararSpritesSoldado();
            soldadoActual = soldadoSaltando;
            soldadoSaltando->setVisible(true);
            soldadoSaltando->iniciar(90);
        }
    } else if (sold->estaAgachado()) {
        if (soldadoActual != soldadoAgachado) {
            ocultarYPararSpritesSoldado();
            soldadoActual = soldadoAgachado;
            soldadoAgachado->setVisible(true);
            soldadoAgachado->iniciar(120);
        }
    } else {
        // En el suelo y de pie: decidir entre correr o quieto
        float dx = x - xJugadorAnterior;
        if (std::fabs(dx) > 0.1f) {
            if (soldadoActual != soldadoCorriendo) {
                ocultarYPararSpritesSoldado();
                soldadoActual = soldadoCorriendo;
                soldadoCorriendo->setVisible(true);
                soldadoCorriendo->iniciar(80);
            }
        } else {
            if (soldadoActual != soldadoQuieto) {
                ocultarYPararSpritesSoldado();
                soldadoActual = soldadoQuieto;
                soldadoQuieto->setVisible(true);
                soldadoQuieto->iniciar(150);
            }
        }
    }

    xJugadorAnterior = x;
}

void MainWindow::sincronizarObstaculosNivel1()
{
    if (ctrl.getNumeroNivelActual() != 1)
        return;

    Nivel1 *n1 = dynamic_cast<Nivel1*>(nivelActual);
    if (!n1)
        return;

    // Borrar rectángulos anteriores
    for (auto *r : rectObstaculos) {
        scene->removeItem(r);
        delete r;
    }
    rectObstaculos.clear();

    // Crear rectángulos nuevos
    const auto &obs = n1->getObstaculos();
    for (const auto &o : obs) {
        if (!o.estaActivo())
            continue;

        QGraphicsRectItem *ro = scene->addRect(
            0, 0,
            o.getAncho(),
            o.getAlto(),
            QPen(Qt::red),
            QBrush(Qt::red)
            );
        ro->setPos(o.getX(), o.getY());
        rectObstaculos.push_back(ro);
    }
}

// ----------------------------------------------
// Bucle de juego
// ----------------------------------------------

void MainWindow::actualizarJuego()
{
    if (!nivelActual)
        return;

    const float dt = 0.016f;
    ctrl.actualizar(dt);

    nivelActual = ctrl.getNivelActual();
    if (!nivelActual)
        return;

    // Actualizar rectángulo lógico del jugador
    Personaje *p = nivelActual->getJugador();
    if (p && rectJugador) {
        rectJugador->setPos(p->getX(), p->getY());
        rectJugador->setRect(0, 0, p->getAncho(), p->getAlto());
    }

    // Obstáculos (rectángulos rojos) del nivel 1
    sincronizarObstaculosNivel1();

    // Sprite del soldado
    actualizarSpriteSoldado();
}

// ----------------------------------------------
// Teclado → hooks de Nivel
// ----------------------------------------------

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
