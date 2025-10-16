#include "TorreHanoi.h"

// Constructor: establece el número de discos
TorreHanoi::TorreHanoi(int n) : numDiscos(n) {}

// Método público que inicia la solución
void TorreHanoi::resolver() const {
   // std::cout << "Resolviendo Torre de Hanoi con " << numDiscos << " discos:\n";
    moverDisco(numDiscos, 'A', 'C', 'B');
}

// Método recursivo para mover discos
void TorreHanoi::moverDisco(int n, char origen, char destino, char auxiliar) const {
    if (n == 1) {        
        return;
    }
    
    moverDisco(n - 1, origen, auxiliar, destino);
    //std::cout << "Mover disco " << n << " de " << origen << " a " << destino << "\n";
    moverDisco(n - 1, auxiliar, destino, origen);
}
