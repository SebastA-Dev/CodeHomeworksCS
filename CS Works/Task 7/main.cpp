#include <iostream>
#include "Listas.h"
using namespace std;

int main() {
    Lista<int> lista;

    lista.insertar_inicio(10);
    lista.insertar_inicio(20);
    lista.insertar_inicio(30);
	
	cout << "Mostrando elementos:\n";
    for (int i = 0; i < lista.getTam(); i++) {
        cout << "Pos " << i << ": " << lista.obtener_info(i) << endl;
    }
	
    lista.insertar_final(40);
    lista.insertar_final(50);
	
	cout << "Mostrando elementos:\n";
    for (int i = 0; i < lista.getTam(); i++) {
        cout << "Pos " << i << ": " << lista.obtener_info(i) << endl;
    }
    
    lista.insertar_pos(25, 2);

    cout << "Mostrando elementos:\n";
    for (int i = 0; i < lista.getTam(); i++) {
        cout << "Pos " << i << ": " << lista.obtener_info(i) << endl;
    }


    lista.eliminar(3);

    cout << "Mostrando elementos:\n";
    for (int i = 0; i < lista.getTam(); i++) {
        cout << "Pos " << i << ": " << lista.obtener_info(i) << endl;
    }    

    lista[1] = 12312;
    
    cout << "Mostrando elementos:\n";
    for (int i = 0; i < lista.getTam(); i++) {
        cout << "Pos " << i << ": " << lista.obtener_info(i) << endl;
    }
    return 0;
}
