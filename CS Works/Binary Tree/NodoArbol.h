#ifndef NODO_ARBOL_H
#define NODO_ARBOL_H


// datos del arbol
template <class T>
struct NodoArbol {
    T clave;                    // La clave de ordenamiento
    int idInfo;                 // ID que referencia información en archivo
    int hijoIzquierdo;         // Índice del hijo izquierdo (-1 si no existe)
    int hijoDerecho;           // Índice del hijo derecho (-1 si no existe)
    bool eliminado;            // Flag para marca lógica de eliminación

    NodoArbol() 
        : clave(T()), idInfo(-1), hijoIzquierdo(-1), hijoDerecho(-1), eliminado(false) {}

    NodoArbol(const T& c, int id)
        : clave(c), idInfo(id), hijoIzquierdo(-1), hijoDerecho(-1), eliminado(false) {}
};

#endif
