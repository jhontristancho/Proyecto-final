#ifndef CONTROLADOR_JUEGO_H
#define CONTROLADOR_JUEGO_H

class controlador_juego
{
private:
    float anchoScene = 1200.0f;
    float altoScene  = 800.0f;
public:
 controlador_juego();
    float getAnchoScene() const { return anchoScene; }
    float getAltoScene()  const { return altoScene; }
    void setTamanoScene(float ancho, float alto) {//por si se llega a cambiar el ancho o el alto
        anchoScene = ancho;
        altoScene  = alto;
    }

};

#endif // CONTROLADOR_JUEGO_H
