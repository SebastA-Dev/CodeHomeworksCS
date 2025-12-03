#ifndef UTILIDADES_ANALISIS_H
#define UTILIDADES_ANALISIS_H

#include <string>

class UtilidadesAnalisis {
public:    
    static std::string stringToHex(const std::string &input);
    static int calcularEdad(const std::tm& fechaNacimiento); //sE AGREGA MÉTODO PARA CALCULAR LA EDAD
    
    static int busquedaBinariaString(const std::vector<std::string>& elementos, const std::string& buscado);  //Método para búsqueda binaria
};
    
};

#endif // UTILIDADES_ANALISIS_H

