#include "UtilidadesAnalisis.h"
#include "EstructuraElectoral.h"
#include "DatosElectoral.h"
#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <regex>
#include <sstream>
#include <cctype>
#include <ctime>
#include <functional>
#include <iomanip>

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

// =========================
// Modificacion
// =========================

// Método auxiliar para leer una línea específica con ID
static bool leerLineaConID(const std::string& rutaArchivo, 
                          const std::string& identificacionBuscada, 
                          std::string& lineaEncontrada,
                          size_t& numeroLinea) {
    std::ifstream archivo(rutaArchivo);
    if (!archivo.is_open()) {
        return false;
    }
    
    std::string linea;
    numeroLinea = 0;
    
    while (std::getline(archivo, linea)) {
        numeroLinea++;
        
        if (linea.empty()) continue;
        
        // Dividir línea para buscar ID en columna 3
        std::vector<std::string> campos;
        size_t inicio = 0;
        size_t fin = linea.find(',');
        int campoActual = 0;
        
        // Solo necesitamos llegar al campo 3 (ID)
        while (fin != std::string::npos && campoActual < 4) {
            std::string campo = linea.substr(inicio, fin - inicio);
            campos.push_back(campo);
            inicio = fin + 1;
            fin = linea.find(',', inicio);
            campoActual++;
        }
        
        // Si llegamos al campo 3
        if (campoActual >= 3 && campos.size() >= 4) {
            std::string idActual = campos[3]; // Columna 3 = ID
            
            if (idActual == identificacionBuscada) {
                lineaEncontrada = linea;
                archivo.close();
                return true;
            }
        }
    }
    
    archivo.close();
    return false;
}

// Método para modificar un candidato en el archivo (versión optimizada)
bool UtilidadesAnalisis::modificarCandidatoEnArchivo(const std::string& rutaArchivo, 
                                                    const std::string& identificacionBuscada,
                                                    const std::vector<int>& columnasModificar,
                                                    const std::vector<std::string>& nuevosValores) {
    // Validar que haya el mismo número de columnas que valores
    if (columnasModificar.size() != nuevosValores.size()) {
        std::cerr << "Error: El número de columnas a modificar no coincide con el número de valores nuevos." << std::endl;
        return false;
    }
    
    // Validar columnas permitidas (1, 2, 4, 5)
    for (int columna : columnasModificar) {
        if (columna != 1 && columna != 2 && columna != 4 && columna != 5) {
            std::cerr << "Error: Columna " << columna << " no permitida para modificación." << std::endl;
            std::cerr << "Columnas permitidas: 1 (nombre), 2 (apellido), 4 (sexo), 5 (estadoCivil)" << std::endl;
            return false;
        }
    }
    
    // Primero, buscar la línea específica con el ID
    std::string lineaOriginal;
    size_t numeroLinea;
    
    if (!leerLineaConID(rutaArchivo, identificacionBuscada, lineaOriginal, numeroLinea)) {
        std::cerr << "Error: No se encontró candidato con identificación " << identificacionBuscada << std::endl;
        return false;
    }
    
    // Dividir la línea original en campos
    std::vector<std::string> campos;
    std::string campo;
    size_t inicio = 0;
    size_t fin = lineaOriginal.find(',');
    
    while (fin != std::string::npos) {
        campo = lineaOriginal.substr(inicio, fin - inicio);
        campos.push_back(campo);
        inicio = fin + 1;
        fin = lineaOriginal.find(',', inicio);
    }
    
    // Último campo
    campo = lineaOriginal.substr(inicio);
    campos.push_back(campo);
    
    // Verificar campos suficientes
    if (campos.size() < 12) {
        std::cerr << "Error: Línea no tiene suficientes campos." << std::endl;
        return false;
    }
    
    // Verificar que el candidato no esté "eliminado"
    if (campos[0] == "false") {
        std::cerr << "Error: No se puede modificar un candidato marcado como false (eliminado)." << std::endl;
        return false;
    }
    
    // Modificar las columnas especificadas
    std::cout << "Modificando candidato " << identificacionBuscada << ":" << std::endl;
    for (size_t i = 0; i < columnasModificar.size(); i++) {
        int columna = columnasModificar[i];
        if (columna >= 0 && columna < (int)campos.size()) {
            std::string valorAnterior = campos[columna];
            campos[columna] = nuevosValores[i];
            std::cout << "  Columna " << columna << ": '" << valorAnterior 
                     << "' -> '" << nuevosValores[i] << "'" << std::endl;
        } else {
            std::cerr << "Error: Columna " << columna << " fuera de rango." << std::endl;
            return false;
        }
    }
    
    // Reconstruir la línea modificada
    std::string lineaModificada;
    for (size_t i = 0; i < campos.size(); i++) {
        if (i > 0) lineaModificada += ",";
        lineaModificada += campos[i];
    }
    
    // Leer todo el archivo y reemplazar solo la línea modificada
    std::ifstream archivoEntrada(rutaArchivo);
    if (!archivoEntrada.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo para lectura." << std::endl;
        return false;
    }
    
    std::vector<std::string> lineas;
    std::string linea;
    size_t lineaActual = 0;
    
    while (std::getline(archivoEntrada, linea)) {
        lineaActual++;
        if (lineaActual == numeroLinea) {
            // Reemplazar esta línea con la modificada
            lineas.push_back(lineaModificada);
        } else {
            lineas.push_back(linea);
        }
    }
    
    archivoEntrada.close();
    
    // Escribir el archivo actualizado
    std::ofstream archivoSalida(rutaArchivo);
    if (!archivoSalida.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo para escritura." << std::endl;
        return false;
    }
    
    for (const auto& l : lineas) {
        archivoSalida << l << std::endl;
    }
    
    archivoSalida.close();
    std::cout << "Candidato modificado exitosamente." << std::endl;
    return true;
}

