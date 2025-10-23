#ifndef LISTA_H
#define LISTA_H
#include <stdlib.h>


template<class T>

struct Nodo{
    T info;     
    Nodo<T>*sig;
    };
            

template <class T>
class Lista  {

      Nodo<T> *cab;
      int tam;                              
      public:
      
        Lista() {        
            cab = nullptr
            tam = 0                    
            };             
            
        void insertar_inicio(T dato);
        void insertar_final(T dato);
        void insertar_pos(T dato, int pos);        
        bool eliminar(int pos);
        T obtener_info (int pos);

        bool lista_vacia();

};


// Insertar  inicio
template <class T>
void Lista<T>::insertar_inicio(T dato){            
      Nodo<T> *nuevo;
      nuevo = new Nodo<T>;
      nuevo-> info = dato;
      nuevo->sig = cab;
      cab = nuevo;
      tam++;      
};

// Insertar final
template<class T>
void Lista<T>::insertar_final(T dato){
    Nodo<T>*nuevo;
    nuevo = new Nodo<T>;
    nuevo->info = dato;
    nuevo->sig = NULL;
    
    // Si la lista est� vacia
    
    if(cab == NULL){
        cab = nuevo;
        tam++;
        return;
    }

    // Recorr hasta el �ltimo dato
    Nodo<T>*aux = obtener_nodo_pos(tam - 1);

    aux->sig = nuevo;
    tam++;
}

//Insertar por posicion

template<class T>
void Lista<T>::insertar_pos(T dato, int pos){
    
    if(!validar_tam(pos))
        return;

    // Si es posici�n 0, insertar al inicio
    if(pos == 0){
        insertar_inicio(dato);
        return;
    }
    
    // Crear nuevo nodo
    
    Nodo<T>*nuevo = new Nodo<T>;
    nuevo->info = dato;
    
    // iterar hasta la posicion
    Nodo<T> *aux = obtener_nodo_pos(pos);
    
    nuevo->sig = aux->sig;
    aux->sig = nuevo;
    tam++;
    
}


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

    // Obtener el nodo actual y el anterior
    Nodo<T>* aux_1 = obtener_nodo_pos(pos);
    Nodo<T>* aux_2 = obtener_nodo_pos(pos - 1);

    // Caso: último nodo (cola)
    if (aux_1->sig == nullptr) {
        aux_2->sig = nullptr;
        delete aux_1;        
        tam--;
        return true;
    }

    // Enlazar los extremos y eliminar el nodo en la posición
    aux_2->sig = aux_1->sig;
    delete aux_1;    
    tam--;
    return true;
}

// retorna la informacion del vector de la posicion deseada
template<class T>
T Lista<T>::obtener_info(int pos) {

    if (!validar_tam(pos))
        return false;
    
    Nodo<T>* aux_1 = obtener_nodo_pos(pos);
    return aux_1->info;
}

// retorna la informacion del vector de la posicion deseada
template<class T>
bool Lista<T>::lista_vacia() {
    if(tam == 0)
        return true;
    
    return false;
}

// Valida el tamano de las listas
template<class T>
bool validar_tam(int pos){    
    if(pos > tam || (pos * -1) > tam){
        return false;
    }
    return true;
}

// Obtiene el nodo en la posicion N
template<class T>
Nodo<T> obtener_nodo_pos(int pos){    
    Nodo<T>*aux = cab;
    int contador = 0;
    
    while(contador < pos - 1){
        aux = aux->sig;
        contador++;
    }

    return aux;
}

#endif
