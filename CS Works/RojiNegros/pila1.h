#ifndef PILA_H
#define PILA_H

#include <iostream>

template <class T>
class NodoPila {
public:
    T dato;
    NodoPila<T>* sig;

    NodoPila(T d) : dato(d), sig(nullptr) {}
};

template <class T>
class pila {
private:
    NodoPila<T>* tope;

public:
    pila() { tope = nullptr; }

    bool PilaVacia() {
        return tope == nullptr;
    }

    void Push(T x) {
        NodoPila<T>* nuevo = new NodoPila<T>(x);
        nuevo->sig = tope;
        tope = nuevo;
    }

    T Pop() {
        if (PilaVacia()) {
            std::cerr << "Error: pila vacía\n";
            return nullptr; // porque lo estás usando con Node*
        }
        NodoPila<T>* temp = tope;
        T dato = temp->dato;
        tope = tope->sig;
        delete temp;
        return dato;
    }

    T Top() {
        if (PilaVacia()) {
            std::cerr << "Error: pila vacía\n";
            return nullptr;
        }
        return tope->dato;
    }

    ~pila() {
        while (!PilaVacia()) Pop();
    }
};

#endif
