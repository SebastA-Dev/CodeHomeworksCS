#ifndef COLA_H
#define COLA_H

#include "Lista.h"

// se crea con Lista

template <class T>
class Cola {

private:
    Lista<T> datos;

public:
    Cola() : datos() {}

    ~Cola() {}

    // Encolar (insertar al final)
    
    void encolar(const T& valor) {
        datos.insertarFinal(valor);
    }

    // Desencolar (extraer del inicio)
    
    T desencolar() {
        if (estaVacia()) {
            throw std::runtime_error("Cola vacia");
        }
        T valor = datos.obtenerInfo(0);
        datos.eliminarPosicion(0);
        return valor;
    }

    // Consultar
    T& frente() {
        if (estaVacia()) {
            throw std::runtime_error("Cola vacia");
        }
        return datos[0];
    }

    const T& frenteConst() const {
        if (estaVacia()) {
            throw std::runtime_error("Cola vacia");
        }
        return datos.obtenerInfo(0);
    }

    // siestá vacía
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
            desencolar();
        }
    }

    // obtener los datos con la iteración de la posición
    T obtenerEnPosicion(int pos) const {
        return datos.obtenerInfo(pos);
    }
};

#endif
