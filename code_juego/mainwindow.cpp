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

constexpr int COLS_OBS_PEQ  = 3; // disparo_peque.png (3 frames)
constexpr int COLS_OBS_MED  = 4; // disparo_mediano.png (4 frames)
constexpr int COLS_OBS_GRAN = 3; // disparo_grande.png (3 frames)

// Rutas (asegúrate que coincidan con tu .qrc)
const char* RUTA_OBS_PEQ  = ":/nivel1/obstaculos/disparo_peque.png";
const char* RUTA_OBS_MED  = ":/nivel1/obstaculos/disparo_mediano.png";
const char* RUTA_OBS_GRAN = ":/nivel1/obstaculos/disparo_grande.png";
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
    , fondoItem(nullptr)
    , anchoFondo(ANCHO_MUNDO)

{
    ui->setupUi(this);
    ui->graphicsView->setGeometry(0, 0, ANCHO_MUNDO, ALTO_MUNDO);
    setFixedSize(static_cast<int>(ANCHO_MUNDO),
                 static_cast<int>(ALTO_MUNDO));
    // Escena y vista
    scene->setSceneRect(0, 0, ANCHO_MUNDO, ALTO_MUNDO);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setFixedSize(static_cast<int>(ANCHO_MUNDO),
                                   static_cast<int>(ALTO_MUNDO));
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QPixmap fondo(":/nivel1/soldado/fondo_nivel1.png");  // misma ruta
    if (!fondo.isNull()) {
        // Hacemos un fondo 3 veces más ancho que la ventana para tener recorrido.
        // Si quieres más o menos "camino", cambia el 3.0f.
        const float factorLargo = 3.0f;

        QPixmap fondoEscalado = fondo.scaled(
            static_cast<int>(ANCHO_MUNDO * factorLargo),
            static_cast<int>(ALTO_MUNDO),
            Qt::IgnoreAspectRatio,
            Qt::SmoothTransformation
            );

        anchoFondo = static_cast<float>(fondoEscalado.width());

        fondoItem = scene->addPixmap(fondoEscalado);
        fondoItem->setZValue(-100);
        fondoItem->setPos(0, 0);
    }

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
    // --- HUD: tiempo y vidas ---
    textoTiempo = scene->addText("Tiempo: 00:00");
    textoTiempo->setDefaultTextColor(Qt::white);
    textoTiempo->setZValue(1000);
    textoTiempo->setPos(10, 10);   // esquina superior izquierda

    textoVida = scene->addText("Vida: 0");
    textoVida->setDefaultTextColor(Qt::white);
    textoVida->setZValue(1000);
    textoVida->setPos(10, 40);
    // --- Damage flash (pantalla roja breve al recibir daño) ---
    damageFlash = scene->addRect(
        0, 0,
        ANCHO_MUNDO,
        ALTO_MUNDO,
        QPen(Qt::NoPen),
        QBrush(QColor(255, 0, 0, 100)) // rojo con algo de transparencia
        );
    damageFlash->setZValue(2000);   // por encima de todo
    damageFlash->setVisible(false);


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
void MainWindow::actualizarFondo(float xJugador)
{
    if (!fondoItem)
        return;

    // Qué tan rápido se mueve el fondo relativo al jugador (parallax)
    const float factorParallax = 0.2f;

    // Cuánto puede desplazarse el fondo como máximo sin mostrar "fuera de la imagen"
    float maxScroll = anchoFondo - ANCHO_MUNDO;
    if (maxScroll <= 0.0f)
        return;

    // Desplazamiento en bucle (cuando llega al final, vuelve al inicio)
    float desplaz = std::fmod(xJugador * factorParallax, maxScroll);
    if (desplaz < 0.0f)
        desplaz += maxScroll;

    // Posicionamos el fondo de forma que la ventana (0..ANCHO_MUNDO)
    // vea siempre un trozo continuo de la misma imagen
    fondoItem->setPos(-desplaz, 0);
}

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
    // Solo nos interesa si el nivel actual es el 1
    if (ctrl.getNumeroNivelActual() != 1)
        return;

    Nivel1 *n1 = dynamic_cast<Nivel1*>(nivelActual);
    if (!n1)
        return;

    const auto &obs = n1->getObstaculos();

    // 1) Aseguramos que el vector de sprites tenga al menos obs.size() posiciones
    if (spritesObstaculos.size() < obs.size()) {
        spritesObstaculos.resize(obs.size(), nullptr);
    }

    // 2) Para cada obstáculo lógico, aseguramos un sprite animado
    for (size_t i = 0; i < obs.size(); ++i) {
        const Obstaculo &o = obs[i];

        if (!o.estaActivo()) {
            if (spritesObstaculos[i])
                spritesObstaculos[i]->setVisible(false);
            continue;
        }

        // --- Elegir tipo de sprite según tamaño lógico (w, h) ---
        // --- Elegir tipo de sprite según tamaño lógico (w, h) ---
        float w = o.getAncho();
        float h = o.getAlto();

        int tipoDeseado = 0;        // 0 = pequeño, 1 = mediano, 2 = grande
        const char *ruta = nullptr;
        int columnas = 1;

        // <= 52  → sprite pequeño (48x48, los obstáculos altos)
        // <= 80  → sprite mediano (64x64, obstáculos de suelo normales)
        //  > 80  → sprite grande  (96x96, obstáculos grandes)
        if (w <= 52.0f && h <= 52.0f) {
            tipoDeseado = 0;
            ruta        = RUTA_OBS_PEQ;
            columnas    = COLS_OBS_PEQ;
        } else if (w <= 80.0f && h <= 80.0f) {
            tipoDeseado = 1;
            ruta        = RUTA_OBS_MED;
            columnas    = COLS_OBS_MED;
        } else {
            tipoDeseado = 2;
            ruta        = RUTA_OBS_GRAN;
            columnas    = COLS_OBS_GRAN;
        }

        // --- Crear o recrear el sprite si no existe o cambia el tipo ---
        animaciones *sprite = spritesObstaculos[i];

        int tipoActual = -1;
        if (sprite) {
            tipoActual = sprite->data(0).toInt(); // usamos data(0) para guardar el tipo
        }

        if (!sprite || tipoActual != tipoDeseado) {
            // Si ya había uno pero de otro tipo, lo borramos
            if (sprite) {
                scene->removeItem(sprite);
                delete sprite;
            }

            // Creamos un nuevo sprite animado para este obstáculo
            sprite = new animaciones(QString::fromUtf8(ruta), columnas, 1, this);
            scene->addItem(sprite);

            // Guardamos qué tipo es en data(0)
            sprite->setData(0, tipoDeseado);

            // Iniciamos animación (puedes ajustar la velocidad)
            sprite->iniciar(1000); // ms entre frames

            spritesObstaculos[i] = sprite;
        }

        // --- Hacer el sprite más grande que la hitbox lógica ---
        const float factorVisual = 2.0f; // súbelo o bájalo si quieres más grande/pequeño
        sprite->ajustarATamanoLogico(w * factorVisual, h * factorVisual);

        // Posición en el mundo (misma que el obstáculo)
        sprite->setPos(o.getX(), o.getY());
        sprite->setZValue(-5);  // delante del fondo, detrás del jugador
        sprite->setVisible(true);
    }

    // 3) Si hay sprites sobrantes (porque se eliminaron obstáculos),
    //    los borramos para no dejar fugas.
    if (spritesObstaculos.size() > obs.size()) {
        for (size_t i = obs.size(); i < spritesObstaculos.size(); ++i) {
            if (spritesObstaculos[i]) {
                scene->removeItem(spritesObstaculos[i]);
                delete spritesObstaculos[i];
            }
        }
        spritesObstaculos.resize(obs.size());
    }
}

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
        if (p) {
            actualizarFondo(p->getX());
        }
    }

    // Obstáculos (rectángulos rojos) del nivel 1
    sincronizarObstaculosNivel1();

    // Sprite del soldado
    actualizarSpriteSoldado();
    actualizarHUDNivel1();

    // --- Damage flash si hubo daño en este frame ---
    if (ctrl.getNumeroNivelActual() == 1) {
        Nivel1 *n1 = dynamic_cast<Nivel1*>(nivelActual);
        if (n1 && n1->huboDanioReciente()) {
            mostrarDamageFlash();
            n1->resetDanioReciente();
        }
    }
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
QString MainWindow::formatearTiempo(float segundos) const
{
    if (segundos < 0.0f)
        segundos = 0.0f;

    int total = static_cast<int>(segundos + 0.5f);
    int minutos = total / 60;
    int segs    = total % 60;

    return QString("%1:%2")
        .arg(minutos, 2, 10, QLatin1Char('0'))
        .arg(segs,    2, 10, QLatin1Char('0'));
}
void MainWindow::actualizarHUDNivel1()
{
    if (!nivelActual)
        return;

    // Solo si estamos en el Nivel 1
    if (ctrl.getNumeroNivelActual() != 1)
        return;

    Nivel1 *n1 = dynamic_cast<Nivel1*>(nivelActual);
    if (!n1)
        return;

    // Tiempo
    float tRestante = n1->getTiempoRestante();
    if (textoTiempo) {
        textoTiempo->setPlainText("Tiempo: " + formatearTiempo(tRestante));
    }

    // Vida del jugador
    const Soldado &sold = n1->getJugadorConst();
    // Asumo que Soldado/Personaje tiene getVida() o similar
    int vida = sold.getVida();  // Si tu getter se llama distinto, ajústalo

    if (textoVida) {
        textoVida->setPlainText(QString("Vida: %1").arg(vida));
    }
}
void MainWindow::mostrarDamageFlash()
{
    if (!damageFlash)
        return;

    damageFlash->setVisible(true);
    damageFlash->setOpacity(0.5);  // un poco más intenso

    // Lo ocultamos después de unos milisegundos
    QTimer::singleShot(120, this, [this]() {
        if (damageFlash)
            damageFlash->setVisible(false);
    });
}
