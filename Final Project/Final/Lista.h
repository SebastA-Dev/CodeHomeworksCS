#ifndef LISTA_H
#define LISTA_H

#include "UtilidadesAnalisis.h"

#include <string>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <type_traits>
#include <iostream>

// ==========================
//     Nodo<T>
// ==========================
template<class T>
struct Nodo {
    std::string id;   // Se guarda siempre en HEX
    T info;
    Nodo<T>* siguiente;
};


// ==========================
//      Clase Lista<T>
// ==========================
template <class T>
class Lista {

private:
    Nodo<T>* cabeza;
    int tam;

    bool _validarPosicion(int pos);
    Nodo<T>* _obtenerNodoPos(int pos);

public:
    // Constructores / destructores
    Lista();
    ~Lista();
    Lista(const Lista<T>& otra);
    Lista<T>& operator=(const Lista<T>& otra);

    // Inserciones
    void insertarInicio(T dato);
    void insertarFinal(T dato);
    void insertarPosicion(T dato, int pos);
    void insertarOrden(T dato, bool asc = true);

    // Busquedas
    bool seEncuentra(T dato);

    // Eliminaciones
    bool eliminarPosicion(int pos);

    template<class CampoOrden, class Extractor>
    bool eliminarElemento(CampoOrden dato, Extractor extraerCampo);

    // Acceso
    T obtenerInfo(int pos);
    bool estaVacia();
    int obtenerTam();
    T& operator[](int pos);
    std::string obtenerIdNodo(int pos);
};

template <class T>
Lista<T>::Lista() : cabeza(nullptr), tam(0) {}

template <class T>
Lista<T>::~Lista() {
    Nodo<T>* actual = cabeza;
    while (actual != nullptr) {
        Nodo<T>* temp = actual;
        actual = actual->siguiente;
        delete temp;
    }
    cabeza = nullptr;
    tam = 0;
}

template <class T>
Lista<T>::Lista(const Lista<T>& otra) : cabeza(nullptr), tam(0) {
    Nodo<T>* actual = otra.cabeza;
    while (actual != nullptr) {
        this->insertarFinal(actual->info);
        actual = actual->siguiente;
    }
}

template <class T>
Lista<T>& Lista<T>::operator=(const Lista<T>& otra) {
    if (this == &otra) return *this;

    Nodo<T>* actual = cabeza;
    while (actual != nullptr) {
        Nodo<T>* temp = actual;
        actual = actual->siguiente;
        delete temp;
    }
    cabeza = nullptr;
    tam = 0;

    actual = otra.cabeza;
    while (actual != nullptr) {
        this->insertarFinal(actual->info);
        actual = actual->siguiente;
    }

    return *this;
}

// ===================== INSERCIONES ======================

template<class T>
void Lista<T>::insertarInicio(T dato) {
    Nodo<T>* nuevo = new Nodo<T>;
    nuevo->info = dato;
    nuevo->siguiente = cabeza;
    nuevo->id = dato -> codigo; //Corrección, al usar el método SeEncuentra retorna falso. Pues los códigos nunca serán iguales por lo que retornaba falso.
    cabeza = nuevo;
    tam++;
}

template<class T>
void Lista<T>::insertarFinal(T dato) {
    Nodo<T>* nuevo = new Nodo<T>;
    nuevo->info = dato;
    nuevo->siguiente = nullptr;
    nuevo->id = dato -> codigo; //Corrección, al usar el método SeEncuentra retorna falso. Pues los códigos nunca serán iguales por lo que retornaba falso.

    if (cabeza == nullptr) {
        cabeza = nuevo;
        tam++;
        return;
    }

    Nodo<T>* aux = _obtenerNodoPos(tam - 1);
    aux->siguiente = nuevo;
    tam++;
}

template<class T>
void Lista<T>::insertarPosicion(T dato, int pos) {
    if (!_validarPosicion(pos))
        return;

    if (pos == 0) {
        insertarInicio(dato);
        return;
    }

    Nodo<T>* nuevo = new Nodo<T>;
    nuevo->info = dato;
    nuevo->id = "";

    Nodo<T>* anterior = _obtenerNodoPos(pos - 1);
    nuevo->siguiente = anterior->siguiente;
    anterior->siguiente = nuevo;
    tam++;
}

