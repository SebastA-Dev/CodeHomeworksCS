#ifndef COLA1_H     
#define COLA1_H 

#include <cstdlib>
#include <iostream>
#include "estructura1.h"

using namespace std;

class cola {
    nodo *cab, *fin;

public:
    cola() { cab = fin = NULL; }
    void InsCola(char i);
    char AtenderCola();
    void ImprimirCola();
    bool ColaVacia();
    ~cola();
};

// Inserta un elemento al final de la cola
void cola::InsCola(char i) {
    nodo *nuevo = new nodo;
    nuevo->dato = i;
    nuevo->sig = NULL;

    if (cab == NULL)
        cab = nuevo;
    else
        fin->sig = nuevo;

    fin = nuevo;
}

// Atiende (elimina y devuelve) el primer elemento de la cola
char cola::AtenderCola() {
    char x;
    nodo *aux = cab;
    cab = aux->sig;
    x = aux->dato;
    delete aux;
    return x;
}

// Imprime todos los elementos de la cola
void cola::ImprimirCola() {
    nodo *aux = cab;
    while (aux != NULL) {
        cout << aux->dato << " ";
        aux = aux->sig;
    }
}

// Verifica si la cola est� vac�a
bool cola::ColaVacia() {
    return (cab == NULL);
}

// Destructor: libera memoria
cola::~cola() {
    nodo *aux;
    while (cab != NULL) {
        aux = cab;
        cab = aux->sig;
        delete aux;
    }
    delete cab;
}

#endif
