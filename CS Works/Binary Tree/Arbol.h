#ifndef ARBOL_H
#define ARBOL_H

#include "NodoArbol.h"
#include "Lista.h"
#include "Cola.h"
#include "Pila.h"
#include <fstream>
#include <sstream>
#include <iostream>

template <class T>
class Arbol {

private:
    Lista<NodoArbol<T>> nodos;      // todos los nodos del árbol
    int cantidad;                   // Cantidad de nodos
    int capacidad;                  // Capacidad máxima
    int proximoIndice;              // max indice disponible
    std::string archivoArbol;       // Nombre archivo
    std::string archivoInfoPattern; 

    // Métodos auxiliares 
    
    int _buscarIndice(const T& clave, int indiceActual) const;
    int _encontrarLugarInsercion(const T& clave, int& indicePadre);
    void _preordenRec(int indice, Cola<std::pair<T, int>>& resultado) const;
    void _inordenRec(int indice, Cola<std::pair<T, int>>& resultado) const;
    void _postordenRec(int indice, Cola<std::pair<T, int>>& resultado) const;
    int _contarHijos(int indice) const;
    int _buscarSucesor(int indice) const;

public:
    // Constructor y Destructor
    
    
    Arbol(int capacidadMax, const std::string& nomArchivoArbol);
    ~Arbol();

    // Consultas básicas
    bool estaVacio() const;
    int obtenerCantidad() const;
    bool estaLleno() const;

    // Inserción
    bool insertar(const T& clave, int idInfo);

    // Búsqueda
    bool buscar(const T& clave, int& idInfo) const;
    int buscarId(const T& clave) const;

    // Modificación
    bool modificar(const T& clave, int nuevoIdInfo);

    // Eliminación
    bool eliminar(const T& clave);

    // Recorridos iterativos

    Cola<std::pair<T, int>> recorridoInorden() const;
    Cola<std::pair<T, int>> recorridoPreorden() const;
    Cola<std::pair<T, int>> recorridoPostorden() const;
    Cola<std::pair<T, int>> recorridoPorNiveles() const;
    
	// Persistencia
    bool guardarEnArchivo();
    bool cargarDesdeArchivo();

    // eliminar datos
    void vaciar();
};

// ==================== IMPLEMENTACIONES ====================

template <class T>
Arbol<T>::Arbol(int capacidadMax, const std::string& nomArchivoArbol)
    : cantidad(0), capacidad(capacidadMax), proximoIndice(0), 
      archivoArbol(nomArchivoArbol), archivoInfoPattern("info_") {
    
    // Pre-llenar la lista con capacidad nodos vacíos
    for (int i = 0; i < capacidadMax; ++i) {
        nodos.insertarFinal(NodoArbol<T>());
    }
}

template <class T>
Arbol<T>::~Arbol() {
    guardarEnArchivo();
}

template <class T>
bool Arbol<T>::estaVacio() const {
    return cantidad == 0;
}

template <class T>
int Arbol<T>::obtenerCantidad() const {
    return cantidad;
}

template <class T>
bool Arbol<T>::estaLleno() const {
    return cantidad >= capacidad;
}

// Búsqueda exitencia de una clave
template <class T>
int Arbol<T>::_buscarIndice(const T& clave, int indiceActual) const {
    if (indiceActual < 0 || indiceActual >= nodos.obtenerTam()) {
        return -1;
    }

    const NodoArbol<T>& nodo = nodos.obtenerInfo(indiceActual);
    
    // Si está marcado como eliminado, no contar como encontrado
    if (nodo.eliminado) {
        return -1;
    }

    if (nodo.clave == clave) {
        return indiceActual;
    } else if (clave < nodo.clave) {
        return _buscarIndice(clave, nodo.hijoIzquierdo);
    } else {
        return _buscarIndice(clave, nodo.hijoDerecho);
    }
}

// retornar índice padre y llenar indicePadre