template<class T>
void Lista<T>::insertarOrden(T dato, bool asc) {

    // Crear nodo
    Nodo<T>* nuevo = new Nodo<T>;
    nuevo->info = dato;
    nuevo->siguiente = nullptr;
    nuevo->id = dato->codigo;

    // ====== INSERTAR EN LISTA VACÃA ======
    if (cabeza == nullptr) {
        cabeza = nuevo;
        tam++;
        return;
    }

    // ====== INSERTAR AL PRINCIPIO ======
    // Comparar hex strings directamente (lexicographically)
    bool debeIrAlPrincipio = false;
    if (asc) {
        debeIrAlPrincipio = (dato->codigo < cabeza->id);
    } else {
        debeIrAlPrincipio = (dato->codigo > cabeza->id);
    }

    if (debeIrAlPrincipio) {
        nuevo->siguiente = cabeza;
        cabeza = nuevo;
        tam++;
        return;
    }

    // ====== RECORRER E INSERTAR EN LA POSICIÃ“N CORRECTA ======
    Nodo<T>* actual = cabeza;

    while (actual->siguiente != nullptr) {
        bool debeInsertar = false;
        if (asc) {
            debeInsertar = (dato->codigo < actual->siguiente->id);
        } else {
            debeInsertar = (dato->codigo > actual->siguiente->id);
        }

        if (debeInsertar) break;

        actual = actual->siguiente;
    }

    nuevo->siguiente = actual->siguiente;
    actual->siguiente = nuevo;
    tam++;
}


// ===================== ELIMINACIONES ======================

template<class T>
bool Lista<T>::eliminarPosicion(int pos) {
    if (!_validarPosicion(pos))
        return false;

    if (pos == 0) {
        Nodo<T>* aux = cabeza;
        cabeza = cabeza->siguiente;
        delete aux;
        tam--;
        return true;
    }

    Nodo<T>* nodoEliminar = _obtenerNodoPos(pos);
    Nodo<T>* anterior = _obtenerNodoPos(pos - 1);

    anterior->siguiente = nodoEliminar->siguiente;
    delete nodoEliminar;
    tam--;
    return true;
}

template<class T>
template<class CampoOrden, class Extractor>
bool Lista<T>::eliminarElemento(CampoOrden valorBuscado, Extractor extraerCampo) {
    Nodo<T>* actual = cabeza;
    Nodo<T>* anterior = nullptr;

    while (actual != nullptr) {
        CampoOrden valorActual = extraerCampo(actual->info);

        if (valorActual == valorBuscado) {
            if (anterior == nullptr)
                cabeza = actual->siguiente;
            else
                anterior->siguiente = actual->siguiente;

            delete actual;
            tam--;
            return true;
        }

        anterior = actual;
        actual = actual->siguiente;
    }
    return false;
}

// ===================== BUSQUEDA ======================

// Busca si el elemento actual esta dentro de la lista
template<class T>
bool Lista<T>::seEncuentra(T dato){
    if (!dato) return false;
    
    Nodo<T>* actual = cabeza;
    while(actual != nullptr){  // Se corrige error, no recorria hasta el último nodo
        if(actual->id == dato->codigo)
            return true;
        actual = actual->siguiente; // Avanza al siguiente nodo
    }
    return false;
}

// ===================== ACCESO ======================

template<class T>
T Lista<T>::obtenerInfo(int pos) {
    if (!_validarPosicion(pos))
        throw std::out_of_range("Posiciï¿½n invï¿½lida");

    return _obtenerNodoPos(pos)->info;
}

template<class T>
T& Lista<T>::operator[](int pos) {
    if (!_validarPosicion(pos))
        throw std::out_of_range("Posiciï¿½n invï¿½lida");

    return _obtenerNodoPos(pos)->info;
}

template<class T>
bool Lista<T>::estaVacia() {
    return tam == 0;
}

template<class T>
int Lista<T>::obtenerTam() {
    return tam;
}

template<class T>
bool Lista<T>::_validarPosicion(int pos) {
    return pos >= 0 && pos < tam;
}

template<class T>
Nodo<T>* Lista<T>::_obtenerNodoPos(int pos) {
    Nodo<T>* aux = cabeza;
    int contador = 0;

    while (contador < pos && aux != nullptr) {
        aux = aux->siguiente;
        contador++;
    }

    return aux;
}

template<class T>
std::string Lista<T>::obtenerIdNodo(int pos) {
    Nodo<T>* nodo = _obtenerNodoPos(pos);
    if (!nodo) return "";
    return nodo->id;
}

#endif
