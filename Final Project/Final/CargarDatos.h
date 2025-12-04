#ifndef CARGAR_DATOS_H
#define CARGAR_DATOS_H

#include "DatosElectoral.h"
#include <string>
#include <vector>

class CargarDatos {
public:
	
    // Función  que carga tods los datos
    
    static bool cargarTodosLosDatos(
        DatosElectoral& sistema,
        const std::string& rutaPartidos,
        const std::string& rutaCiudades,
        const std::string& rutaCandidatos
    );
    
private:
	
    // Funciones para cada archivo
    static bool cargarPartidos(DatosElectoral& sistema, const std::string& ruta);
    static bool cargarCiudades(DatosElectoral& sistema, const std::string& ruta);
    static bool cargarCandidatos(DatosElectoral& sistema, const std::string& ruta);
    
    // Utilidades
    static std::vector<std::string> dividirLinea(const std::string& linea, char delimitador = ',');
    static std::tm parsearFecha(const std::string& fecha);
};

#endif 
