#ifndef TORRE_HANOI_H
#define TORRE_HANOI_H

#include <iostream>

class TorreHanoi {
private:
    int numDiscos;  // Número de discos
    void moverDisco(int n, char origen, char destino, char auxiliar) const;

public:
    TorreHanoi(int n);
    void resolver() const; // Inicia el proceso de resolución
};

#endif // TORRE_HANOI_H
