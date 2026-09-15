#ifndef JUEGO_H
#define JUEGO_H

class juego
{
public:
    juego();
};


void rellenarTableroA(unsigned char *tablero, int filas,int columnas);

void desplazamientoVertical(unsigned char *tablero, int filas, int columnas);

void rellenarVacios(unsigned char *tablero, int filas, int columnas);

int identificarCombinaciones(const unsigned char *tablero, int filas, int columnas, unsigned char *marcas);

#endif
