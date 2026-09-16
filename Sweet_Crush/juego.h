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

int eliminarFichaMarcada (unsigned char *tablero, int filas, int columnas, const unsigned char *marcas);

int cascadas(unsigned char *tablero, int filas, int columnas, int vacios,
             int &combinacionesTotales, int &fichasEliminadas, int &puntuacion);

void usuarioEliminarFicha(unsigned char *tablero, int filas, int columnas, int fila, int columna, int &eliminacionUsuario,
                          int &fichasEliminadas, int &combinacionesTotales, int &cascadasMov, int &puntuacion);

void agregarFila(unsigned char *&tablero, int &filas, int columnas, int &cantBytes, int fila);

void eliminarFila(unsigned char *&tablero, int &filas, int columnas, int &cantBytes, int fila);

void agregarColumna(unsigned char *&tablero, int filas, int &columnas, int &cantBytes, int columna);

void eliminarColumna(unsigned char *&tablero, int filas, int &columnas, int &cantBytes, int columna);

#endif
