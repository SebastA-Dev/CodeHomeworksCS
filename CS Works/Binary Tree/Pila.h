#ifndef PILA_H
#define PILA_H

#include "Lista.h"

// se crea con Lista

template <class T>
class Pila {

private:
    Lista<T> datos;

public:
    Pila() : datos() {}

    ~Pila() {}

    // Apilar (insertar al inicio)
    
    void apilar(const T& valor) {
        datos.insertarInicio(valor);
    }

    // Desapilar (extraer del inicio)
    
    T desapilar() {
        if (estaVacia()) {
            throw std::runtime_error("Pila vacia");
        }
        T valor = datos.obtenerInfo(0);
        datos.eliminarPosicion(0);
        return valor;
    }

    // Consultar tope
    T& tope() {
        if (estaVacia()) {
            throw std::runtime_error("Pila vacia");
        }
        return datos[0];
    }

    const T& topeConst() const {
        if (estaVacia()) {
            throw std::runtime_error("Pila vacia");
        }
        return datos.obtenerInfo(0);
    }

    // si está vacía
    
    bool estaVacia() const {
        return datos.estaVacia();
    }

    // tamaño
    int obtenerTam() const {
        return datos.obtenerTam();
    }

    // eliminar todos los datos
    void vaciar() {
        while (!estaVacia()) {
            desapilar();
        }
    }
};

#endif
