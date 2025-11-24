#include <iostream>
#include <fstream>
#include <string>

void abrirArchivo(const std::string& url) {
	//TODO: Lista para guardar
	
	//TODO: Abrir una multilista para guardar la informacion	
    std::ifstream archivo(url);

    if (!archivo.is_open()) {
        std::cout << "No se pudo abrir el archivo." << std::endl;
        return;
    }

    std::string linea;
    while (std::getline(archivo, linea)) {
        std::cout << linea << std::endl;
    }

    archivo.close();
}

int main(int argc, char** argv) {
    abrirArchivo("Regiones.txt");
	abrirArchivo("Ciudades.txt");
    return 0;
}