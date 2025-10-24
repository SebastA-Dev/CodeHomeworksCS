#include <iostream>
#include "./Listas.h"

using namespace std;

template <typename T>
void imprimirLista(Lista<T>& lista) {
    for (int i = 0; i < lista.obtenerTam(); ++i) {
        cout << lista[i] << " ";
    }
    cout << endl;
}

int main() {
    // Nuevas listas
    // Lista<int> listaEnteros;
    // Lista<float> listaReales;
    // Lista<char> listaCaracteres;

    // // Insertar 3 al final
    // for (int i = 1; i <= 3; ++i) listaEnteros.insertarFinal(i);
    // for (int i = 1; i <= 3; ++i) listaReales.insertarFinal(i * 1.1f);
    // for (int i = 1; i <= 3; ++i) listaCaracteres.insertarFinal('a' + i - 1);

    // // Insertar 3 al inicio
    // for (int i = 10; i >= 8; --i) listaEnteros.insertarInicio(i);
    // for (int i = 10; i >= 8; --i) listaReales.insertarInicio(i * 1.1f);
    // for (int i = 10; i >= 8; --i) listaCaracteres.insertarInicio('A' + i - 8);

    // // Insertar 4 en posiciones intermedias
    // listaEnteros.insertarPosicion(100, 2);
    // listaEnteros.insertarPosicion(101, 3);
    // listaEnteros.insertarPosicion(102, 4);
    // listaEnteros.insertarPosicion(103, 5);

    // listaReales.insertarPosicion(100.1f, 2);
    // listaReales.insertarPosicion(101.1f, 3);
    // listaReales.insertarPosicion(102.1f, 4);
    // listaReales.insertarPosicion(103.1f, 5);

    // listaCaracteres.insertarPosicion('X', 2);
    // listaCaracteres.insertarPosicion('Y', 3);
    // listaCaracteres.insertarPosicion('Z', 4);
    // listaCaracteres.insertarPosicion('W', 5);

    // // Obtener informaci�n
    // int posBuscada = 4;
    // cout << "Enteros, posici�n " << posBuscada << ": " << listaEnteros.obtenerInfo(posBuscada) << endl;
    // cout << "Reales, posici�n " << posBuscada << ": " << listaReales.obtenerInfo(posBuscada) << endl;
    // cout << "Caracteres, posici�n " << posBuscada << ": " << listaCaracteres.obtenerInfo(posBuscada) << endl;

    // // Eliminar la posici�n buscada
    // listaEnteros.eliminarPosicion(posBuscada);
    // listaReales.eliminarPosicion(posBuscada);
    // listaCaracteres.eliminarPosicion(50); // No hace nada (fuera de rango)

    // // Buscar e imprimir la misma posici�n
    // cout << "Enteros tras borrado, posici�n " << posBuscada << ": " << listaEnteros.obtenerInfo(posBuscada) << endl;
    // cout << "Reales tras borrado, posici�n " << posBuscada << ": " << listaReales.obtenerInfo(posBuscada) << endl;
    // cout << "Caracteres tras borrado, posici�n " << posBuscada << ": " << listaCaracteres.obtenerInfo(posBuscada) << endl;

    // // Modificar posici�n
    // listaEnteros[posBuscada] = 999;
    // listaReales[posBuscada] = 999.9f;
    // listaCaracteres[posBuscada] = '*';

    // // Verificar modificaci�n
    // cout << "Enteros tras modificaci�n, posici�n " << posBuscada << ": " << listaEnteros.obtenerInfo(posBuscada) << endl;
    // cout << "Reales tras modificaci�n, posici�n " << posBuscada << ": " << listaReales.obtenerInfo(posBuscada) << endl;
    // cout << "Caracteres tras modificaci�n, posici�n " << posBuscada << ": " << listaCaracteres.obtenerInfo(posBuscada) << endl;

    // // Imprimir listas completas
    // cout << "Lista de enteros: ";
    // imprimirLista(listaEnteros);

    // cout << "Lista de reales: ";
    // imprimirLista(listaReales);

    // cout << "Lista de caracteres: ";
    // imprimirLista(listaCaracteres);

    Lista<Lista<int>> listaDeListas;

    // Crea algunas listas de enteros
    Lista<int> lista1;
    lista1.insertarFinal(1);
    lista1.insertarFinal(2);
    lista1.insertarFinal(3);

    Lista<int> lista2;
    lista2.insertarFinal(10);
    lista2.insertarFinal(20);
    lista2.insertarFinal(30);

    // Inserta esas listas dentro de la lista principal
    listaDeListas.insertarFinal(lista1);
    listaDeListas.insertarFinal(lista2);

    // Accede e imprime los elementos
    for (int i = 0; i < listaDeListas.obtenerTam(); ++i) {
        cout << "Sublista " << i + 1 << ": ";
        Lista<int> sublista = listaDeListas.obtenerInfo(i);
        for (int j = 0; j < sublista.obtenerTam(); ++j) {
            cout << sublista[j] << " ";
        }
        cout << endl;
    }

    listaDeListas[0][0] = 2222222;

    // Accede e imprime los elementos
    for (int i = 0; i < listaDeListas.obtenerTam(); ++i) {
        cout << "Sublista " << i + 1 << ": ";
        Lista<int> sublista = listaDeListas.obtenerInfo(i);
        for (int j = 0; j < sublista.obtenerTam(); ++j) {
            cout << sublista[j] << " ";
        }
        cout << endl;
    }

    return 0;
}

