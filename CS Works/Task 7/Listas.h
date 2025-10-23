#ifndef LISTA_H
#define LISTA_H
#include <stdlib.h>
#include <stdexcept> // Para std::out_of_range

template<class T>
struct Nodo {
    T info;     
    Nodo<T>* sig;
};

template <class T>
class Lista {

private:
    Nodo<T>* cab;
    int tam;

    // Métodos auxiliares internos
    bool validar_tam(int pos);
    Nodo<T>* obtener_nodo_pos(int pos);

public:
    Lista() {
        cab = nullptr;
        tam = 0;
    }

    void insertar_inicio(T dato);
    void insertar_final(T dato);
    void insertar_pos(T dato, int pos);
    bool eliminar(int pos);
    T obtener_info(int pos);
    bool lista_vacia();     
    int getTam();
};

// Insertar al inicio
template <class T>
void Lista<T>::insertar_inicio(T dato) {
    Nodo<T>* nuevo = new Nodo<T>;
    nuevo->info = dato;
    nuevo->sig = cab;
    cab = nuevo;
    tam++;
}

// Insertar al final
template<class T>
void Lista<T>::insertar_final(T dato) {
    Nodo<T>* nuevo = new Nodo<T>;
    nuevo->info = dato;
    nuevo->sig = nullptr;

    if (cab == nullptr) {
        cab = nuevo;
        tam++;
        return;
    }

    Nodo<T>* aux = obtener_nodo_pos(tam - 1);
    aux->sig = nuevo;
    tam++;
}

// Insertar por posición
template<class T>
void Lista<T>::insertar_pos(T dato, int pos) {
    if (!validar_tam(pos))
        return;

    if (pos == 0) {
        insertar_inicio(dato);
        return;
    }

    Nodo<T>* nuevo = new Nodo<T>;
    nuevo->info = dato;

    Nodo<T>* aux = obtener_nodo_pos(pos - 1);
    nuevo->sig = aux->sig;
    aux->sig = nuevo;
    tam++;
}

// Eliminar nodo en posición
template<class T>
bool Lista<T>::eliminar(int pos) {
    if (!validar_tam(pos))
        return false;

    // Caso especial: eliminar el primer nodo
    if (pos == 0) {
        Nodo<T>* aux = cab;
        cab = cab->sig;
        delete aux;
        aux = nullptr;
        tam--;
        return true;
    }

    Nodo<T>* aux_1 = obtener_nodo_pos(pos);
    Nodo<T>* aux_2 = obtener_nodo_pos(pos - 1);

    if (aux_1->sig == nullptr) {
        aux_2->sig = nullptr;
        delete aux_1;
        tam--;
        return true;
    }

    aux_2->sig = aux_1->sig;
    delete aux_1;
    tam--;
    return true;
}

// Obtener información en posición
template<class T>
T Lista<T>::obtener_info(int pos) {
    if (!validar_tam(pos))
        throw std::out_of_range("Posición inválida");

    Nodo<T>* aux = obtener_nodo_pos(pos);
    return aux->info;
}

// Verificar si la lista está vacía
template<class T>
bool Lista<T>::lista_vacia() {
    return tam == 0;
}

// Validar tamaño
template<class T>
bool Lista<T>::validar_tam(int pos) {
    if (pos < 0 || pos >= tam)
        return false;
    return true;
}

// Obtener nodo en posición
template<class T>
Nodo<T>* Lista<T>::obtener_nodo_pos(int pos) {
    Nodo<T>* aux = cab;
    int contador = 0;

    while (contador < pos && aux != nullptr) {
        aux = aux->sig;
        contador++;
    }

    return aux;
}

template<class T>
int Lista<T>::getTam() {
    return tam;
}

#endif