template <class T>
int Arbol<T>::_encontrarLugarInsercion(const T& clave, int& indicePadre) {
    if (cantidad == 0) {
        indicePadre = -1;
        return -1; // Es la raíz
    }

    int indiceActual = 0;
    indicePadre = -1;

    while (indiceActual >= 0 && indiceActual < nodos.obtenerTam()) {
        const NodoArbol<T>& nodo = nodos.obtenerInfo(indiceActual);

        if (nodo.clave == clave) {
            return -2; // Es un duplicado
        }

        indicePadre = indiceActual;

        if (clave < nodo.clave) {
            indiceActual = nodo.hijoIzquierdo;
        } else {
            indiceActual = nodo.hijoDerecho;
        }
    }

    return indiceActual; // vacío
}

// Insertar un nuevo nodo
template <class T>
bool Arbol<T>::insertar(const T& clave, int idInfo) {
    if (estaLleno()) {
        return false;
    }

    // Buscar si la clave ya existe
    
    if (!estaVacio() && _buscarIndice(clave, 0) >= 0) {
        return false; 
    }

    if (estaVacio()) {
    	
        // Insertar como raíz
        NodoArbol<T> nuevoNodo(clave, idInfo);
        nodos[0] = nuevoNodo;
        cantidad = 1;
        proximoIndice = 1;
        return true;
    }

    // Encontrar lugar
    int indicePadre = -1;
    int resultado = _encontrarLugarInsercion(clave, indicePadre);

    if (resultado == -2) {
        return false; // Es un duplicado
    }

    if (proximoIndice >= capacidad) {
        return false; // capacidad excedida
    }

    NodoArbol<T> nuevoNodo(clave, idInfo);
    int nuevoIndice = proximoIndice++;

    nodos[nuevoIndice] = nuevoNodo;

    // Vincular al padre
    
    if (indicePadre >= 0) {
        NodoArbol<T>& padre = nodos[indicePadre];
        if (clave < padre.clave) {
            padre.hijoIzquierdo = nuevoIndice;
        } else {
            padre.hijoDerecho = nuevoIndice;
        }
    }

    cantidad++;
    return true;
}

// Busqueda de la clave

template <class T>
bool Arbol<T>::buscar(const T& clave, int& idInfo) const{
    int indice = _buscarIndice(clave, 0);
    
    if (indice < 0) {
        return false;
    }

    idInfo = nodos.obtenerInfo(indice).idInfo;
    return true;
}

template <class T>
int Arbol<T>::buscarId(const T& clave) const {
    int indice = _buscarIndice(clave, 0);
    
    if (indice < 0) {
        return -1;
    }

    return nodos.obtenerInfo(indice).idInfo;
}

// Modificacar información
template <class T>
bool Arbol<T>::modificar(const T& clave, int nuevoIdInfo) {
    int indice = _buscarIndice(clave, 0);
    
    if (indice < 0) {
        return false;
    }

    nodos[indice].idInfo = nuevoIdInfo;
    return true;
}

// Contar hijos 

template <class T>
int Arbol<T>::_contarHijos(int indice) const {
    if (indice < 0 || indice >= nodos.obtenerTam()) {
        return 0;
    }

    const NodoArbol<T>& nodo = nodos.obtenerInfo(indice);
    int hijos = 0;

    if (nodo.hijoIzquierdo >= 0) hijos++;
    if (nodo.hijoDerecho >= 0) hijos++;

    return hijos;
}

// Buscar sucesor 

template <class T>
int Arbol<T>::_buscarSucesor(int indice) const {
    if (indice < 0 || indice >= nodos.obtenerTam()) {
        return -1;
    }

    const NodoArbol<T>& nodo = nodos.obtenerInfo(indice);
    
    if (nodo.hijoDerecho < 0) {
        return -1;
    }

    int actual = nodo.hijoDerecho;
    while (actual >= 0 && actual < nodos.obtenerTam()) {
        const NodoArbol<T>& nodoActual = nodos.obtenerInfo(actual);
        if (nodoActual.hijoIzquierdo < 0) {
            return actual;
        }
        actual = nodoActual.hijoIzquierdo;
    }

    return actual;
}

// Eliminar nodo

