#include "juego.h"
#include "tablero.h"

juego::juego() {}

//Generador de numeros aleatorios (1, 6) -Generador Congruencial Lineal (GCL)-
static unsigned int semilla = 123456789;
static int numAleatorio (int limite){
    semilla = semilla * 1103515245 + 12345;
    int aleatorio = (semilla >> 16) % limite + 1;
    return aleatorio;
}

//Comprobación de que una ficha genera combinaciones
static bool formaCombinacion(const unsigned char *tablero, int filas, int columnas, int fila, int columna){
    unsigned int ficha = obtenerBitsFicha(tablero, fila * columnas + columna);

    if (ficha > ficha6){
        return false;
    }

    int cant = 1;
    //Izquierda
    int c = columna - 1;
    while (c >= 0 && obtenerBitsFicha(tablero, fila * columnas + c) == ficha) {
        cant++;
        c--;
    }

    //Derecha
    c = columna + 1;
    while (c < columnas && obtenerBitsFicha(tablero, fila * columnas + c) == ficha) {
        cant++;
        c++;
    }

    if (cant >= 3){
        return true;
    }

    cant = 1;
    //Arriba
    int a = fila - 1;
    while (a >= 0 && obtenerBitsFicha(tablero, a * columnas + columna) == ficha) {
        cant++;
        a--;
    }

    //Abajo
    a = fila + 1;
    while (a < filas && obtenerBitsFicha(tablero, a * columnas + columna) == ficha) {
        cant++;
        a++;
    }
    return cant >= 3;
}

//Rellenar tablero alatoriamente
void rellenarTableroA(unsigned char *tablero, int filas,int columnas){

    for (int i = 0; i < filas; ++i) {
        for (int j = 0; j < columnas; ++j) {
            int indice = i * columnas +j;
            unsigned char ficha;

            do{
                ficha = numAleatorio(6);
                guardarFicha(tablero, indice, ficha);
            } while (formaCombinacion(tablero, filas, columnas, i, j));
        }
    }
}

//Desplazamiento vertical -gravedad-
void desplazamientoVertical(unsigned char *tablero, int filas, int columnas){
    for (int columna = 0; columna < columnas; ++columna) {

        int destino = filas -1;

        for (int fila = filas - 1; fila >= 0; --fila) {

            unsigned int ficha = obtenerBitsFicha(tablero, fila * columnas + columna);

            if(ficha != fichaVacia){
                if(destino != fila){
                    guardarFicha(tablero, destino * columnas + columna, ficha);
                    guardarFicha(tablero, fila * columnas + columna, fichaVacia);
                }
                destino--;
            }
        }

        while (destino >= 0) {
            guardarFicha(tablero, destino *columnas + columna, fichaVacia);
            destino--;
        }
    }
}

//Rellenar espacios vacios
void rellenarVacios(unsigned char *tablero, int filas, int columnas){

    for (int fila = 0; fila < filas; ++fila) {
        for (int columna = 0; columna < columnas; ++columna) {
            int indice = fila * columnas + columna;

            if(obtenerBitsFicha(tablero, indice) == fichaVacia){
                unsigned int ficha;
                int intentos = 0;

                do{
                    ficha = numAleatorio(6);
                    guardarFicha(tablero, indice, ficha);
                    intentos++;
                } while(formaCombinacion(tablero, filas, columnas, fila, columna) && intentos < 30);
            }
        }
    }
}

//Operaciones para marcas en las fichas
static void limpiarMarcas(unsigned char *marcas, int bytes){

    for (int i = 0; i < bytes; ++i) {
        marcas[i] = 0;
    }
}

static void marcar(unsigned char *marcas, int indice){

    int byte = indice / 8;
    int bit = indice % 8;

    marcas[byte] = marcas[byte] | static_cast<unsigned char>(1 << bit);
}

static bool marcada(const unsigned char *marcas, int indice){
    int byte = indice / 8;
    int bit = indice % 8;

    return (marcas[byte] & (1 << bit)) != 0;
}

//Identificar combinaciones
int identificarCombinaciones(const unsigned char *tablero, int filas, int columnas, unsigned char *marcas){

    int pos = filas * columnas;
    int bytesMarcas = (pos + 7) / 8;
    limpiarMarcas(marcas, bytesMarcas);

    int combinaciones = 0;
    //Horizontales
    for (int fila = 0; fila < filas; ++fila) {

        int columna = 0;
        while (columna < columnas) {
            unsigned int ficha = obtenerBitsFicha(tablero, fila * columnas + columna);

            if(ficha > ficha6){
                columna++;
                continue;
            }

            int inicio = columna;
            int cant = 0;
            while (columna < columnas && obtenerBitsFicha(tablero, fila * columnas + columna) == ficha) {
                cant++;
                columna++;
            }

            if(cant >= 3){
                combinaciones++;

                for (int c = inicio; c < inicio + cant; ++c) {
                    marcar(marcas, fila * columnas + c);
                }
            }
        }
    }

    //Verticales
    for (int columna = 0; columna < columnas; ++columna) {

        int fila = 0;
        while (fila < filas) {
            unsigned int ficha = obtenerBitsFicha(tablero, fila * columnas + columna);

            if(ficha > ficha6){
                fila++;
                continue;
            }

            int inicio = fila;
            int cant = 0;
            while (fila < filas && obtenerBitsFicha(tablero, fila * columnas + columna) == ficha) {
                cant++;
                fila++;

            }

            if(cant >=3){
                combinaciones++;

                for (int a = inicio; a < inicio + cant; ++a) {
                    marcar(marcas, a * columnas + columna);
                }
            }
        }
    }
    return combinaciones;
}