// UtilidadesAnalisis.cpp
#include "UtilidadesAnalisis.h"
#include <sstream>
#include <iomanip>

std::string UtilidadesAnalisis::stringToHex(const std::string &input) {
    std::ostringstream oss;
    for (unsigned char c : input) {        
        oss << std::hex << std::setw(2) << std::setfill('0')
            << static_cast<int>(c);
    }
    return oss.str();
}


int UtilidadesAnalisis::calcularEdad(const std::tm& fechaNacimiento) {
    time_t ahora = std::time(nullptr);
    std::tm* fechaActual = std::localtime(&ahora);
    
    int edad = fechaActual->tm_year - fechaNacimiento.tm_year;
    
    // En caso que no hay  cumplido años
    if (fechaActual->tm_mon < fechaNacimiento.tm_mon ||
        (fechaActual->tm_mon == fechaNacimiento.tm_mon && 
        
         fechaActual->tm_mday < fechaNacimiento.tm_mday)) {
         	
        edad--;
    }
    
    return edad;
}

int UtilidadesAnalisis::busquedaBinariaString(const std::vector<std::string>& elementos, const std::string& buscado) {
	
    int izquierda = 0;
    int derecha = elementos.size() - 1;
    
    while (izquierda <= derecha) {
        int medio = izquierda + (derecha - izquierda) / 2;
        
        if (elementos[medio] == buscado) {
            return medio;  // Encuentra el medio
        }
        else if (elementos[medio] < buscado) {
            izquierda = medio + 1;  // Busca en la derecha
        }
        else {
            derecha = medio - 1;  // Busca en la izquierda
        }
    }
    
    return -1;  // No encontró
}

