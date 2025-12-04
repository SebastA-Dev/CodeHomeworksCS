#include "CargarDatos.h"
#include <fstream>
#include <sstream>
#include <iostream>

// Divide la linea por la "," en cada archivo 
std::vector<std::string> CargarDatos::dividirLinea(const std::string& linea, char delimitador) {
    std::vector<std::string> resultado;
    std::stringstream ss(linea);
    std::string item;
    
    while (std::getline(ss, item, delimitador)) {
        resultado.push_back(item);
    }
    
    return resultado;
}

// Parsear fecha formato DD/MM/YYYY
std::tm CargarDatos::parsearFecha(const std::string& fecha) {
    std::tm result = {};
    int dia, mes, anio;
    sscanf(fecha.c_str(), "%d/%d/%d", &dia, &mes, &anio);
    
    result.tm_mday = dia;
    result.tm_mon = mes - 1;
    result.tm_year = anio - 1900; 
    
    return result;
}

// ------------------------------------------------------------------------
// CARGAR PARTIDOS.TXT
// ------------------------------------------------------------------------

bool CargarDatos::cargarPartidos(DatosElectoral& sistema, const std::string& ruta) {
    std::ifstream archivo(ruta);
    
    if (!archivo.is_open()) {
        std::cerr << "? No se pudo abrir: " << ruta << std::endl;
        return false;
    }
    
    std::string linea;
    int contador = 0;
    
    while (std::getline(archivo, linea)) {
        if (linea.empty()) continue;  // Ignorar líneas vacías
        
        auto datos = dividirLinea(linea, ',');
        
        // Validar que tenga los 3 campos necesarios
        if (datos.size() < 3) {
            std::cerr << "??  Línea incompleta en Partidos.txt: " << linea << std::endl;
            continue;
        }
        
        // Extraer datos
        std::string nombre = datos[0];
        std::string representante = datos[1];
        bool legal = (datos[2] == "1");  // 1 = legal, 0 = ilegal
        
        // Crear partido
        Partido* p = sistema.crearPartido(nombre, representante, legal);
        
        if (p) {
            contador++;
        } else {
            std::cerr << "??  Error al crear partido: " << nombre << std::endl;
        }
    }
    
    archivo.close();
    std::cout << "? Cargados " << contador << " partidos" << std::endl;
    return contador > 0;
}

// ------------------------------------------------------------------------
// CARGAR CIUDADES.TXT
// ------------------------------------------------------------------------

bool CargarDatos::cargarCiudades(DatosElectoral& sistema, const std::string& ruta) {
    std::ifstream archivo(ruta);
    
    if (!archivo.is_open()) {
        std::cerr << "? No se pudo abrir: " << ruta << std::endl;
        return false;
    }
    
    std::string linea;
    int contador = 0;
    
    // Primero, crear todas las regiones si no existen
    
    while (std::getline(archivo, linea)) {
        if (linea.empty()) continue;
        
        auto datos = dividirLinea(linea, ',');
        
        if (datos.size() < 3) {
            std::cerr << "??  Línea incompleta en Ciudades.txt: " << linea << std::endl;
            continue;
        }
        
        std::string nombreCiudad = datos[0];
        std::string nombreRegion = datos[1];
        int censo = std::stoi(datos[2]);

        // Se asume que las regiones ya existen
        
        Region* region = nullptr;  // Buscar en sistema
        
        if (!region) {
            region = sistema.crearRegion(nombreRegion, {}, nullptr);
        }
        
        // Crear ciudad
        Ciudad* ciudad = sistema.crearCiudad(nombreCiudad, region, {});
        
        if (ciudad) {
            ciudad->censoElectoral = censo;
            contador++;
        }
    }
    
    archivo.close();
    std::cout << "? Cargadas " << contador << " ciudades" << std::endl;
    return contador > 0;
}

// ------------------------------------------------------------------------
// CARGAR CANDIDATOS.TXT
// ------------------------------------------------------------------------

