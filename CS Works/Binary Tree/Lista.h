#ifndef LISTA_H
#define LISTA_H

#include <cstdlib>
#include <stdexcept>

// ==================== NODO ====================

template<class T>
struct Nodo {
    T id = T();             // Campo opcional de identificación
    T info;                 // Información almacenada
    Nodo<T>* siguiente;     // Puntero al siguiente nodo

    Nodo() : info(T()), siguiente(nullptr) {}
};

// ==================== LISTA CON NODO DE CONTROL ====================

template <class T>
class Lista {

private:    
    Nodo<T>* cabeza;
    int tam;

    // Métodos auxiliares internos
    bool _validarPosicion(int pos) const;
    Nodo<T>* _obtenerNodoPos(int pos) const;

public:
    // Constructor y Destructor
    Lista();
    ~Lista();

    // Constructor de copia y operador de asignación (deep copy)
    Lista(const Lista<T>& otra);
    Lista<T>& operator=(const Lista<T>& otra);

    // Métodos de inserción
    void insertarInicio(const T& dato);
    void insertarFinal(const T& dato);
    void insertarPosicion(const T& dato, int pos);        
    void insertarOrden(const T& dato, bool asc = true);    

    template<class CampoOrden, class Extractor>
    void insertarOrdenStruct(const T& dato, Extractor extraerCampo, bool asc = true);
    
    // Eliminación
    bool eliminarPosicion(int pos);

    template<class CampoOrden, class Extractor>
    bool eliminarElemento(const CampoOrden& dato, Extractor extraerCampo);

    // Acceso
    T obtenerInfo(int pos) const;
    bool estaVacia() const;
    int obtenerTam() const;
    T& operator[](int pos);
    const T& operator[](int pos) const;
};

// ==================== IMPLEMENTACIONES ====================

// Constructor: crea nodo de control
template <class T>
Lista<T>::Lista() : cabeza(new Nodo<T>), tam(0) {
    cabeza->siguiente = nullptr;
}

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
Lista<T>::Lista(const Lista<T>& otra) : cabeza(new Nodo<T>), tam(0) {
    cabeza->siguiente = nullptr;

    Nodo<T>* actual = otra.cabeza->siguiente;
    while (actual != nullptr) {
        this->insertarFinal(actual->info);
        actual = actual->siguiente;
    }
}

// Operador de asignación (Deep Copy)
template <class T>
Lista<T>& Lista<T>::operator=(const Lista<T>& otra) {
    if (this == &otra) {
        return *this;
    }

    // Liberar todos los nodos excepto el centinela
    Nodo<T>* actual = cabeza->siguiente;
    while (actual != nullptr) {
        Nodo<T>* temp = actual;
        actual = actual->siguiente;
        delete temp;
    }
    cabeza->siguiente = nullptr;
    tam = 0;

    // Copiar elementos reales de la otra lista
    Nodo<T>* actualOtra = otra.cabeza->siguiente;
    while (actualOtra != nullptr) {
        this->insertarFinal(actualOtra->info);
        actualOtra = actualOtra->siguiente;
    }

    return *this;
}

// Insertar al inicio (posición lógica 0)
template <class T>
void Lista<T>::insertarInicio(const T& dato) {
    Nodo<T>* nuevo = new Nodo<T>;
    nuevo->info = dato;
    // insertar justo después del centinela
    nuevo->siguiente = cabeza->siguiente;
    cabeza->siguiente = nuevo;
    tam++;
}

// Insertar al final
template<class T>
void Lista<T>::insertarFinal(const T& dato) {
    Nodo<T>* nuevo = new Nodo<T>;
    nuevo->info = dato;
    nuevo->siguiente = nullptr;

    Nodo<T>* aux = cabeza;
    // recorrer hasta el último nodo real
    while (aux->siguiente != nullptr) {
        aux = aux->siguiente;
    }

    aux->siguiente = nuevo;
    tam++;
}

