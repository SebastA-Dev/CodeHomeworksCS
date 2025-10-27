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
	
	    // Mï¿½todos auxiliares internos
	    bool _validarPosicion(int pos);
	    Nodo<T>* _obtenerNodoPos(int pos);
	
	public:
	    Lista() : cabeza(nullptr), tam(0) {}
	    ~Lista(); // âœ… Destructor agregado
	
	    void insertarInicio(T dato);
	    void insertarFinal(T dato);
	    void insertarPosicion(T dato, int pos);
	    bool eliminarPosicion(int pos);
	    T obtenerInfo(int pos);
	    bool estaVacia();
	    int obtenerTam();
	    T& operator[](int pos);
	};
	
	
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
	
	// Insertar por posiciï¿½n
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
	
	// Operador []
	template<class T>
	T& Lista<T>::operator[](int pos) {
	    if (!_validarPosicion(pos))
	        throw std::out_of_range("Posiciï¿½n invï¿½lida");
	
	    return _obtenerNodoPos(pos)->info;
	}
	
	// Obtener informaciï¿½n en posiciï¿½n
	template<class T>
	T Lista<T>::obtenerInfo(int pos) {
	    if (!_validarPosicion(pos))
	        throw std::out_of_range("Posiciï¿½n invï¿½lida");
	
	    Nodo<T>* aux = _obtenerNodoPos(pos);
	    return aux->info;
	}
	
	// Verificar si la lista estï¿½ vacï¿½a
	template<class T>
	bool Lista<T>::estaVacia() {
	    return tam == 0;
	}
	
	// Validar posiciï¿½n
	template<class T>
	bool Lista<T>::_validarPosicion(int pos) {
	    return pos >= 0 && pos < tam;
	}
	
	// Obtener nodo en posiciï¿½n
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
	
	// Obtener tamaï¿½o
	template<class T>
	int Lista<T>::obtenerTam() {
	    return tam;
	}
	
	#endif