bool CargarDatos::cargarCandidatos(DatosElectoral& sistema, const std::string& ruta) {
    std::ifstream archivo(ruta);
    
    if (!archivo.is_open()) {
        std::cerr << "? No se pudo abrir: " << ruta << std::endl;
        return false;
    }
    
    std::string linea;
    int contador = 0;
    
    while (std::getline(archivo, linea)) {
        if (linea.empty()) continue;
        
        auto datos = dividirLinea(linea, ',');
        
        // Recordar el tipo de formato según los requerimientos: nombre,apellido,id,sexo,estadoCivil,fecha,ciudadNac,ciudadRes,partido,tipo,ciudadAspirante
        
        if (datos.size() < 11) {
            std::cerr << "??  Línea incompleta en Candidatos.txt" << std::endl;
            continue;
        }
        
        // Extraer datos
        std::string nombre = datos[0];
        std::string apellido = datos[1];
        std::string identificacion = datos[2];
    
        Sexo sexo = (datos[3] == "M") ? Sexo::Masculino : Sexo::Femenino;
  
        EstadoCivil estadoCivil;
        std::string ec = datos[4];
        if (ec == "Soltero") estadoCivil = EstadoCivil::Soltero;
        else if (ec == "Casado") estadoCivil = EstadoCivil::Casado;
        else if (ec == "Divorciado") estadoCivil = EstadoCivil::Divorciado;
        else estadoCivil = EstadoCivil::UnionLibre;
        
        // Parsear fecha
        std::tm fechaNacimiento = parsearFecha(datos[5]);
        
        // Buscar ciudades por nombre
        Ciudad* ciudadNacimiento = nullptr;
        Ciudad* ciudadResidencia = nullptr;
        
        // Buscar partido por nombre
        std::shared_ptr<Partido> partido = nullptr;
        
        //tipo de candidato
        TipoCandidato tipo;
        std::string tipoStr = datos[9];
        if (tipoStr == "ALCALDE") tipo = TipoCandidato::ALCALDE;
        else if (tipoStr == "PRESIDENTE") tipo = TipoCandidato::PRESIDENTE;
        else tipo = TipoCandidato::VICEPRESIDENTE;
        
        //crear candidato
        Candidato* candidato = sistema.crearCandidato(
            nombre, apellido, identificacion, sexo, estadoCivil,
            fechaNacimiento, ciudadNacimiento, ciudadResidencia,
            partido, tipo, nullptr, nullptr
        );
        
        if (candidato) {
            contador++;
        }
    }
    
    archivo.close();
    std::cout << "? Cargados " << contador << " candidatos" << std::endl;
    return contador > 0;
}

// ------------------------------------------------------------------------
// FUNCIÓN PRINCIPAL DE CARGAR DATOS
// ------------------------------------------------------------------------

bool CargarDatos::cargarTodosLosDatos(
    DatosElectoral& sistema,
    const std::string& rutaPartidos,
    const std::string& rutaCiudades,
    const std::string& rutaCandidatos) {
    
    std::cout << "\n+----------------------------------------+" << std::endl;
    std::cout << "¦  CARGANDO DATOS DEL SISTEMA ELECTORAL  ¦" << std::endl;
    std::cout << "+----------------------------------------+\n" << std::endl;
    
    bool ok = true;
    
    std::cout << "1. Cargando partidos..." << std::endl;
    ok = ok && cargarPartidos(sistema, rutaPartidos);
    
    std::cout << "2. Cargando ciudades..." << std::endl;
    ok = ok && cargarCiudades(sistema, rutaCiudades);
    
    std::cout << "3. Cargando candidatos..." << std::endl;
    ok = ok && cargarCandidatos(sistema, rutaCandidatos);
    
    if (ok) {
        std::cout << "\n? DATOS CARGADOS EXITOSAMENTE\n" << std::endl;
    } else {
        std::cout << "\n??  ALGUNOS DATOS NO SE CARGARON CORRECTAMENTE\n" << std::endl;
    }
    
    return ok;
}

