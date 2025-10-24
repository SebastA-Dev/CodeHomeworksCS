#include <iostream>
#include "Listas.h"

using namespace std;

template <typename T>
void imprimir_lista(Lista<T>& lista) {
    for (int i = 0; i < lista.getTam(); ++i) {
        cout << lista[i] << " ";
    }
    cout << endl;
}

int main() {
    
	// nuevas listas
	
    Lista<int> listaEnteros;
    Lista<float> listaReales;
    Lista<char> listaCaracteres;

    
	// Insertar 3 al final
	
    for (int i = 1; i <= 3; ++i) listaEnteros.insertar_final(i);
    for (int i = 1; i <= 3; ++i) listaReales.insertar_final(i * 1.1f);
    for (int i = 1; i <= 3; ++i) listaCaracteres.insertar_final('a' + i - 1);

    
	// Insertar 3 al inicio
	
    for (int i = 10; i >= 8; --i) listaEnteros.insertar_inicio(i);
    for (int i = 10; i >= 8; --i) listaReales.insertar_inicio(i * 1.1f);
    for (int i = 10; i >= 8; --i) listaCaracteres.insertar_inicio('A' + i - 8);

    
	// Insertar 4 en intermedias
    
    listaEnteros.insertar_pos(100, 2);
    listaEnteros.insertar_pos(101, 3);
    listaEnteros.insertar_pos(102, 4);
    listaEnteros.insertar_pos(103, 5);

    listaReales.insertar_pos(100.1f, 2);
    listaReales.insertar_pos(101.1f, 3);
    listaReales.insertar_pos(102.1f, 4);
    listaReales.insertar_pos(103.1f, 5);

    listaCaracteres.insertar_pos('X', 2);
    listaCaracteres.insertar_pos('Y', 3);
    listaCaracteres.insertar_pos('Z', 4);
    listaCaracteres.insertar_pos('W', 5);

    // imprimir
    
	int posBuscada = 4;
    cout << "Enteros, posición " << posBuscada << ": " << listaEnteros.obtener_info(posBuscada) << endl;
    cout << "Reales, posición " << posBuscada << ": " << listaReales.obtener_info(posBuscada) << endl;
    cout << "Caracteres, posición " << posBuscada << ": " << listaCaracteres.obtener_info(posBuscada) << endl;

    // Eliminar la posicion buscada
    
    
    listaEnteros.eliminar(posBuscada);
    listaReales.eliminar(posBuscada);
    listaCaracteres.eliminar(50); 


    // Buscar e imprimir la misma posicion
    
    cout << "Enteros tras borrado, posición " << posBuscada << ": " << listaEnteros.obtener_info(posBuscada) << endl;
    cout << "Reales tras borrado, posición " << posBuscada << ": " << listaReales.obtener_info(posBuscada) << endl;
    cout << "Caracteres tras borrado, posición " << posBuscada << ": " << listaCaracteres.obtener_info(posBuscada) << endl;


    // nueva psoicion
    
    listaEnteros[posBuscada] = 999;
    listaReales[posBuscada] = 999.9f;
    listaCaracteres[posBuscada] = '*';

    // Verificar modificación
    cout << "Enteros tras modificación, posición " << posBuscada << ": " << listaEnteros.obtener_info(posBuscada) << endl;
    cout << "Reales tras modificación, posición " << posBuscada << ": " << listaReales.obtener_info(posBuscada) << endl;
    cout << "Caracteres tras modificación, posición " << posBuscada << ": " << listaCaracteres.obtener_info(posBuscada) << endl;

    // Imprimir
    cout << "Lista de enteros: ";
    imprimir_lista(listaEnteros);

    cout << "Lista de reales: ";
    imprimir_lista(listaReales);

    cout << "Lista de caracteres: ";
    imprimir_lista(listaCaracteres);

    return 0;
}

