#ifndef PILA1_H     
#define PILA1_H    

#include <string>      
#include <cctype>      
#include <iostream>
#include "estructura1.h"

class pila {
    nodo *cab;
    int tam;

public:
    pila() {
        cab = NULL;
        tam = 0;
    }

    void Push(char v);
    char Pop();
    bool PilaVacia();
    void CargarDesdeString(const std::string &texto);  
    int getTam();
    std::string ToString() const;   // 🔹 Devuelve el contenido de la pila como string
    ~pila();
};

void pila::Push(char v) {
    nodo *t = new nodo;
    t->dato = v;
    t->sig = cab;
    cab = t;
    tam++;
}

char pila::Pop() {
    if (PilaVacia()) return '\0';
    nodo *t = cab;
    char x = t->dato;
    cab = cab->sig;
    delete t;
    tam--;
    return x;
}

bool pila::PilaVacia() {
    return (cab == NULL);
}

void pila::CargarDesdeString(const std::string &texto) {
    for (char c : texto) {
        Push(c);
    }
}

std::string pila::ToString() const {
    std::string resultado;
    nodo *t = cab;
    while (t != NULL) {
        resultado += t->dato;
        t = t->sig;
    }
    return resultado;
}

int pila::getTam(){
    return tam;
}

pila::~pila() {
    while (!PilaVacia()) {
        Pop();
    }
}

#endif
