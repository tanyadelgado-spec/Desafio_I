#include <ctime>

#include "juego.h"
#include "tablero.h"

juego::juego() {}

//Generador de numeros aleatorios (1, 6) -Generador Congruencial Lineal (GCL)-
static unsigned int semilla = static_cast<unsigned int>(time(nullptr));
static int numAleatorio (int limite){
    semilla = semilla * 1103515245 + 12345;
    int aleatorio = (semilla >> 16) % limite;
    return aleatorio;
}

//Comprobación de que una ficha genera combinaciones
static bool formaCombinacion(const unsigned char *tablero, int filas, int columnas, int fila, int columna){
    unsigned int ficha = obtenerBitsFicha(tablero, fila * columnas + columna);

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
    return (cant >= 3);
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
        int destino = filas - 1;

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

//Eliminar las fichas marcadas
int eliminarFichaMarcada (unsigned char *tablero, int filas, int columnas, const unsigned char *marcas){

    int fichasEliminadas = 0;
    int posiciones = filas * columnas;

    for (int i = 0; i < posiciones; ++i) {
        if (marcada(marcas, i)){
            guardarFicha(tablero, i, fichaVacia);
            fichasEliminadas++;
        }
    }
    return fichasEliminadas;
}

//Metodo cascadas
int cascadas(unsigned char *tablero, int filas, int columnas, int vacios,
             int &combinacionesTotales, int &fichasEliminadas, int &puntuacion){

    int cascadas = 0;
    int posiciones = filas * columnas;
    int bytesMarcas= (posiciones + 7) / 8;

    unsigned char *marcas = new unsigned char [bytesMarcas];
    int combinaciones;

    do{
        //En caso de eliminaciones o nuevos espacios, aplicamos desplazamiento vertical y rellenar
        if (vacios){
            desplazamientoVertical(tablero, filas, columnas);
            rellenarVacios(tablero, filas, columnas);
            vacios = 0;
        }
        combinaciones = identificarCombinaciones(tablero, filas, columnas, marcas);

        if(combinaciones > 0){

            int eliminadas = eliminarFichaMarcada(tablero, filas, columnas, marcas);

            cascadas++;
            combinacionesTotales += combinaciones;
            fichasEliminadas += eliminadas;

            //Puntuaciones. 5pts por ficha eliminada por el numero de cascadas; 20pts adicionales por cada combinación
            puntuacion += eliminadas * 5 * cascadas;
            puntuacion += combinaciones * 20;

            vacios = 1;
        }
    }while (combinaciones > 0);

    delete [] marcas;
    return cascadas;
}

//Eliminacion de ficha por el usuario
void usuarioEliminarFicha(unsigned char *tablero, int filas, int columnas, int fila, int columna, int &eliminacionUsuario,
                          int &fichasEliminadas, int &combinacionesTotales, int &cascadasMov, int &puntuacion){

    int indice = fila * columnas + columna;
    unsigned int ficha = obtenerBitsFicha(tablero, indice);

    //Eliminacion solicitada por consola
    guardarFicha(tablero, indice, fichaVacia);
    eliminacionUsuario++;
    fichasEliminadas++;

    //Puntuacion. 2pts adicionales por eliminacion especifica de ficha
    puntuacion +=2;

    //Posibles cascadas por eliminacion
    cascadasMov = cascadas(tablero, filas, columnas, 1, combinacionesTotales, fichasEliminadas, puntuacion);
}

//Copia del tablero
static void copiaTablero(unsigned char *destino, const unsigned char *origen, int filas, int columnas){
    int total = filas * columnas;
    for (int i = 0; i < total; ++i) {
        unsigned int ficha = obtenerBitsFicha(origen, i);
        guardarFicha(destino, i, ficha);
    }
}

//Añadir fila
void agregarFila(unsigned char *&tablero, int &filas, int columnas, int &cantBytes, int fila){

    int actFilas = filas + 1;
    int actBytes = bytesUtilizados(actFilas, columnas);

    unsigned char *nueva = new unsigned char[actBytes];

    for (int i = 0; i < actBytes; ++i) {
        nueva [i] = 0;
    }
    for (int f = 0; f < actFilas; ++f) {
        for (int c = 0; c < columnas; ++c) {

            int actIndice = f * columnas + c;
            if(f == fila){
                guardarFicha(nueva, actIndice, fichaVacia);
            }
            else{
                int fAnterior = (f < fila) ? f : f - 1;
                int indiceAnterior = fAnterior * columnas + c;

                unsigned int ficha = obtenerBitsFicha(tablero, indiceAnterior);
                guardarFicha(nueva, actIndice, ficha);
            }
        }
    }
    delete [] tablero;
    tablero = nueva;
    filas = actFilas;
    cantBytes = actBytes;

    limpiarBitsInvalidos(tablero, filas, columnas, cantBytes);
}

//Eliminar fila
void eliminarFila(unsigned char *&tablero, int &filas, int columnas, int &cantBytes, int fila){

    //Minimo una fila
    if(filas <= 1){
        return;
    }

    int actFilas = filas - 1;
    int actBytes = bytesUtilizados(actFilas, columnas);

    //Utilizacion respecto a memoria actual
    double utilizacion = (3.0 * actFilas * columnas) / (8.0 * cantBytes);
    if(utilizacion < 0.65){
        //Reasignación de la memoria
        unsigned char *nuevo = new unsigned char[actBytes];

        for (int i = 0; i < actBytes; ++i) {
            nuevo[i] = 0;
        }
        for (int f = 0; f < actFilas; ++f) {
            int fAnterior = (f < fila) ? f : f + 1;
            for (int c = 0; c < columnas; ++c) {
                unsigned int ficha = obtenerBitsFicha(tablero, fAnterior * columnas + c);
                guardarFicha(nuevo, f * columnas + c, ficha);
            }
        }
        delete [] tablero;
        tablero = nuevo;
        cantBytes = actBytes;
    }
    else{

        //Se conserva el bloque fisico
        for (int f = 0; f < actFilas; ++f) {
            int fAnterior = (f < fila) ? f : f + 1;
            for (int c = 0; c < columnas; ++c) {
                unsigned int ficha = obtenerBitsFicha(tablero, fAnterior * columnas + c);
                guardarFicha(tablero, f * columnas + c, ficha);
            }
        }
    }
    filas = actFilas;
    limpiarBitsInvalidos(tablero, filas, columnas, cantBytes);
}

//Añadir columna
void agregarColumna(unsigned char *&tablero, int filas, int &columnas, int &cantBytes, int columna){

    int actColumnas = columnas + 1;
    int actBytes = bytesUtilizados(filas, actColumnas);
    unsigned char *nuevo = new unsigned char[actBytes];

    for (int i = 0; i < actBytes; ++i) {
        nuevo[i] = 0;
    }
    for (int f = 0; f < filas; ++f) {
        for (int c = 0; c < actColumnas; ++c) {

            int actIndice = f * actColumnas + c;
            if(c == columnas){
                guardarFicha(nuevo, actIndice, fichaVacia);
            }
            else{
                int cAnterior = (c < columna) ? c : c - 1;
                int indiceAnterior = f * columnas + cAnterior;

                unsigned int ficha = obtenerBitsFicha(tablero, indiceAnterior);
                guardarFicha(nuevo, actIndice, ficha);
            }
        }
    }
    delete [] tablero,
    tablero = nuevo;
    columnas = actColumnas;
    cantBytes = actBytes;

    limpiarBitsInvalidos(tablero, filas, columnas, cantBytes);
}

//Eliminar columna
void eliminarColumna(unsigned char *&tablero, int filas, int &columnas, int &cantBytes, int columna){

    //Minimo una columna
    if(columnas <= 1){
        return;
    }

    int actColumnas = columnas - 1;
    int actBytes = bytesUtilizados(filas, actColumnas);

    double utilizacion = (3.0 * filas * actColumnas) / (8.0 * cantBytes);
    if(utilizacion < 0.65){
        unsigned char *nuevo = new unsigned char[actBytes];

        for (int i = 0; i < actBytes; ++i) {
            nuevo[i] = 0;
        }
        for (int f = 0; f < filas; ++f) {
            for (int c = 0; c < actColumnas; ++c) {
                int cAnterior = (c < columna) ? c : c + 1;

                unsigned int ficha = obtenerBitsFicha(tablero, f * columnas + cAnterior);
                guardarFicha(nuevo, f * actColumnas + c, ficha);
            }
        }
        delete [] tablero;
        tablero = nuevo;
        cantBytes = actBytes;
    }
    else{
        //Se conserva memoria actual
        for (int f = 0; f < filas; ++f) {
            for (int c = 0; c < actColumnas; ++c) {
                int cAnterior = (c < columna) ? c : c + 1;

                unsigned int ficha = obtenerBitsFicha(tablero, f * columnas + cAnterior);
                guardarFicha(tablero, f * actColumnas + c, ficha);
            }
        }
    }
    columnas = actColumnas;
    limpiarBitsInvalidos(tablero, filas, columnas, cantBytes);
}
