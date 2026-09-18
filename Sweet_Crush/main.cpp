#include <iostream>

#include "tablero.h"
#include "juego.h"

using namespace std;

int main(int argc, char *argv[]){

    unsigned char *tablero = nullptr;
    int filas, columnas, cantBytes;

    //Datos de avance del juego
    int eliminacionesUsuario = 0;
    int fichasEliminadas = 0;
    int combinacionesTotales = 0;
    int cascadasMov = 0;
    int puntuacion = 0;

    cout << "_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-\n";
    cout << "_-_-_-_-_ Sweet Crush -_-_-_-_-_\n";
    cout << "_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-\n\n";

    do{
        cout << "Ingrese el numero de filas para el tablero de juego inicial (minimo 1): ";
        cin >> filas;
        cout << "Ingrese el numero de columnas (minimo 1): ";
        cin >> columnas;
    }while (filas < 1 || columnas < 1);

    //Memoria dinamica para el tablero y rellenarlo
    crearTablero(tablero, filas, columnas, cantBytes);
    rellenarTableroA(tablero, filas, columnas);

    int opcion;
    do{
        cout << "\n_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-\n";
        cout << "_-_-_-_-_ Tablero SC -_-_-_-_-_-\n";
        cout << "_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-\n";

        imprimirTablero(tablero, filas, columnas);
        cout << "Filas: " << filas << endl;
        cout << "Columnas: " << columnas << endl;
        cout << "Bytes reservados: " << cantBytes << endl;
        cout << "Fichas eliminadas manualmente: " << eliminacionesUsuario << endl;
        cout << "Fichas totales eliminadas: " << fichasEliminadas << endl;
        cout << "Combinaciones: " << combinacionesTotales << endl;
        cout << "Cascadas del ultimo movimiento: " << cascadasMov << endl;
        cout << "Puntuacion: " << puntuacion << endl;

        //Menu de juego
        cout << "\n_-_-_-_ Opciones de juego _-_-_-_\n";
        cout << " 1. Eliminar ficha\n";
        cout << " 2. Agregar fila\n";
        cout << " 3. Eliminar fila\n";
        cout << " 4. Agregar columna\n";
        cout << " 5. Eliminar columna\n";
        cout << " 6. Mostrar bits\n";
        cout << " 7. Salir del juego\n";
        cout << "\nIngrese una opcion: ";
        cin >> opcion;

        int fila, columna;

        switch (opcion) {
        case 1://Eliminar ficha

            do{
                cout << "Ingrese la fila (1-" << filas << "): ";
                cin >> fila;
                cout << "Ingrese la columna (1-" << columnas << "): ";
                cin >> columna;

                if(fila > 0 && fila <= filas && columna > 0 && columna <= columnas){
                    cascadasMov = 0;
                    usuarioEliminarFicha(tablero, filas, columnas, fila - 1, columna - 1, eliminacionesUsuario,
                                         fichasEliminadas, combinacionesTotales, cascadasMov, puntuacion);
                }
                else{
                    cout << "Posicion invalida. Intente nuevamente:" << endl;
                }
            } while(fila <= 0 || fila > filas || columna <= 0 || columna > columnas);

            break;

        case 2://Agregar fila

            do{
                cout << "Posicion de la nueva fila (1-" << filas + 1 << "): ";
                cin >> fila;

                if(fila > 0 && fila <= filas + 1){
                    agregarFila(tablero, filas, columnas, cantBytes, fila - 1);
                    cascadasMov = cascadas(tablero, filas, columnas, 1, combinacionesTotales, fichasEliminadas, puntuacion);
                }
                else{
                    cout << "Posicion invalida. Intente nuevamente." << endl;
                }
            }while(fila <= 0 || fila > filas + 1);

            break;

        case 3:{//Eliminar fila

            int limFilas = filas;

            do{
                cout << "Posicion de la fila a eliminar (1-" << filas << "): ";
                cin >> fila;

                if(fila > 0 && fila <= filas){
                    eliminarFila(tablero, filas, columnas, cantBytes, fila - 1);
                    cascadasMov = cascadas(tablero, filas, columnas, 0, combinacionesTotales, fichasEliminadas, puntuacion);
                }
                else{
                    cout << "Posicion invalida. Intente nuevamente." << endl;
                }
            }while(fila <= 0 || fila > limFilas);

            break;
        }

        case 4://Agregar columna

            do{
                cout << "Posicion de la nueva columna (1-" << columnas + 1 << "): ";
                cin >> columna;

                if(columna > 0 && columna <= columnas + 1){
                    agregarColumna(tablero, filas, columnas, cantBytes, columna - 1);
                    cascadasMov = cascadas(tablero, filas, columnas, 1, combinacionesTotales, fichasEliminadas, puntuacion);
                }
                else{
                    cout << "Posicion invalida. Intente nuevamente." << endl;
                }
            }while(columna <= 0 || columna > columnas + 1);

            break;

        case 5:{//Eliminar columna

            int limColumnas = columnas;
            do{
                cout << "Posicion de la columna a eliminar (1-" << columnas << "): ";
                cin >> columna;

                if(columna > 0 && columna <= columnas){
                    eliminarColumna(tablero, filas, columnas, cantBytes, columna - 1);
                    cascadasMov = cascadas(tablero, filas, columnas, 0, combinacionesTotales, fichasEliminadas, puntuacion);
                }
                else{
                    cout << "Posicion invalida. Intente nuevamente." << endl;
                }
            }while(columna <= 0 || columna > limColumnas);

            break;
        }

        case 6://Imprimir bits

            imprimirBits(tablero, filas, columnas);
            break;

        case 7://Salir del juego
            cout << "¡Buen juego! Hasta pronto.";
            break;

        default:
            cout << "Opcion invalida. Intente nuevamente." ;
            break;
        }
    } while (opcion != 7);

    delete [] tablero;
    tablero = nullptr;

    return 0;
}