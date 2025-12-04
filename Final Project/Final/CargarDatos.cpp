#include "CargarDatos.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

std::vector<std::string> CargarDatos::dividirLinea(const std::string& linea, char delimitador) {
    std::vector<std::string> resultado;
    std::stringstream ss(linea);
    std::string item;
    
    while (std::getline(ss, item, delimitador)) {
        resultado.push_back(item);
    }
    
    return resultado;
}

// Elimina espacios en blanco al inicio y final
static std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, last - first + 1);
}

std::tm CargarDatos::parsearFecha(const std::string& fecha) {
    std::tm result = {};
    int dia, mes, anio;
    sscanf(fecha.c_str(), "%d/%d/%d", &dia, &mes, &anio);
    
    result.tm_mday = dia;
    result.tm_mon = mes - 1;
    result.tm_year = anio - 1900; 
    
    return result;
}

bool CargarDatos::cargarPartidos(DatosElectoral& sistema, const std::string& ruta) {
    std::ifstream archivo(ruta);
    
    if (!archivo.is_open()) {
        std::cerr << "No se pudo abrir: " << ruta << std::endl;
        return false;
    }
    
    std::string linea;
    int contador = 0;
    
    while (std::getline(archivo, linea)) {
        linea = trim(linea);
        if (linea.empty()) continue;
        
        auto datos = dividirLinea(linea, ',');
        
        if (datos.size() < 3) continue;
        
        std::string nombre = trim(datos[0]);
        std::string representante = trim(datos[1]);
        bool legal = (trim(datos[2]) == "1");
        
        Partido* p = sistema.crearPartido(nombre, representante, legal);
        
        if (p) contador++;
    }
    
    archivo.close();
    std::cout << "Cargados " << contador << " partidos" << std::endl;
    return contador > 0;
}

bool CargarDatos::cargarRegiones(DatosElectoral& sistema, const std::string& ruta, Pais* pais) {
    std::ifstream archivo(ruta);
    
    if (!archivo.is_open()) {
        std::cerr << "No se pudo abrir: " << ruta << std::endl;
        return false;
    }
    
    std::string linea;
    int contador = 0;
    
    while (std::getline(archivo, linea)) {
        linea = trim(linea);
        if (linea.empty()) continue;
        
        size_t posColon = linea.find(':');
        if (posColon == std::string::npos) continue;
        
        std::string nombreRegion = trim(linea.substr(0, posColon));
        
        Region* region = sistema.crearRegion(nombreRegion, {}, pais);
        
        if (region) contador++;
    }
    
    archivo.close();
    std::cout << "Cargadas " << contador << " regiones" << std::endl;
    return contador > 0;
}

bool CargarDatos::cargarCiudades(DatosElectoral& sistema, const std::string& ruta) {
    std::ifstream archivo(ruta);
    
    if (!archivo.is_open()) {
        std::cerr << "No se pudo abrir: " << ruta << std::endl;
        return false;
    }
    
    std::string linea;
    int contador = 0;
    
    while (std::getline(archivo, linea)) {
        linea = trim(linea);
        if (linea.empty()) continue;
        
        auto datos = dividirLinea(linea, ',');
        
        if (datos.size() < 3) continue;
        
        std::string nombreCiudad = trim(datos[0]);
        std::string nombreRegion = trim(datos[1]);
        int censo = std::stoi(trim(datos[2]));
        
        Region* region = nullptr;
        auto& regiones = sistema.obtenerListaRegiones();
        
        for (auto r : regiones) {
            if (r->nombre == nombreRegion) {
                region = r;
                break;
            }
        }
        
        if (!region) {
            std::cerr << "Región no encontrada: " << nombreRegion << std::endl;
            continue;
        }
        
        Ciudad* ciudad = sistema.crearCiudad(nombreCiudad, region, {});
        
        if (ciudad) {
            ciudad->censoElectoral = censo;
            region->ciudades.push_back(ciudad);
            contador++;
        }
    }
    
    archivo.close();
    std::cout << "Cargadas " << contador << " ciudades" << std::endl;
    return contador > 0;
}

