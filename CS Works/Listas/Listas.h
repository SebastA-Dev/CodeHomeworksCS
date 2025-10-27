#ifndef LISTA_H
#define LISTA_H

#include <cstdlib>
#include <stdexcept> // Para std::out_of_range

template<class T>
struct Nodo {
    T id = T();  // Valor por defecto del tipo T
    T info;
    Nodo<T>* siguiente;
};

template <class T>
class Lista {

private:
    Nodo<T>* cabeza;
    int tam;

    // Métodos auxiliares internos
    bool _validarPosicion(int pos);
    Nodo<T>* _obtenerNodoPos(int pos);

public:
	//Constructor y Destructor
    Lista() : cabeza(nullptr), tam(0) {}
    ~Lista();

	//Constructor y Copiados (anidaciones)
    Lista(const Lista<T>& otra);
    Lista<T>& operator=(const Lista<T>& otra);

	//metodos de insercion
    void insertarInicio(T dato);
    void insertarFinal(T dato);
    void insertarPosicion(T dato, int pos);        
    void insertarOrden(T dato, bool asc = true);    
    template<class CampoOrden, class Extractor>
    void insertarOrdenStruct(T dato, Extractor extraerCampo, bool asc = true);
    
    bool eliminarPosicion(int pos);

    template<class CampoOrden, class Extractor>
    bool eliminarPosicion(T dato, Extractor extraerCampo);
    T obtenerInfo(int pos);
    bool estaVacia();
    int obtenerTam();
    T& operator[](int pos);
};

// ==================== IMPLEMENTACIONES ====================

// Destructor
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

// Constructor de copia (Deep Copy)
template <class T>
Lista<T>::Lista(const Lista<T>& otra) : cabeza(nullptr), tam(0) {
    // Si la lista original está vacía, no hay nada que copiar
    if (otra.cabeza == nullptr) {
        return;
    }
    
    // Copiar cada elemento de la lista original
    Nodo<T>* actual = otra.cabeza;
    while (actual != nullptr) {
        this->insertarFinal(actual->info);
        actual = actual->siguiente;
    }
}

// Operador de asignación (Deep Copy)
template <class T>
Lista<T>& Lista<T>::operator=(const Lista<T>& otra) {
    // Evitar auto-asignación
    if (this == &otra) {
        return *this;
    }
    
    // Liberar memoria actual
    Nodo<T>* actual = cabeza;
    while (actual != nullptr) {
        Nodo<T>* temp = actual;
        actual = actual->siguiente;
        delete temp;
    }
    cabeza = nullptr;
    tam = 0;
    
    // Copiar elementos de la otra lista
    actual = otra.cabeza;
    while (actual != nullptr) {
        this->insertarFinal(actual->info);
        actual = actual->siguiente;
    }
    
    return *this;
}

// Insertar al inicio
template <class T>
void Lista<T>::insertarInicio(T dato) {
    Nodo<T>* nuevo = new Nodo<T>;
    nuevo->info = dato;
    nuevo->siguiente = cabeza;
    cabeza = nuevo;
    tam++;
}

// Insertar al final
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

// Insertar por posición
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

// Eliminar nodo en posición
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

// Hace una insercion buscando un tipo de dato especifico siguiente la logica detras
// de datos abstractos y un campo de orden
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


// Operador []
template<class T>
T& Lista<T>::operator[](int pos) {
    if (!_validarPosicion(pos))
        throw std::out_of_range("Posición inválida");

    return _obtenerNodoPos(pos)->info;
}

// Obtener información en posición
template<class T>
T Lista<T>::obtenerInfo(int pos) {
    if (!_validarPosicion(pos))
        throw std::out_of_range("Posición inválida");

    Nodo<T>* aux = _obtenerNodoPos(pos);
    return aux->info;
}

// Verificar si la lista está vacía
template<class T>
bool Lista<T>::estaVacia() {
    return tam == 0;
}

// Validar posición
template<class T>
bool Lista<T>::_validarPosicion(int pos) {
    return pos >= 0 && pos < tam;
}

// Obtener nodo en posición
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

// Obtener tamaño
template<class T>
int Lista<T>::obtenerTam() {
    return tam;
}

#endif
