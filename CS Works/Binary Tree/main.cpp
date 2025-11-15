#include <iostream>

#include "Tree.h"

using namespace std;

int main(int argc, char** argv) {

    // =====================================================
    // PRUEBAS CRUDAS DE Lista<int>
    // =====================================================
    cout << "===== PRUEBA Lista<int> =====" << endl;

    Lista<int> lista;

    cout << "Lista vacia? " << (lista.estaVacia() ? "SI" : "NO") << endl;

    // Insertar algunos elementos
    lista.insertarFinal(10);
    lista.insertarFinal(20);
    lista.insertarFinal(30);

    cout << "Tamano de la lista: " << lista.obtenerTam() << endl;

    cout << "Elementos de la lista por operador[]:" << endl;
    for (int i = 0; i < lista.obtenerTam(); ++i) {
        cout << "  lista[" << i << "] = " << lista[i] << endl;
    }

    // Insertar al inicio
    lista.insertarInicio(5);
    cout << "Despues de insertarInicio(5):" << endl;
    for (int i = 0; i < lista.obtenerTam(); ++i) {
        cout << "  lista[" << i << "] = " << lista[i] << endl;
    }

    // Eliminar posicion 1 (deberia eliminar el 10)
    lista.eliminarPosicion(1);
    cout << "Despues de eliminarPosicion(1):" << endl;
    for (int i = 0; i < lista.obtenerTam(); ++i) {
        cout << "  lista[" << i << "] = " << lista[i] << endl;
    }

    // =====================================================
    // PRUEBAS CRUDAS DE Tree<int> SOBRE Lista<int>
    // =====================================================
    cout << endl << "===== PRUEBA Tree<int> =====" << endl;

    Tree<int> arbol;

    cout << "Arbol vacio? " << (arbol.estaVacio() ? "SI" : "NO") << endl;

    // Insertar raiz
    arbol.insertarRaiz(100);
    cout << "Insertada raiz 100" << endl;
    cout << "Arbol vacio? " << (arbol.estaVacio() ? "SI" : "NO") << endl;
    cout << "Cantidad de nodos: " << arbol.obtenerCantidad() << endl;
    cout << "Raiz: " << arbol.raiz() << endl;

    // Insertar hijos
    arbol.insertarHijoIzquierdo(1, 50);   // indice 2
    arbol.insertarHijoDerecho(1, 150);    // indice 3
    cout << "Insertados hijos 50 (izq) y 150 (der) de la raiz" << endl;
    cout << "Cantidad de nodos: " << arbol.obtenerCantidad() << endl;

    if (arbol.tieneHijoIzquierdo(1)) {
        cout << "Hijo izquierdo de 1: " << arbol.hijoIzquierdo(1) << endl;
    }
    if (arbol.tieneHijoDerecho(1)) {
        cout << "Hijo derecho de 1: " << arbol.hijoDerecho(1) << endl;
    }

    // Insertar un nivel mas
    arbol.insertarHijoIzquierdo(2, 25);   // indice 4
    arbol.insertarHijoDerecho(2, 75);     // indice 5

    cout << "Insercion de nietos (25 y 75) bajo el nodo 2" << endl;
    cout << "Cantidad de nodos: " << arbol.obtenerCantidad() << endl;

    // Acceso tipo arreglo (recuerda: indice 0 es espacio de control)
    cout << "Acceso tipo arreglo en arbol:" << endl;
    cout << "  arbol[1] (raiz)       = " << arbol[1] << endl;
    cout << "  arbol[2] (hijo izq)   = " << arbol[2] << endl;
    cout << "  arbol[3] (hijo der)   = " << arbol[3] << endl;
    cout << "  arbol[4] (nieto izq)  = " << arbol[4] << endl;
    cout << "  arbol[5] (nieto der)  = " << arbol[5] << endl;

    // Comprobar algunas propiedades
    cout << "Es hoja indice 4? " << (arbol.esHoja(4) ? "SI" : "NO") << endl;
    cout << "Es hoja indice 1? " << (arbol.esHoja(1) ? "SI" : "NO") << endl;

    cout << "Altura aproximada del arbol: " << arbol.altura() << endl;

    // Probar eliminarSubarbol
    cout << "Eliminando subarbol con raiz en indice 2..." << endl;
    arbol.eliminarSubarbol(2);
    cout << "Cantidad de nodos despues de eliminar subarbol(2): "
         << arbol.obtenerCantidad() << endl;

    // Verificar que el hijo izq de la raiz ya no existe
    cout << "Tiene hijo izquierdo 1? "
         << (arbol.tieneHijoIzquierdo(1) ? "SI" : "NO") << endl;
    cout << "Tiene hijo derecho 1? "
         << (arbol.tieneHijoDerecho(1) ? "SI" : "NO") << endl;

    // Limpiar todo el arbol
    cout << "Llamando arbol.limpiar()..." << endl;
    arbol.limpiar();
    cout << "Arbol vacio? " << (arbol.estaVacio() ? "SI" : "NO") << endl;
    cout << "Cantidad de nodos despues de limpiar: "
         << arbol.obtenerCantidad() << endl;

    cout << endl << "===== FIN DE PRUEBA =====" << endl;

    return 0;
}