bool CargarDatos::cargarCandidatos(DatosElectoral& sistema, const std::string& ruta) {
    std::ifstream archivo(ruta);
    
    if (!archivo.is_open()) {
        std::cerr << "No se pudo abrir: " << ruta << std::endl;
        return false;
    }
    
    std::string linea;
    int contador = 0;
    int linea_num = 0;
    
    while (std::getline(archivo, linea)) {
        linea_num++;
        linea = trim(linea);
        if (linea.empty()) continue;
        
        auto datos = dividirLinea(linea, ',');
        
        if (datos.size() < 10) {
            std::cerr << "Linea " << linea_num << ": Datos insuficientes (" << datos.size() << ")" << std::endl;
            continue;
        }
        
        std::string nombre = trim(datos[0]);
        std::string apellido = trim(datos[1]);
        std::string identificacion = trim(datos[2]);
        
        if (nombre.empty() || apellido.empty() || identificacion.empty()) {
            std::cerr << "Linea " << linea_num << ": Datos personales vacios" << std::endl;
            continue;
        }
        
        Sexo sexo = (trim(datos[3]) == "M") ? Sexo::Masculino : Sexo::Femenino;
        
        EstadoCivil estadoCivil;
        std::string ec = trim(datos[4]);
        if (ec == "Soltero") estadoCivil = EstadoCivil::Soltero;
        else if (ec == "Casado") estadoCivil = EstadoCivil::Casado;
        else if (ec == "Divorciado") estadoCivil = EstadoCivil::Divorciado;
        else estadoCivil = EstadoCivil::UnionLibre;
        
        std::tm fechaNacimiento = parsearFecha(trim(datos[5]));
        
        Ciudad* ciudadNacimiento = nullptr;
        Ciudad* ciudadResidencia = nullptr;
        
        auto& ciudades = sistema.obtenerListaCiudades();
        
        for (auto c : ciudades) {
            if (c && c->nombre == trim(datos[6])) {
                ciudadNacimiento = c;
            }
            if (c && c->nombre == trim(datos[7])) {
                ciudadResidencia = c;
            }
        }
        
        if (!ciudadNacimiento || !ciudadResidencia) {
            std::cerr << "Linea " << linea_num << ": Ciudades no encontradas para " << nombre << " " << apellido << std::endl;
            continue;
        }
        
        Partido* partido = nullptr;
		auto& partidos = sistema.obtenerListaPartidos();

		for (auto p : partidos) {
    		if (p && p->nombre == trim(datos[8])) {
        		partido = p;
        		break;
    		}
		}

		if (!partido) {
    		std::cerr << "Linea " << linea_num << ": Partido no encontrado: '" << trim(datos[8]) << "'" << std::endl;
    		continue;
		}
        
        TipoCandidato tipo;
        std::string tipoStr = trim(datos[9]);
        if (tipoStr == "ALCALDE") tipo = TipoCandidato::ALCALDE;
        else if (tipoStr == "PRESIDENTE") tipo = TipoCandidato::PRESIDENTE;
        else tipo = TipoCandidato::VICEPRESIDENTE;
        
        Ciudad* ciudadAspirante = nullptr;
        if (tipo == TipoCandidato::ALCALDE && datos.size() > 10) {
            for (auto c : ciudades) {
                if (c && c->nombre == trim(datos[10])) {
                    ciudadAspirante = c;
                    break;
                }
            }
            if (!ciudadAspirante) {
                std::cerr << "Linea " << linea_num << ": Ciudad aspirante no encontrada para alcalde" << std::endl;
                continue;
            }
        }
        
		Candidato* candidato = sistema.crearCandidato(
    		nombre, apellido, identificacion, sexo, estadoCivil,
    		fechaNacimiento, ciudadNacimiento, ciudadResidencia,
    		partido, tipo, ciudadAspirante, nullptr
		);

        
   	    if (candidato) {
            contador++;
            std::cout << "  Candidato creado: " << nombre << " " << apellido << std::endl;
        } else {
            std::cerr << "Linea " << linea_num << ": crearCandidato retorno nullptr para " << nombre << " " << apellido << std::endl;
        }

    }
    
    archivo.close();
    std::cout << "Cargados " << contador << " candidatos" << std::endl;
    return contador > 0;
    
}

bool CargarDatos::cargarTodosLosDatos(
    DatosElectoral& sistema,
    const std::string& rutaPartidos,
    const std::string& rutaRegiones,
    const std::string& rutaCiudades,
    const std::string& rutaCandidatos) {
    
    std::cout << "\n+----------------------------------------+" << std::endl;
    std::cout << "|  CARGANDO DATOS DEL SISTEMA ELECTORAL  |" << std::endl;
    std::cout << "+----------------------------------------+\n" << std::endl;
    
    bool ok = true;
    
    Pais* pais = new Pais();
    pais->nombre = "País";
    pais->codigo = "PAIS_001";
    
    std::cout << "1. Cargando partidos..." << std::endl;
    ok = ok && cargarPartidos(sistema, rutaPartidos);
    
    std::cout << "2. Cargando regiones..." << std::endl;
    ok = ok && cargarRegiones(sistema, rutaRegiones, pais);
    
    std::cout << "3. Cargando ciudades..." << std::endl;
    ok = ok && cargarCiudades(sistema, rutaCiudades);
    
    std::cout << "4. Cargando candidatos..." << std::endl;
    ok = ok && cargarCandidatos(sistema, rutaCandidatos);
    
    if (ok) {
        std::cout << "\nDATOS CARGADOS EXITOSAMENTE\n" << std::endl;
    } else {
        std::cout << "\nALGUNOS DATOS NO SE CARGARON CORRECTAMENTE\n" << std::endl;
    }
    
    return ok;
}
