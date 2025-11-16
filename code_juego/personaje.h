#ifndef PERSONAJE_H
#define PERSONAJE_H
class Personaje {
protected:
    float x, y;//posicion
    float vx, vy;//velocidad
    float ancho, alto;
    int vida;
public:
    Personaje(float px, float py, float w, float h, int vidaInicial);
    virtual ~Personaje() = default;//destructpr para la herencia
    void mover(float dx, float dy);
    void recibirDanio(int d);
    bool estaVivo() const { return vida > 0; }
    float getX() const { return x; }
    float getY() const { return y; }
    float getAncho() const { return ancho; }
    float getAlto()  const { return alto; }
    virtual void actualizar(float dt) = 0;
};
#endif
