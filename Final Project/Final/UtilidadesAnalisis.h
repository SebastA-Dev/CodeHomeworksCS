#ifndef UTILIDADES_ANALISIS_H
#define UTILIDADES_ANALISIS_H

#include <string>

class UtilidadesAnalisis {
public:    
    static std::string stringToHex(const std::string &input);
    static int calcularEdad(const std::tm& fechaNacimiento); //sE AGREGA MÉTODO PARA CALCULAR LA EDAD
    
};

#endif // UTILIDADES_ANALISIS_H

