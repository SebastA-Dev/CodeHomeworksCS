#ifndef LISTA_H
#define LISTA_H

#include <string>

// ==========================
//     Nodo<T>
// ==========================
template<class T>
struct Nodo {
    std::string id;   // Se guarda siempre en HEX
    T info;
    Nodo<T>* siguiente;
};

// Convierte un unsigned long a string HEX
std::string toHexString(unsigned long value);

// ==========================
//      Clase Lista<T>
// ==========================
template <class T>
class Lista {

private:
    Nodo<T>* cabeza;
    int tam;

    bool _validarPosicion(int pos);
    Nodo<T>* _obtenerNodoPos(int pos);

public:
    // Constructores / destructores
    Lista();
    ~Lista();
    Lista(const Lista<T>& otra);
    Lista<T>& operator=(const Lista<T>& otra);

    // Inserciones
    void insertarInicio(T dato);
    void insertarFinal(T dato);
    void insertarPosicion(T dato, int pos);

    void insertarOrdenPorPeso(T dato, std::string peso, bool asc = true);

    // Eliminaciones
    bool eliminarPosicion(int pos);

    template<class CampoOrden, class Extractor>
    bool eliminarElemento(CampoOrden dato, Extractor extraerCampo);

    // Acceso
    T obtenerInfo(int pos);
    bool estaVacia();
    int obtenerTam();
    T& operator[](int pos);
    std::string obtenerIdNodo(int pos);
};

#endif
