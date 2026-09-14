#ifndef TABLERO_H
#define TABLERO_H

class tablero
{
public:
    tablero();
};


const unsigned int ficha1 = 0;
const unsigned int ficha2 = 1;
const unsigned int ficha3 = 2;
const unsigned int ficha4 = 3;
const unsigned int ficha5 = 4;
const unsigned int ficha6 = 5;
const unsigned int fichaVacia = 6;
const unsigned int fichaEspecial = 7;

int bytesUtilizados(int filas, int columnas);

void crearTablero(unsigned char *&tablero, int filas, int columnas, int &cantBytes);

unsigned int obtenerBitsFicha(const unsigned char *tablero, int indice);

void guardarFicha(unsigned char *tablero, int indice, unsigned int ficha);

#endif
