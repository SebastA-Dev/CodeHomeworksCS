#ifndef LISTA_H
#define LISTA_H
#include<stdlib>


template<class T>

struct Nodo{

      T info;
      
            Nodo<T>*sig;
            
            }
            

template <CLass T>
Class Lista  {

      Nodo<T> *cab;
      int tam;
                        
      
      
      Public:
      
            Lista() {
            
                  cab = NULL
                  tam = 0
                        
                        }
                        
            
            void insertar_inicio(T dato);
            void insertar_final(T dato);
            void insertar_pos(T dato, int pos);
            bool eliminar(int pos);
            T obtener_info (int pos);
            bool lista_vacia();
            
};


// Insertar  inicio
template <Class T>
void Lista<T>::insertar_inicio(T dato){
      
      
      Nodo<T>*nuevo;
      nuevo = new Nodo<T>;
      nuevo-> info = dato;
      nuevo->sig = cabeza;
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
    
    // Si la lista está vacia
    
    if(cab == NULL){
        cab = nuevo;
    }
    else{
    	
        // Recorr hasta el último dato
        Nodo<T>*aux = cab;
        while(aux->sig != NULL){
            aux = aux->sig;
        }
        aux->sig = nuevo;
    }
    tam++;
}

//Insertar por posicion

template<class T>
void Lista<T>::insertar_pos(T dato, int pos){
	
	
    // PENDIENTE, definir para validar la posición, en caso que la posición ingresada sea < 0 o > tamaño de la lista

    
    // Si es posición 0, insertar al inicio
    
    if(pos == 0){
        insertar_inicio(dato);
        return;
    }
    
    // Crear nuevo nodo
    
    Nodo<T>*nuevo = new Nodo<T>;
    nuevo->info = dato;
    
    // iterar hasta la posicion
    Nodo<T>*aux = cab;
    int contador = 0;
    
    while(contador < pos - 1){
        aux = aux->sig;
        contador++;
    }
    
    nuevo->sig = aux->sig;
    aux->sig = nuevo;
    tam++;
    
}







#endif
