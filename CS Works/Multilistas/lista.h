#ifndef LISTA_H
#define LISTA_H

#include <cstdlib>
#include <stdexcept>
#include <string>

using namespace std;

template<class T>
struct Nodo {
    T id = T();
    T info;
    Nodo<T>* siguiente;
};

template <class T>
class Lista {

private:
    Nodo<T>* cabeza;
    int tam;

    bool _validarPosicion(int pos);
    Nodo<T>* _obtenerNodoPos(int pos);

public:
    // Constructor y Destructor
    Lista() : cabeza(nullptr), tam(0) {}
    ~Lista();

    // Constructor de copia (Deep Copy)
    Lista(const Lista<T>& otra);
    Lista<T>& operator=(const Lista<T>& otra);

    // Metodos de insercion
    void insertarInicio(T dato);
    void insertarFinal(T dato);
    void insertarPosicion(T dato, int pos);        
    void insertarOrden(T dato, bool asc = true);    
    template<class CampoOrden, class Extractor>
    void insertarOrdenStruct(T dato, Extractor extraerCampo, bool asc = true);
    
    // Metodos de eliminacion
    bool eliminarPosicion(int pos);
    template<class CampoOrden, class Extractor>
    bool eliminarElemento(CampoOrden dato, Extractor extraerCampo);

    // Metodos de acceso
    T obtenerInfo(int pos);
    bool estaVacia();
    int obtenerTam();
    T& operator[](int pos);
};

// ==================== IMPLEMENTACIONES ====================

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
    if (otra.cabeza == nullptr) {
        return;
    }
    
    Nodo<T>* actual = otra.cabeza;
    while (actual != nullptr) {
        this->insertarFinal(actual->info);
        actual = actual->siguiente;
    }
}

template <class T>
Lista<T>& Lista<T>::operator=(const Lista<T>& otra) {
    if (this == &otra) {
        return *this;
    }
    
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

template <class T>
void Lista<T>::insertarInicio(T dato) {
    Nodo<T>* nuevo = new Nodo<T>;
    nuevo->info = dato;
    nuevo->siguiente = cabeza;
    cabeza = nuevo;
    tam++;
}

template<class T>
void Lista<T>::insertarFinal(T dato) {
    Nodo<T>* nuevo = new Nodo<T>;
    nuevo->info = dato;
    nuevo->siguiente = nullptr;

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

    Nodo<T>* anterior = _obtenerNodoPos(pos - 1);
    nuevo->siguiente = anterior->siguiente;
    anterior->siguiente = nuevo;
    tam++;
}

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
    if (cabeza == nullptr)
        return false;

    Nodo<T>* actual = cabeza;
    Nodo<T>* anterior = nullptr;

    while (actual != nullptr) {
        CampoOrden valorActual = extraerCampo(actual->info);

        if (valorActual == valorBuscado) {
            if (anterior == nullptr) {
                cabeza = actual->siguiente;
            } else {
                anterior->siguiente = actual->siguiente;
            }

            delete actual;
            tam--;
            return true;
        }

        anterior = actual;
        actual = actual->siguiente;
    }

    return false;
}

template<class T>
void Lista<T>::insertarOrden(T dato, bool asc) {
    Nodo<T>* nuevo = new Nodo<T>;
    nuevo->info = dato;
    nuevo->siguiente = nullptr;

    if (cabeza == nullptr) {
        cabeza = nuevo;
        tam++;
        return;
    }

    if ((asc && dato < cabeza->info) || (!asc && dato > cabeza->info)) {
        nuevo->siguiente = cabeza;
        cabeza = nuevo;
        tam++;
        return;
    }

    Nodo<T>* actual = cabeza;
    
    while (actual->siguiente != nullptr) {
        T valorSiguiente = actual->siguiente->info;
        
        bool debeInsertar = (asc && dato < valorSiguiente) || 
                           (!asc && dato > valorSiguiente);
        
        if (debeInsertar) {
            break;
        }
        
        actual = actual->siguiente;
    }

    nuevo->siguiente = actual->siguiente;
    actual->siguiente = nuevo;
    tam++;
}

template<class T>
template<class CampoOrden, class Extractor>
void Lista<T>::insertarOrdenStruct(T dato, Extractor extraerCampo, bool asc) {
    Nodo<T>* nuevo = new Nodo<T>;
    nuevo->info = dato;
    nuevo->siguiente = nullptr;

    CampoOrden valorNuevo = extraerCampo(dato);

    if (cabeza == nullptr) {
        cabeza = nuevo;
        tam++;
        return;
    }

    CampoOrden valorCabeza = extraerCampo(cabeza->info);
    if ((asc && valorNuevo < valorCabeza) || (!asc && valorNuevo > valorCabeza)) {
        nuevo->siguiente = cabeza;
        cabeza = nuevo;
        tam++;
        return;
    }

    Nodo<T>* actual = cabeza;
    
    while (actual->siguiente != nullptr) {
        CampoOrden valorSiguiente = extraerCampo(actual->siguiente->info);
        
        bool debeInsertar = (asc && valorNuevo < valorSiguiente) || 
                           (!asc && valorNuevo > valorSiguiente);
        
        if (debeInsertar) {
            break;
        }
        
        actual = actual->siguiente;
    }

    nuevo->siguiente = actual->siguiente;
    actual->siguiente = nuevo;
    tam++;
}

template<class T>
T& Lista<T>::operator[](int pos) {
    if (!_validarPosicion(pos))
        throw out_of_range("Posicion invalida");

    return _obtenerNodoPos(pos)->info;
}

template<class T>
T Lista<T>::obtenerInfo(int pos) {
    if (!_validarPosicion(pos))
        throw out_of_range("Posicion invalida");

    Nodo<T>* aux = _obtenerNodoPos(pos);
    return aux->info;
}

template<class T>
bool Lista<T>::estaVacia() {
    return tam == 0;
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
int Lista<T>::obtenerTam() {
    return tam;
}

#endif
