#include "UtilidadesAnalisis.h"
#include "EstructuraElectoral.h"
#include "DatosElectoral.h"
#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <regex>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <ctime>
#include <functional>

std::string UtilidadesAnalisis::stringToHex(const std::string &input) {
    std::ostringstream oss;
    for (char c : input) {        
        oss << std::hex << std::setw(2) << std::setfill('0')
            << static_cast<int>(static_cast<unsigned char>(c));
    }
    return oss.str();
}

int UtilidadesAnalisis::calcularEdad(const std::tm& fechaNacimiento) {
    time_t ahora = std::time(nullptr);
    std::tm* fechaActual = std::localtime(&ahora);
    
    int edad = fechaActual->tm_year - fechaNacimiento.tm_year;
    
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

//Parsers

// =========================
// PARSERS
// =========================

std::tm UtilidadesAnalisis::parsearFecha(const std::string& fechaStr) {
    std::tm fecha = {};
    int year, month, day;
    if (sscanf(fechaStr.c_str(), "%d-%d-%d", &year, &month, &day) != 3) {
        std::cerr << "Formato de fecha invalido. Use YYYY-MM-DD" << std::endl;
        return fecha;
    }
    fecha.tm_year = year - 1900;
    fecha.tm_mon = month - 1;
    fecha.tm_mday = day;
    return fecha;
}

Sexo UtilidadesAnalisis::parsearSexo(char opcion) {
    return (opcion == 'M' || opcion == 'm') ? Sexo::Masculino : Sexo::Femenino;
}

EstadoCivil UtilidadesAnalisis::parsearEstadoCivil(int opcion) {
    switch(opcion) {
        case 1: return EstadoCivil::Soltero;
        case 2: return EstadoCivil::Casado;
        case 3: return EstadoCivil::Divorciado;
        case 4: return EstadoCivil::UnionLibre;
        default: return EstadoCivil::Soltero;
    }
}

EstadoCivil UtilidadesAnalisis::parsearEstadoCivil(std::string opcion) {    
    std::transform(opcion.begin(), opcion.end(), opcion.begin(),
                   [](unsigned char c){ return std::tolower(c); });

    // Eliminar espacios al inicio y final
    opcion.erase(0, opcion.find_first_not_of(" \t\n\r"));
    opcion.erase(opcion.find_last_not_of(" \t\n\r") + 1);

    // Comparaciones textuales
    if (opcion == "soltero")
        return EstadoCivil::Soltero;
    if (opcion == "casado")
        return EstadoCivil::Casado;
    if (opcion == "divorciado")
        return EstadoCivil::Divorciado;
    if (opcion == "union libre" || opcion == "unionlibre" || opcion == "union libre")
        return EstadoCivil::UnionLibre;

    // Valor por defecto
    return EstadoCivil::Soltero;
}

TipoCandidato UtilidadesAnalisis::parsearTipoCandidato(int opcion) {
    switch(opcion) {
        case 1: return TipoCandidato::ALCALDE;
        case 2: return TipoCandidato::PRESIDENTE;
        case 3: return TipoCandidato::VICEPRESIDENTE;
        default: return TipoCandidato::ALCALDE;
    }
}

TipoCandidato UtilidadesAnalisis::parsearTipoCandidato(std::string opcion) {
    // Pasar a minúsculas
    std::transform(opcion.begin(), opcion.end(), opcion.begin(),
                   [](unsigned char c){ return std::tolower(c); });

    if (opcion == "alcalde")
        return TipoCandidato::ALCALDE;
    if (opcion == "presidente")
        return TipoCandidato::PRESIDENTE;
    if (opcion == "vicepresidente")
        return TipoCandidato::VICEPRESIDENTE;
    
    return TipoCandidato::ALCALDE;
}

std::string UtilidadesAnalisis::hashToHex(const std::string &input) {
    unsigned long long v = hashToULL(input);
    std::ostringstream oss;
    oss << std::hex << std::uppercase << v;
    return oss.str();
}

//Validaciones

// ============================
// VALIDAR SEXO
// ============================
char UtilidadesAnalisis::leerSexo() {    
    char sexoChar;

    while (true) {
        std::cout << "Sexo (M/F): ";
        std::cin >> sexoChar;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        sexoChar = static_cast<char>(std::toupper(static_cast<unsigned char>(sexoChar)));

        if (sexoChar == 'M' || sexoChar == 'F')
            break;

        std::cout << "Entrada invalida. Solo se permite 'M' o 'F'.\n";
    }

    return sexoChar;
}

// ============================
// VALIDAR ESTADO CIVIL
// ============================
int UtilidadesAnalisis::leerEstadoCivil() {
    int opc;

    while (true) {
        std::cout << "\nEstado Civil:\n"
                     "1. Soltero\n"
                     "2. Casado\n"
                     "3. Divorciado\n"
                     "4. Union Libre\n"
                     "Opcion: ";

        std::cin >> opc;

        if (!std::cin.fail() && opc >= 1 && opc <= 4) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        }

        std::cout << "Opcion invalida. Ingrese un numero entre 1 y 4.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    return opc;
}

// ============================
// VALIDAR FECHA DE NACIMIENTO
// ============================
std::tm UtilidadesAnalisis::leerFechaNacimiento() {
    std::string fechaStr;
    std::tm fechaNacimiento = {};

    while (true) {
        std::cout << "Fecha de Nacimiento (YYYY-MM-DD): ";
        std::getline(std::cin, fechaStr);

        std::regex formato("^\\d{4}-\\d{2}-\\d{2}$");
        if (!std::regex_match(fechaStr, formato)) {
            std::cout << "Formato invalido. Use YYYY-MM-DD.\n";
            continue;
        }

        std::istringstream ss(fechaStr);
        ss >> std::get_time(&fechaNacimiento, "%Y-%m-%d");

        if (ss.fail()) {
            std::cout << "Fecha invalida.\n";
            continue;
        }

        // validar edad coherente
        std::time_t t = std::time(nullptr);
        std::tm* now = std::localtime(&t);

        int edad = now->tm_year + 1900 - (fechaNacimiento.tm_year + 1900);
        if ((now->tm_mon < fechaNacimiento.tm_mon) ||
           (now->tm_mon == fechaNacimiento.tm_mon && now->tm_mday < fechaNacimiento.tm_mday)) {
            edad--;
        }

        if (edad < 18 || edad > 100) {
            std::cout << "Edad no valida (" << edad << " anos). Debe ser entre 18 y 100.\n";
            continue;
        }

        break;
    }

    return fechaNacimiento;
}

// ============================
// VALIDAR TIPO DE CANDIDATO
// ============================
int UtilidadesAnalisis::leerTipoCandidato() {
    int opc;

    while (true) {
        std::cout << "\nTipo de Candidato:\n"
                     "1. Alcalde\n"
                     "2. Presidente\n"
                     "3. Vicepresidente\n"
                     "Opcion: ";

        std::cin >> opc;

        if (!std::cin.fail() && opc >= 1 && opc <= 3) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        }

        std::cout << "Opcion invalida. Ingrese un numero entre 1 y 3.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    return opc;
}

// ============================================================================
// FUNCIONES DE IMPRESION
// ============================================================================

void UtilidadesAnalisis::imprimirTarjetonAlcaldia(Ciudad* ciudad) {
    if (!ciudad) {
        std::cerr << "Ciudad no valida" << std::endl;
        return;
    }
    
    std::cout << "\n";
    std::cout << "+======================================================================+" << std::endl;
    std::cout << "        TARJETON DE ALCALDIA - " 
              << std::setw(25) << std::left << ciudad->nombre << " " << std::endl;
    std::cout << "========================================================================" << std::endl;
    std::cout << "  0. VOTO EN BLANCO" << std::endl;
    std::cout << "------------------------------------------------------------------------" << std::endl;
    
    int opcion = 1;
    
    // Columnas fijas para mejor alineación
    const int ANCHO_PARTIDO = 30;      // Para nombres de partidos largos
    const int ANCHO_CANDIDATO = 25;    // Para nombres de candidatos
    
    for (size_t i = 0; i < ciudad->candidatosAlcaldia.size(); i++) {
        Candidato* candidato = ciudad->candidatosAlcaldia[i];
        
        if (candidato != nullptr) {
            std::string partidoNombre = "Sin partido";
            if (candidato->partido != nullptr) {
                partidoNombre = candidato->partido->nombre;
            }
            
            std::string candidatoNombre = candidato->nombre + " " + candidato->apellido;
            
            // Asegurar que los nombres no sean muy largos
            if (partidoNombre.length() > ANCHO_PARTIDO) {
                partidoNombre = partidoNombre.substr(0, ANCHO_PARTIDO - 3) + "...";
            }
            
            if (candidatoNombre.length() > ANCHO_CANDIDATO) {
                candidatoNombre = candidatoNombre.substr(0, ANCHO_CANDIDATO - 3) + "...";
            }
            
            // Imprimir con columnas alineadas
            std::cout << "  " << opcion << ". " 
                      << std::setw(ANCHO_PARTIDO) << std::left << partidoNombre
                      << std::setw(ANCHO_CANDIDATO) << std::left <<" - "<<candidatoNombre << std::endl;
            opcion++;
        }
    }
    
    std::cout << "+======================================================================+" << std::endl;
}

void UtilidadesAnalisis::imprimirTarjetonPresidencia(Pais* pais) {
    if (!pais) {
        std::cerr << "Pais no valido" << std::endl;
        return;
    }
    
    std::cout << "\n";
    std::cout << "+======================================================================+" << std::endl;
    std::cout << "        TARJETON DE PRESIDENCIA - " 
              << std::setw(25) << std::left << pais->nombre << " " << std::endl;
    std::cout << "========================================================================" << std::endl;
    std::cout << "  0. VOTO EN BLANCO" << std::endl;
    std::cout << "------------------------------------------------------------------------" << std::endl;
    
    int opcion = 1;
    
    // Columnas con anchos fijos optimizados
    const int ANCHO_PARTIDO = 30;
    const int ANCHO_PRESIDENTE = 25;
    
    for (size_t i = 0; i < pais->candidatosPresidencia.size(); i++) {
        Candidato* presidente = pais->candidatosPresidencia[i];
        
        if (presidente != nullptr) {
            std::string partidoNombre = "Sin partido";
            if (presidente->partido != nullptr) {
                partidoNombre = presidente->partido->nombre;
                // Acortar si es muy largo
                if (partidoNombre.length() > ANCHO_PARTIDO) {
                    partidoNombre = partidoNombre.substr(0, ANCHO_PARTIDO - 3) + "...";
                }
            }
            
            std::string presidenteNombre = presidente->nombre + " " + presidente->apellido;
            if (presidenteNombre.length() > ANCHO_PRESIDENTE) {
                presidenteNombre = presidenteNombre.substr(0, ANCHO_PRESIDENTE - 3) + "...";
            }
            
            std::string viceNombre;
            if (presidente->vicepresidente != nullptr) {
                Candidato* vicepresidente = presidente->vicepresidente;
                viceNombre = vicepresidente->nombre + " " + vicepresidente->apellido;
                viceNombre = " | VP: " + viceNombre;
            } else {
                viceNombre = " | Sin VP";
            }
            
            // Formatear con columnas alineadas
            std::cout << "  " << opcion << ". " 
                      << std::setw(ANCHO_PARTIDO) << std::left << partidoNombre << " "
                      << std::setw(ANCHO_PRESIDENTE) << std::left << presidenteNombre
                      << viceNombre << std::endl;
            opcion++;
        }
    }
    
    std::cout << "+======================================================================+" << std::endl;
}

void UtilidadesAnalisis::imprimirCensoElectoral(const std::vector<Ciudad*>& ciudades) {
    std::cout << "\n";
    std::cout << "+--------------------------------------------+" << std::endl;
    std::cout << "       CENSO ELECTORAL POR CIUDAD           " << std::endl;
    std::cout << "--------------------------------------------" << std::endl;
    
    long totalNacional = 0;
    
    for (auto ciudad : ciudades) {
        std::cout << "  " << std::setw(26) << std::left << ciudad->nombre 
                  << "  " << std::setw(11) << std::right << ciudad->censoElectoral << std::endl;
        totalNacional += ciudad->censoElectoral;
    }
    
    std::cout << "-----------------------------+--------------" << std::endl;
    std::cout << "  " << std::setw(26) << std::left << "TOTAL NACIONAL" 
              << "  " << std::setw(11) << std::right << totalNacional << std::endl;
    std::cout << "+--------------------------------------------+" << std::endl;
}

void UtilidadesAnalisis::imprimirCandidatosPartidoRegion(
    const std::vector<Candidato*>& candidatos, Partido* partido, Region* region) {
    
    std::cout << "\n=== CANDIDATOS A ALCALDIA - " << partido->nombre 
              << " EN " << region->nombre << " ===" << std::endl;
    
    if (candidatos.empty()) {
        std::cout << "No hay candidatos de este partido en esta region." << std::endl;
    } else {
        for (auto c : candidatos) {
            int edad = calcularEdad(c->fechaNacimiento);
            std::string sexo = (c->sexo == Sexo::Masculino) ? "M" : "F";
            std::cout << "- " << c->nombre << " " << c->apellido 
                      << " (Edad: " << edad << ", Sexo: " << sexo 
                      << ", Ciudad: " << c->ciudadAspirante->nombre << ")" << std::endl;
        }
    }
}

void UtilidadesAnalisis::imprimirCandidatosPartidoTodasCiudades(
    const std::vector<std::tuple<Ciudad*, std::string, Sexo, int>>& candidatos, Partido* partido) {
    
    std::cout << "\n=== CANDIDATOS A ALCALDIA - " << partido->nombre 
              << " EN TODAS LAS CIUDADES ===" << std::endl;
    
    if (candidatos.empty()) {
        std::cout << "No hay candidatos de este partido." << std::endl;
    } else {
        for (const auto& item : candidatos) {
            auto ciudad = std::get<0>(item);
            auto nombre = std::get<1>(item);
            auto sexo = std::get<2>(item);
            auto edad = std::get<3>(item);
        
            std::string sexoStr = (sexo == Sexo::Masculino) ? "M" : "F";
            std::cout << "- " << ciudad->nombre << ": " << nombre 
                      << " (Edad: " << edad << ", Sexo: " << sexoStr << ")" << std::endl;
        }
    }
}

void UtilidadesAnalisis::imprimirCandidatosPorCiudadPorPartido(
    const std::vector<std::pair<Partido*, Candidato*>>& candidatos, Ciudad* ciudad) {
    
    std::cout << "\n=== CANDIDATOS A ALCALDIA POR PARTIDO EN " << ciudad->nombre << " ===" << std::endl;
    
    if (candidatos.empty()) {
        std::cout << "No hay candidatos registrados en esta ciudad." << std::endl;
    } else {
        for (const auto& item : candidatos) {
            auto partido = item.first;
            auto candidato = item.second;
        
            int edad = calcularEdad(candidato->fechaNacimiento);
            std::string sexo = (candidato->sexo == Sexo::Masculino) ? "M" : "F";
            std::cout << "- " << partido->nombre << ": " << candidato->nombre << " " 
                      << candidato->apellido << " (Edad: " << edad << ", Sexo: " << sexo << ")" << std::endl;
        }
    }
}