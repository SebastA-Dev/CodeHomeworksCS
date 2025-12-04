// CargarDatos.h
#ifndef CARGAR_DATOS_H
#define CARGAR_DATOS_H

#include "DatosElectoral.h"
#include <string>
#include <vector>

class CargarDatos {
public:
    static bool cargarTodosLosDatos(DatosElectoral& sistema, const std::string& rutaPartidos, const std::string& rutaRegiones, const std::string& rutaCiudades, const std::string& rutaCandidatos);
    
private:
    static bool cargarPartidos(DatosElectoral& sistema, const std::string& ruta);
    static bool cargarCiudades(DatosElectoral& sistema, const std::string& ruta);
    static bool cargarCandidatos(DatosElectoral& sistema, const std::string& ruta);
    static bool cargarRegiones(DatosElectoral& sistema, const std::string& ruta, Pais* pais);
    
    static std::vector<std::string> dividirLinea(const std::string& linea, char delimitador = ',');
    static std::tm parsearFecha(const std::string& fecha);
};

#endif