template <class T>
bool Arbol<T>::eliminar(const T& clave) {
    int indice = _buscarIndice(clave, 0);
    
    if (indice < 0) {
        return false;
    }

    NodoArbol<T>& nodo = nodos[indice];
    int hijos = _contarHijos(indice);

    std::cout << "\n=== ELIMINACION ===" << std::endl;
    std::cout << "Clave: " << clave << " | ID: " << nodo.idInfo << std::endl;

    if (hijos == 0) {
        // hoja
        std::cout << "Tipo: Hoja" << std::endl;
        nodo.eliminado = true;
        cantidad--;

    } else if (hijos == 1) {
        // hijo
        std::cout << "Tipo: Un hijo" << std::endl;
        int hijoIndice = (nodo.hijoIzquierdo >= 0) ? nodo.hijoIzquierdo : nodo.hijoDerecho;

        // Copiar información del hijo al nuevo nodo
        
        NodoArbol<T>& hijo = nodos[hijoIndice];
        nodo.clave = hijo.clave;
        nodo.idInfo = hijo.idInfo;
        nodo.hijoIzquierdo = hijo.hijoIzquierdo;
        nodo.hijoDerecho = hijo.hijoDerecho;

        // eliminar el hijo
        hijo.eliminado = true;
        cantidad--;

    } else {
        // si tiene dos hijos
        std::cout << "Tipo: Dos hijos" << std::endl;
        int sucesorIndice = _buscarSucesor(indice);

        if (sucesorIndice >= 0) {
            NodoArbol<T>& sucesor = nodos[sucesorIndice];

            // Copiar datos del sucesor al nuevo nodo
            nodo.clave = sucesor.clave;
            nodo.idInfo = sucesor.idInfo;

            // Eliminar el sucesor
            sucesor.eliminado = true;
            cantidad--;
        }
    }

    return true;
}

// Preorden

template <class T>
void Arbol<T>::_preordenRec(int indice, Cola<std::pair<T, int>>& resultado) const {
    if (indice < 0 || indice >= nodos.obtenerTam()) {
        return;
    }

    const NodoArbol<T>& nodo = nodos.obtenerInfo(indice);
    
    if (!nodo.eliminado) {
        resultado.encolar(std::make_pair(nodo.clave, nodo.idInfo));
    }

    if (nodo.hijoIzquierdo >= 0) {
        _preordenRec(nodo.hijoIzquierdo, resultado);
    }
    if (nodo.hijoDerecho >= 0) {
        _preordenRec(nodo.hijoDerecho, resultado);
    }
}

// Inorden
template <class T>
void Arbol<T>::_inordenRec(int indice, Cola<std::pair<T, int>>& resultado) const {
    if (indice < 0 || indice >= nodos.obtenerTam()) {
        return;
    }

    const NodoArbol<T>& nodo = nodos.obtenerInfo(indice);

    if (nodo.hijoIzquierdo >= 0) {
        _inordenRec(nodo.hijoIzquierdo, resultado);
    }

    if (!nodo.eliminado) {
        resultado.encolar(std::make_pair(nodo.clave, nodo.idInfo));
    }

    if (nodo.hijoDerecho >= 0) {
        _inordenRec(nodo.hijoDerecho, resultado);
    }
}

// Postorden 
template <class T>
void Arbol<T>::_postordenRec(int indice, Cola<std::pair<T, int>>& resultado) const {
    if (indice < 0 || indice >= nodos.obtenerTam()) {
        return;
    }

    const NodoArbol<T>& nodo = nodos.obtenerInfo(indice);

    if (nodo.hijoIzquierdo >= 0) {
        _postordenRec(nodo.hijoIzquierdo, resultado);
    }
    if (nodo.hijoDerecho >= 0) {
        _postordenRec(nodo.hijoDerecho, resultado);
    }

    if (!nodo.eliminado) {
        resultado.encolar(std::make_pair(nodo.clave, nodo.idInfo));
    }
}

// Recorridos
template <class T>
Cola<std::pair<T, int>> Arbol<T>::recorridoInorden() const {
    Cola<std::pair<T, int>> resultado;
    _inordenRec(0, resultado);
    return resultado;
}

