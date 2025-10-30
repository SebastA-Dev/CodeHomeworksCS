#ifndef COLA1_H     
#define COLA1_H 

#include <iostream>
#include "estructura1.h"
using namespace std;

template <typename T>
class cola {
    nodoT<T>* cab;
    nodoT<T>* fin;

    // Auxiliar: copia profunda de la cadena de nodos (estructura), preservando orden.
    static void copiar_nodos(const nodoT<T>* src_head, nodoT<T>*& dst_head, nodoT<T>*& dst_tail) {
        dst_head = nullptr;
        dst_tail = nullptr;
        const nodoT<T>* cur = src_head;
        while (cur != nullptr) {
            nodoT<T>* nuevo = new nodoT<T>;
            nuevo->dato = cur->dato;     // copia superficial del dato T (puntero en tu caso)
            nuevo->sig  = nullptr;

            if (dst_head == nullptr) {
                dst_head = nuevo;
                dst_tail = nuevo;
            } else {
                dst_tail->sig = nuevo;
                dst_tail = nuevo;
            }
            cur = cur->sig;
        }
    }

public:
    cola() : cab(nullptr), fin(nullptr) {}
    void InsCola(const T& i);
    T AtenderCola();
    void ImprimirCola();
    bool ColaVacia() const;
    ~cola();

    // NUEVO: clona la estructura de la cola (deep copy de nodos, shallow del dato)
    cola<T> Clone() const {
        cola<T> copia;
        copiar_nodos(this->cab, copia.cab, copia.fin);
        return copia;
    }
};

// Inserta un elemento al final de la cola
template <typename T>
void cola<T>::InsCola(const T& i) {
    nodoT<T>* nuevo = new nodoT<T>;
    nuevo->dato = i;
    nuevo->sig = nullptr;

    if (cab == nullptr)
        cab = nuevo;
    else
        fin->sig = nuevo;

    fin = nuevo;
}

// Atiende (elimina y devuelve) el primer elemento de la cola
template <typename T>
T cola<T>::AtenderCola() {
    if (cab == nullptr) {
        throw runtime_error("Cola vacía");
    }

    nodoT<T>* aux = cab;
    T x = aux->dato;
    cab = aux->sig;
    delete aux;

    if (cab == nullptr) fin = nullptr;

    return x;
}

template <typename T>
void cola<T>::ImprimirCola() {
    int count = 1;
    for (nodoT<T>* aux = cab; aux != nullptr; aux = aux->sig) {
        std::cout << count << "  " << aux->dato->to_string() << "\n";
        count++;
    }
    std::cout << '\n';
}

// Verifica si la cola está vacía
template <typename T>
bool cola<T>::ColaVacia() const {
    return (cab == nullptr);
}

// Destructor: libera memoria de nodos (no del dato si T es puntero)
template <typename T>
cola<T>::~cola() {
    nodoT<T>* aux;
    while (cab != nullptr) {
        aux = cab;
        cab = aux->sig;
        delete aux;
    }
}

#endif
