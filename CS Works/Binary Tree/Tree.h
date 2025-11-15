#ifndef TREE_H
#define TREE_H

#include "Lista.h"
#include <stdexcept>

template <class T>
class Tree {

private:    
    Lista<T> datos;

    Lista<bool> ocupado;

    int cantidad;

    int _indiceHijoIzquierdo(int i) const { return 2 * i; }
    int _indiceHijoDerecho(int i) const { return 2 * i + 1; }
    int _indicePadre(int i) const { return i / 2; }

    void _asegurarCapacidad(int indiceMax) {
        int tamActual = datos.obtenerTam();
        
        if (tamActual > indiceMax) {
            return;
        }

        // Si está vacío, primero crear la posición 0 (espacio de control)
        if (tamActual == 0) {
            datos.insertarFinal(T());     
            ocupado.insertarFinal(false);
            tamActual = 1;
        }

        // Agregar elementos por defecto hasta llegar a indiceMax
        while (tamActual <= indiceMax) {
            datos.insertarFinal(T());   
            ocupado.insertarFinal(false); 
            tamActual++;
        }
    }

    bool _indiceValido(int indice) const {
        // debe estar dentro de rango y marcado como ocupado
        if (indice <= 0 || indice >= ocupado.obtenerTam()) {
            return false;
        }
        return ocupado.obtenerInfo(indice);
    }

    // Eliminación recursiva de subárbol a partir de un índice
    void _eliminarSubarbolRec(int indice) {
        if (!_indiceValido(indice))
            return;

        int idxIzq = _indiceHijoIzquierdo(indice);
        int idxDer = _indiceHijoDerecho(indice);

        if (_indiceValido(idxIzq))
            _eliminarSubarbolRec(idxIzq);
        if (_indiceValido(idxDer))
            _eliminarSubarbolRec(idxDer);

        ocupado[indice] = false;
        cantidad--;
    }

public:    

    Tree() : datos(), ocupado(), cantidad(0) {
        // crear posición 0 como espacio de control
        datos.insertarFinal(T());
        ocupado.insertarFinal(false);
    }
	
	~Tree() {        
        limpiar();
        
        while (!datos.estaVacia()) {
            datos.eliminarPosicion(0);
        }
        while (!ocupado.estaVacia()) {
            ocupado.eliminarPosicion(0);
        }

    }
	
    // ==================== CONSULTAS BÁSICAS ====================

    bool estaVacio() const {
        return cantidad == 0;
    }

    int obtenerCantidad() const {
        return cantidad;
    }

    bool existeEnIndice(int indice) const {
        return _indiceValido(indice);
    }

    // ==================== RAÍZ ====================

    bool tieneRaiz() const {
        return _indiceValido(1);
    }

    T& raiz() {
        if (!tieneRaiz())
            throw std::runtime_error("El arbol no tiene raiz");
        return datos[1];
    }

    const T& raiz() const {
        if (!tieneRaiz())
            throw std::runtime_error("El arbol no tiene raiz");
        return datos[1];
    }

    void insertarRaiz(const T& valor) {
        _asegurarCapacidad(1);
        if (!ocupado.obtenerInfo(1)) {
            cantidad++;
        }
        datos[1] = valor;
        ocupado[1] = true;
    }

    // ==================== INSERCIÓN DE HIJOS ====================

    void insertarHijoIzquierdo(int indicePadre, const T& valor) {
        if (!_indiceValido(indicePadre))
            throw std::out_of_range("Indice de padre invalido");

        int idxHijo = _indiceHijoIzquierdo(indicePadre);
        _asegurarCapacidad(idxHijo);

        if (!ocupado.obtenerInfo(idxHijo))
            cantidad++;

        datos[idxHijo] = valor;
        ocupado[idxHijo] = true;
    }

    void insertarHijoDerecho(int indicePadre, const T& valor) {
        if (!_indiceValido(indicePadre))
            throw std::out_of_range("Indice de padre invalido");

        int idxHijo = _indiceHijoDerecho(indicePadre);
        _asegurarCapacidad(idxHijo);

        if (!ocupado.obtenerInfo(idxHijo))
            cantidad++;

        datos[idxHijo] = valor;
        ocupado[idxHijo] = true;
    }

    // ==================== CONSULTA DE HIJOS ====================

    bool tieneHijoIzquierdo(int indicePadre) const {
        int idx = _indiceHijoIzquierdo(indicePadre);
        return _indiceValido(idx);
    }

    bool tieneHijoDerecho(int indicePadre) const {
        int idx = _indiceHijoDerecho(indicePadre);
        return _indiceValido(idx);
    }

    T& hijoIzquierdo(int indicePadre) {
        int idx = _indiceHijoIzquierdo(indicePadre);
        if (!_indiceValido(idx))
            throw std::out_of_range("Hijo izquierdo inexistente");
        return datos[idx];
    }

    T& hijoDerecho(int indicePadre) {
        int idx = _indiceHijoDerecho(indicePadre);
        if (!_indiceValido(idx))
            throw std::out_of_range("Hijo derecho inexistente");
        return datos[idx];
    }

    const T& hijoIzquierdoConst(int indicePadre) const {
        int idx = _indiceHijoIzquierdo(indicePadre);
        if (!_indiceValido(idx))
            throw std::out_of_range("Hijo izquierdo inexistente");
        return datos.obtenerInfo(idx);
    }

    const T& hijoDerechoConst(int indicePadre) const {
        int idx = _indiceHijoDerecho(indicePadre);
        if (!_indiceValido(idx))
            throw std::out_of_range("Hijo derecho inexistente");
        return datos.obtenerInfo(idx);
    }

    // ==================== ACCESO TIPO ARREGLO ====================

    T& operator[](int indice) {
        if (!_indiceValido(indice))
            throw std::out_of_range("Indice invalido en el arbol");
        return datos[indice];
    }

    const T& operator[](int indice) const {
        if (!_indiceValido(indice))
            throw std::out_of_range("Indice invalido en el arbol");
        return datos.obtenerInfo(indice);
    }

    // ==================== RELACIONES ESTRUCTURALES ====================

    int obtenerIndicePadre(int indice) const {
        if (!_indiceValido(indice) || indice == 1)
            throw std::out_of_range("No existe padre para este indice");
        return _indicePadre(indice);
    }

    bool esHoja(int indice) const {
        if (!_indiceValido(indice))
            throw std::out_of_range("Indice invalido");
        int idxIzq = _indiceHijoIzquierdo(indice);
        int idxDer = _indiceHijoDerecho(indice);
        return !(_indiceValido(idxIzq) || _indiceValido(idxDer));
    }

    // ==================== ELIMINACIÓN ====================

    void eliminarSubarbol(int indice) {
        _eliminarSubarbolRec(indice);
    }

    void limpiar() {
        int tam = ocupado.obtenerTam();
        for (int i = 1; i < tam; ++i) {
            ocupado[i] = false;
        }
        cantidad = 0;
    }

    // ==================== ALTURA (APROXIMADA) ====================

    int altura() const {
        if (estaVacio())
            return -1;

        int maxIndice = 0;
        int tam = ocupado.obtenerTam();
        for (int i = 1; i < tam; ++i) {
            if (ocupado.obtenerInfo(i))
                maxIndice = i;
        }

        int h = 0;
        while ((1 << h) <= maxIndice) {
            h++;
        }
        return h - 1;
    }

};

#endif