template <class T>
Cola<std::pair<T, int>> Arbol<T>::recorridoPreorden() const{
    Cola<std::pair<T, int>> resultado;
    _preordenRec(0, resultado);
    return resultado;
}

template <class T>
Cola<std::pair<T, int>> Arbol<T>::recorridoPostorden() const{
    Cola<std::pair<T, int>> resultado;
    _postordenRec(0, resultado);
    return resultado;
}

template <class T>
Cola<std::pair<T, int>> Arbol<T>::recorridoPorNiveles() const{
    Cola<std::pair<T, int>> resultado;
    Cola<int> colaIndices;

    if (!estaVacio()) {
        colaIndices.encolar(0);

        while (!colaIndices.estaVacia()) {
            int indiceActual = colaIndices.desencolar();

            if (indiceActual < 0 || indiceActual >= nodos.obtenerTam()) {
                continue;
            }

            const NodoArbol<T>& nodo = nodos.obtenerInfo(indiceActual);

            if (!nodo.eliminado) {
                resultado.encolar(std::make_pair(nodo.clave, nodo.idInfo));
            }

            if (nodo.hijoIzquierdo >= 0) {
                colaIndices.encolar(nodo.hijoIzquierdo);
            }
            if (nodo.hijoDerecho >= 0) {
                colaIndices.encolar(nodo.hijoDerecho);
            }
        }
    }

    return resultado;
}

// Guardar datos del arbol

template <class T>
bool Arbol<T>::guardarEnArchivo() {
    std::ofstream archivo(archivoArbol, std::ios::binary);
    if (!archivo.is_open()) {
        return false;
    }
    
    archivo.write(reinterpret_cast<char*>(&cantidad), sizeof(int));
    archivo.write(reinterpret_cast<char*>(&proximoIndice), sizeof(int));

    // Guardar cada nodo
    
    for (int i = 0; i < proximoIndice && i < nodos.obtenerTam(); ++i) {
        const NodoArbol<T>& nodo = nodos.obtenerInfo(i);
        
        // Escribir clave 

        archivo.write(reinterpret_cast<const char*>(&nodo.clave), sizeof(T));
        archivo.write(reinterpret_cast<const char*>(&nodo.idInfo), sizeof(int));
        archivo.write(reinterpret_cast<const char*>(&nodo.hijoIzquierdo), sizeof(int));
        archivo.write(reinterpret_cast<const char*>(&nodo.hijoDerecho), sizeof(int));
        archivo.write(reinterpret_cast<const char*>(&nodo.eliminado), sizeof(bool));
    }

    archivo.close();
    return true;
}

// Cargar datos de un arbol

template <class T>
bool Arbol<T>::cargarDesdeArchivo() {
    std::ifstream archivo(archivoArbol, std::ios::binary);
    if (!archivo.is_open()) {
        return false;
    }

    int cantidadCargada = 0;
    int proximoIndiceCargado = 0;

    archivo.read(reinterpret_cast<char*>(&cantidadCargada), sizeof(int));
    archivo.read(reinterpret_cast<char*>(&proximoIndiceCargado), sizeof(int));

    for (int i = 0; i < proximoIndiceCargado && i < capacidad; ++i) {
        NodoArbol<T> nodo;

        archivo.read(reinterpret_cast<char*>(&nodo.clave), sizeof(T));
        archivo.read(reinterpret_cast<char*>(&nodo.idInfo), sizeof(int));
        archivo.read(reinterpret_cast<char*>(&nodo.hijoIzquierdo), sizeof(int));
        archivo.read(reinterpret_cast<char*>(&nodo.hijoDerecho), sizeof(int));
        archivo.read(reinterpret_cast<char*>(&nodo.eliminado), sizeof(bool));

        nodos[i] = nodo;
    }

    cantidad = cantidadCargada;
    proximoIndice = proximoIndiceCargado;

    archivo.close();
    return true;
}

// eliminar datos

template <class T>
void Arbol<T>::vaciar() {
    cantidad = 0;
    proximoIndice = 0;

    for (int i = 0; i < nodos.obtenerTam(); ++i) {
        nodos[i] = NodoArbol<T>();
    }
}

#endif
