// UtilidadesAnalisis.cpp
#include "UtilidadesAnalisis.h"
#include <sstream>
#include <iomanip>
#include <functional>

#include <stdint.h>

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
    
    // En caso que no hay  cumplido a�os
    if (fechaActual->tm_mon < fechaNacimiento.tm_mon ||
        (fechaActual->tm_mon == fechaNacimiento.tm_mon && 
        
         fechaActual->tm_mday < fechaNacimiento.tm_mday)) {
         	
        edad--;
    }
    
    return edad;
}

unsigned long long UtilidadesAnalisis::hashToULL(const std::string &input) {
    std::hash<std::string> hasher;
    auto h = hasher(input);
    return static_cast<unsigned long long>(h);
}

std::string UtilidadesAnalisis::hashToHex(const std::string &input) {
    unsigned long long v = hashToULL(input);
    std::ostringstream oss;
    oss << std::hex << std::uppercase << v;
    return oss.str();
}