// Método para "eliminar" un candidato (versión optimizada)
bool UtilidadesAnalisis::eliminarCandidatoDelArchivo(const std::string& rutaArchivo,
                                                    const std::string& identificacionBuscada) {
    // Primero, buscar la línea específica con el ID
    std::string lineaOriginal;
    size_t numeroLinea;
    
    if (!leerLineaConID(rutaArchivo, identificacionBuscada, lineaOriginal, numeroLinea)) {
        std::cerr << "Error: No se encontró candidato con identificación " << identificacionBuscada << std::endl;
        return false;
    }
    
    // Dividir la línea original para verificar
    std::vector<std::string> campos;
    size_t inicio = 0;
    size_t fin = lineaOriginal.find(',');
    
    while (fin != std::string::npos) {
        std::string campo = lineaOriginal.substr(inicio, fin - inicio);
        campos.push_back(campo);
        inicio = fin + 1;
        fin = lineaOriginal.find(',', inicio);
    }
    
    // Último campo
    std::string campo = lineaOriginal.substr(inicio);
    campos.push_back(campo);
    
    if (campos.size() < 12) {
        std::cerr << "Error: Línea no tiene suficientes campos." << std::endl;
        return false;
    }
    
    std::string nombreCandidato = campos[1] + " " + campos[2];
    
    // Verificar si ya está marcado como false
    if (campos[0] == "false") {
        std::cerr << "Error: El candidato " << nombreCandidato 
                 << " ya está marcado como eliminado (false)." << std::endl;
        return false;
    }
    
    // Cambiar true por false
    campos[0] = "false";
    
    // Reconstruir la línea modificada
    std::string lineaModificada;
    for (size_t i = 0; i < campos.size(); i++) {
        if (i > 0) lineaModificada += ",";
        lineaModificada += campos[i];
    }
    
    // Leer todo el archivo y reemplazar solo la línea modificada
    std::ifstream archivoEntrada(rutaArchivo);
    if (!archivoEntrada.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo para lectura." << std::endl;
        return false;
    }
    
    std::vector<std::string> lineas;
    std::string linea;
    size_t lineaActual = 0;
    
    while (std::getline(archivoEntrada, linea)) {
        lineaActual++;
        if (lineaActual == numeroLinea) {
            // Reemplazar esta línea con la modificada
            lineas.push_back(lineaModificada);
            std::cout << "Marcando como eliminado: " << nombreCandidato 
                     << " (ID: " << identificacionBuscada << ")" << std::endl;
        } else {
            lineas.push_back(linea);
        }
    }
    
    archivoEntrada.close();
    
    // Escribir el archivo actualizado
    std::ofstream archivoSalida(rutaArchivo);
    if (!archivoSalida.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo para escritura." << std::endl;
        return false;
    }
    
    for (const auto& l : lineas) {
        archivoSalida << l << std::endl;
    }
    
    archivoSalida.close();
    std::cout << "Candidato " << nombreCandidato << " marcado como eliminado (false)." << std::endl;
    return true;
}

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

        // Parsear la fecha usando sscanf
        int year, month, day;
        if (sscanf(fechaStr.c_str(), "%d-%d-%d", &year, &month, &day) != 3) {
            std::cout << "Fecha invalida.\n";
            continue;
        }

        // Asignar a la estructura tm
        fechaNacimiento.tm_year = year - 1900;
        fechaNacimiento.tm_mon = month - 1;
        fechaNacimiento.tm_mday = day;

        // Validar que la fecha es v�lida (d�as del mes, etc.)
        // Podemos hacer una validaci�n b�sica
        if (month < 1 || month > 12 || day < 1 || day > 31) {
            std::cout << "Fecha invalida.\n";
            continue;
        }

        // validar edad coherente
        std::time_t t = std::time(nullptr);
        std::tm* now = std::localtime(&t);

        int edad = now->tm_year + 1900 - year;
        if ((now->tm_mon + 1 < month) ||
           (now->tm_mon + 1 == month && now->tm_mday < day)) {
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
        
        if (candidato == nullptr || candidato->persiste == false)
            continue;
        
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
        
        if (presidente == nullptr)
            continue;

        std::string partidoNombre = "Sin partido";
        if (presidente->partido == nullptr || presidente->persiste == false || !(presidente->vicepresidente->persiste) || presidente->vicepresidente == nullptr)
            continue;
        
        partidoNombre = presidente->partido->nombre;
        // Acortar si es muy largo
        if (partidoNombre.length() > ANCHO_PARTIDO) {
            partidoNombre = partidoNombre.substr(0, ANCHO_PARTIDO - 3) + "...";
        }

        std::string presidenteNombre = presidente->nombre + " " + presidente->apellido;
        if (presidenteNombre.length() > ANCHO_PRESIDENTE) {
            presidenteNombre = presidenteNombre.substr(0, ANCHO_PRESIDENTE - 3) + "...";
        }
        
        std::string viceNombre;
        Candidato* vicepresidente = presidente->vicepresidente;
        viceNombre = vicepresidente->nombre + " " + vicepresidente->apellido;
        viceNombre = " | VP: " + viceNombre;
        
        // Formatear con columnas alineadas
        std::cout << "  " << opcion << ". " 
                    << std::setw(ANCHO_PARTIDO) << std::left << partidoNombre << " "
                    << std::setw(ANCHO_PRESIDENTE) << std::left << presidenteNombre
                    << viceNombre << std::endl;
        opcion++;
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
            if(!c->persiste)
                continue;                
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
