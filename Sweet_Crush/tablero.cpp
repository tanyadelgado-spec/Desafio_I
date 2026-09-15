#include "tablero.h"
#include <iostream>

using namespace std;

tablero::tablero() {}


//Calcular los bytes a utilizar dadas las dimensiones del tablero
int bytesUtilizados(int filas, int columnas){

    int posiciones = filas * columnas;
    int bits = posiciones * 3; //3 bits por ficha
    int bytes = (bits + 7) / 8; //Redondeo hacia arriba por si el # de bits genera division no exacta entre 8 (bits+(n-1))/n

    return bytes;

}

//Generar tablero de juego
void crearTablero(unsigned char *&tablero, int filas, int columnas, int &cantBytes){

    cantBytes = bytesUtilizados(filas, columnas);
    tablero = new unsigned char[cantBytes];

    //Inicializar los bytes en 0
    for (int i = 0; i < cantBytes; ++i) {
        tablero[i] = 0;
    }

    int totalPos = filas * columnas;

    //Inicializar cada posicion con la ficha vacia (6 = 110)
    for (int i = 0; i < totalPos; ++i) {
        guardarFicha(tablero, i, fichaVacia);
    }
}

//Obtener 3bits de cada ficha, 'const' para leer el tablero pero no modificarlo
unsigned int obtenerBitsFicha(const unsigned char *tablero, int indice){
    int bitInicial = indice * 3;
    int byte = bitInicial / 8;
    int offset = bitInicial % 8;

    unsigned int valor;

    if(offset <= 5){
        valor = (tablero[byte] >> offset) & 7; //ficha7=111, haciendo la suma de los bits de la ficha con los de la 7, nos quedan solo los de la ficha

    }
    else{
        //Caso en que la ficha se divide en 2 bits
        int primerosBits = 8 - offset;
        int segundosBits = 3 - primerosBits;

        unsigned int part1 = (tablero[byte] >> offset) & ((1 << primerosBits) - 1);
        unsigned int part2 = tablero[byte+1] & ((1 << segundosBits) - 1);

        //Unir ambas partes de bits
        valor = part1 | (part2 << primerosBits);
    }

    return valor & 7; //Asegurar que solo se devuelvan 3bits
}

void guardarFicha(unsigned char *tablero, int indice, unsigned int ficha){

    ficha = ficha & 7;

    int bitInicial = indice * 3;
    int byte = bitInicial / 8;
    int offset = bitInicial % 8;

    if(offset <= 5){

        //Crear mascara de 3bits y la desplazamos hasta la posición correspondiente
        unsigned char mascara = static_cast<unsigned char>(7 << offset);
        //Borrar los tres bits anteriores
        tablero[byte] = tablero[byte] & static_cast<unsigned char>(~mascara);
        //Colocamos la nueva ficha
        tablero[byte] = tablero[byte] | static_cast<unsigned char>(ficha << offset);
    }
    else{

        int primerosBits = 8 - offset;
        int segundosBits = 3 - primerosBits;

        //Mascara para cada grupo de bits de la ficha -en cada byte-
        unsigned int mascara1 = (1 << primerosBits) - 1;
        unsigned int mascara2 = (1 << segundosBits) - 1;

        //Para el primer byte

            //Borramos los bits ya almacenados
        tablero[byte] = tablero[byte] & static_cast<unsigned char>(~(mascara1 << offset));
            //Extraemos la parte de la ficha que corresponde al 1er byte y la colocamos en su posición
        tablero[byte] = tablero[byte] | static_cast<unsigned char>((ficha & mascara1) << offset);

        //Segundo byte

            //Borramos los bits ya almacenados
        tablero[byte+1] = tablero[byte+1] & static_cast<unsigned char>(~mascara2);
            //Extraemos la parte restante de la ficha y la colocamos en su posición
        tablero[byte+1] = tablero[byte+1] | static_cast<unsigned char>((ficha >> primerosBits) & mascara2);
    }
}

//Eliminar los bits que no pertenecen al tablero -invalidos-
void limpiarBitsInvalidos(unsigned char *tablero, int filas, int columnas, int cantBytes){

    //Cant de bits del tablero
    int bitsValidos = filas * columnas * 3;
    //Cant bits validos en ultimo byte
    int resto = bitsValidos % 8;

    if (resto != 0){

        unsigned char mascara = static_cast<unsigned char>((1 << resto) -1);
        tablero[cantBytes - 1] = tablero[cantBytes - 1] & mascara;
    }
    int bytesValidos = bytesUtilizados(filas, columnas);
    for (int i = bytesValidos; i < cantBytes; ++i) {
        tablero[i] = 0;
    }
}

//Convertir los codigos de las fichas
char codigoFicha(unsigned int ficha){

    switch (ficha) {
    case ficha1:
        return 'T';

    case ficha2:
        return '#';

    case ficha3:
        return '%';

    case ficha4:
        return 'K';

    case ficha5:
        return '@';

    case ficha6:
        return '=';

    case fichaEspecial:
        return '*';

    default: //fichaVacia
        return '-';
    }
}

//Imprimir tablero
void imprimirTablero(const unsigned char *tablero, int filas, int columnas){

    cout << "\n";

    for (int i = 0; i < filas; ++i) {
        for (int j = 0; j < columnas; ++j) {

            //Indice logico -fila y columna-
            int indice = i * columnas + j;

            //Obtener bits de la ficha y convertirla en simbolo
            unsigned int ficha = obtenerBitsFicha(tablero, indice);
            char simbolo = codigoFicha(ficha);

            cout << simbolo << "  ";

        }
        cout << "\n";
    }
    cout << "\n";
}

//Imprimir representación binaria del tablero
void imprimirBits(const unsigned char *tablero, int filas, int columnas){

    int bitsValidos = filas * columnas * 3;

    //Representacion de bits
    cout << "\nRepresentacion compacta (fichas):\n";

    for (int bit = 0; bit < bitsValidos; ++bit) {
        //Determinar el byte en que esta ese bit y su posicion
        int byte = bit / 8;
        int pos = bit % 8;

        unsigned int valor = (tablero[byte] >> pos) & 1;
        cout << valor;

        if ((bit + 1) % 3 == 0){
            cout << " ";
        }
    }
    cout << "\n";

    //Imprimir bytes
    int bytes = bytesUtilizados(filas, columnas);
    cout << "\nBytes utilizados:\n";

    for (int i = 0; i < bytes; ++i) {
        cout << "Byte " << i << ": ";

        //Imprimir cada byte desde el bit 7 al 0
        for (int b = 7; b >= 0; --b) {
            unsigned int valor = (tablero[i] >> b) & 1;
            cout << valor;
        }
        cout << "\n";
    }
    cout << "\n";
}