// Insertar por posición (0 .. tam)
template<class T>
void Lista<T>::insertarPosicion(const T& dato, int pos) {
    // se permite pos == tam para insertar al final
    if (pos < 0 || pos > tam) {
        throw std::out_of_range("Posicion invalida");
    }

    if (pos == 0) {
        insertarInicio(dato);
        return;
    }

    // obtener nodo anterior (pos-1) empezando desde centinela
    Nodo<T>* anterior = _obtenerNodoPos(pos - 1);
    if (anterior == nullptr) {
        throw std::out_of_range("Posicion invalida");
    }

    Nodo<T>* nuevo = new Nodo<T>;
    nuevo->info = dato;
    nuevo->siguiente = anterior->siguiente;
    anterior->siguiente = nuevo;
    tam++;
}

// Eliminar nodo en posición
template<class T>
bool Lista<T>::eliminarPosicion(int pos) {
    if (!_validarPosicion(pos))
        return false;

    // obtener nodo anterior al que se elimina
    Nodo<T>* anterior = (pos == 0) ? cabeza : _obtenerNodoPos(pos - 1);
    Nodo<T>* nodoEliminar = anterior->siguiente;

    if (nodoEliminar == nullptr)
        return false;

    anterior->siguiente = nodoEliminar->siguiente;
    delete nodoEliminar;
    tam--;
    return true;
}

// Eliminar elemento por campo
template<class T>
template<class CampoOrden, class Extractor>
bool Lista<T>::eliminarElemento(const CampoOrden& valorBuscado, Extractor extraerCampo) {
    if (cabeza->siguiente == nullptr)
        return false;

    Nodo<T>* actual = cabeza->siguiente;
    Nodo<T>* anterior = cabeza;

    while (actual != nullptr) {
        CampoOrden valorActual = extraerCampo(actual->info);

        if (valorActual == valorBuscado) {
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

// Insertar ordenado para tipos básicos comparables
template<class T>
void Lista<T>::insertarOrden(const T& dato, bool asc) {
    Nodo<T>* nuevo = new Nodo<T>;
    nuevo->info = dato;
    nuevo->siguiente = nullptr;

    Nodo<T>* actual = cabeza;

    // buscar posición donde insertar
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

// Insertar ordenado usando un campo de una estructura
template<class T>
template<class CampoOrden, class Extractor>
void Lista<T>::insertarOrdenStruct(const T& dato, Extractor extraerCampo, bool asc) {
    Nodo<T>* nuevo = new Nodo<T>;
    nuevo->info = dato;
    nuevo->siguiente = nullptr;

    CampoOrden valorNuevo = extraerCampo(dato);

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

// Operador [] (acceso por referencia)
template<class T>
T& Lista<T>::operator[](int pos) {
    if (!_validarPosicion(pos))
        throw std::out_of_range("Posicion invalida");

    return _obtenerNodoPos(pos)->info;
}

// Versión const del operador []
template<class T>
const T& Lista<T>::operator[](int pos) const {
    if (!_validarPosicion(pos))
        throw std::out_of_range("Posicion invalida");

    return _obtenerNodoPos(pos)->info;
}

// Obtener información en posición
template<class T>
T Lista<T>::obtenerInfo(int pos) const {
    if (!_validarPosicion(pos))
        throw std::out_of_range("Posicion invalida");

    Nodo<T>* aux = _obtenerNodoPos(pos);
    return aux->info;
}

// Verificar si la lista está vacía
template<class T>
bool Lista<T>::estaVacia() const {
    return tam == 0;
}

// Validar posición (solo elementos reales 0..tam-1)
template<class T>
bool Lista<T>::_validarPosicion(int pos) const {
    return pos >= 0 && pos < tam;
}

// Obtener nodo en posición lógica (0..tam-1)
// Se recorre desde el centinela (posición lógica -1)
template<class T>
Nodo<T>* Lista<T>::_obtenerNodoPos(int pos) const {
    if (!_validarPosicion(pos))
        return nullptr;

    Nodo<T>* aux = cabeza->siguiente;
    int contador = 0;

    while (contador < pos && aux != nullptr) {
        aux = aux->siguiente;
        contador++;
    }

    return aux;
}

// Obtener tamaño
template<class T>
int Lista<T>::obtenerTam() const {
    return tam;
}

#endif
