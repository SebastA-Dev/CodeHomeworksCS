#include "CargarDatos.h"
#include "UtilidadesAnalisis.h"
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
        bool legal = (trim(datos[2]) == "true");
        
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
            std::cerr << "Regi�n no encontrada: " << nombreRegion << std::endl;
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

std::vector<Candidato*> presidentes;
bool CargarDatos::cargarCandidatos(DatosElectoral& sistema, const std::string& ruta, Pais* pais) {
    auto& ciudades = sistema.obtenerListaCiudades();
    auto& partidos = sistema.obtenerListaPartidos();
    
    std::vector<Candidato*> presidentesLocales;

    auto buscarCiudad = [&](const std::string& nombre) -> Ciudad* {
        std::string buscado = trim(nombre);
        for (auto c : ciudades) {
            if (c && c->nombre == buscado) {
                return c;
            }
        }
        return nullptr;
    };

    auto buscarPartido = [&](const std::string& nombre) -> Partido* {
        std::string buscado = trim(nombre);
        for (auto c : partidos) {
            if (c && c->nombre == buscado) {
                return c;
            }
        }
        return nullptr;
    };

    auto buscarPresidente = [&](const Partido* partido, const std::vector<Candidato*>& listaPresidentes) -> Candidato* {            
        for (auto c : listaPresidentes) {
            if (c && c->partido == partido) {
                return c;
            }
        }
        return nullptr;
    };

    std::ifstream archivo(ruta);

    if (!archivo.is_open()) {
        std::cerr << "No se pudo abrir: " << ruta << std::endl;
        return false;
    }

    std::string linea;
    int cargados = 0;

    while (std::getline(archivo, linea)) {
        if (linea.empty())
            continue;

        std::vector<std::string> campos;
        std::string campo;
        
        // Separar manualmente por comas
        size_t inicio = 0;
        size_t fin = linea.find(',');
        
        while (fin != std::string::npos) {
            campo = linea.substr(inicio, fin - inicio);
            campos.push_back(trim(campo));
            inicio = fin + 1;
            fin = linea.find(',', inicio);
        }
        
        // Último campo después de la última coma
        campo = linea.substr(inicio);
        campos.push_back(trim(campo));

        // Verificar que tenemos los campos mínimos
        if (campos.size() < 10) {
            continue;
        }

        // Extraer campos en el orden correcto
        std::string nombre          = campos[0];
        std::string apellido        = campos[1];
        std::string codigo          = campos[2];
        
        char sexoChar = campos[3].empty() ? ' ' : campos[3][0];
        Sexo sexo = UtilidadesAnalisis::parsearSexo(sexoChar);
        
        EstadoCivil estadoCivil = UtilidadesAnalisis::parsearEstadoCivil(campos[4]);
        
        std::tm fechaNac = parsearFecha(campos[5]);
        
        Ciudad* ciudadNac = buscarCiudad(campos[6]);
        Ciudad* ciudadRes = buscarCiudad(campos[7]);
        Partido* partido = buscarPartido(campos[8]);
        
        TipoCandidato cargo = UtilidadesAnalisis::parsearTipoCandidato(campos[9]);
        
        Candidato* candidato = nullptr;

        // Validaciones básicas
        if (!ciudadNac || !ciudadRes || !partido) {
            continue;
        }

        if (cargo == TipoCandidato::ALCALDE && campos.size() > 10) {
            Ciudad* ciudadAlcalde = buscarCiudad(campos[10]);
            if (!ciudadAlcalde) {
                continue;
            }
            
            candidato = sistema.crearCandidato(
                nombre, apellido, codigo, sexo, estadoCivil, fechaNac,
                ciudadNac, ciudadRes, partido, cargo,pais, ciudadAlcalde, nullptr
            );
        } else if (cargo == TipoCandidato::PRESIDENTE) {
            // Verificar si ya existe un presidente para este partido
            Candidato* presidenteExistente = buscarPresidente(partido, presidentesLocales);
            if (presidenteExistente) {
                continue;
            }
            
            candidato = sistema.crearCandidato(
                nombre, apellido, codigo, sexo, estadoCivil, fechaNac,
                ciudadNac, ciudadRes, partido, cargo, pais,nullptr, nullptr
            );
            
            if (candidato) {
                presidentesLocales.push_back(candidato);
            }
        } else if (cargo == TipoCandidato::VICEPRESIDENTE) {
            Candidato* presidente = buscarPresidente(partido, presidentesLocales);
            if (!presidente) {
                continue;
            }
            
            Candidato* vic = new Candidato();
            vic->nombre = nombre;
            vic->apellido = apellido;
            vic->identificacion = codigo;
            vic->sexo = sexo;
            vic->estadoCivil = estadoCivil;
            vic->fechaNacimiento = fechaNac;
            vic->ciudadNacimiento = ciudadNac;
            vic->ciudadResidencia = ciudadNac;
            vic->partido = partido;
            vic->tipo = cargo;
            
            if (presidente) {
                presidente->vicepresidente = vic;
            }
        }
        
        if (candidato != nullptr) {
            cargados++;
        }
    }

    std::cout << "Cargados " << cargados << " candidatos\n";
    return cargados > 0;
}


bool CargarDatos::cargarTodosLosDatos(
    DatosElectoral& sistema,
    const std::string& rutaPartidos,
    const std::string& rutaRegiones,
    const std::string& rutaCiudades,
    const std::string& rutaCandidatos,
    Pais* pais) {
    
    std::cout << "\n+----------------------------------------+" << std::endl;
    std::cout << "|  CARGANDO DATOS DEL SISTEMA ELECTORAL  |" << std::endl;
    std::cout << "+----------------------------------------+\n" << std::endl;
    
    bool ok = true;
    
    std::cout << "1. Cargando partidos..." << std::endl;
    ok = ok && cargarPartidos(sistema, rutaPartidos);
    
    std::cout << "2. Cargando regiones..." << std::endl;
    ok = ok && cargarRegiones(sistema, rutaRegiones, pais);
    
    std::cout << "3. Cargando ciudades..." << std::endl;
    ok = ok && cargarCiudades(sistema, rutaCiudades);
    
    std::cout << "4. Cargando candidatos..." << std::endl;
    ok = ok && cargarCandidatos(sistema, rutaCandidatos, pais);
    
    if (ok) {
        std::cout << "\nDATOS CARGADOS EXITOSAMENTE\n" << std::endl;
    } else {
        std::cout << "\nALGUNOS DATOS NO SE CARGARON CORRECTAMENTE\n" << std::endl;
    }
    
    return ok;
}
