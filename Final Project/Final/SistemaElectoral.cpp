#include "SistemaElectoral.h"
#include "UtilidadesAnalisis.h"
#include <iostream>
#include <string>
#include <limits>
#include <regex>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <cctype>


// Todas las funciones fueron copiadas desde main.cpp respetando la logica original.
// Se colocaron dentro del namespace SistemaElectoral.

namespace SistemaElectoral {

// =========================
// SELECCION
// =========================

Ciudad* seleccionarCiudad(DatosElectoral& sistema) {
    auto& ciudades = sistema.obtenerListaCiudades();
    if (ciudades.empty()) {
        std::cout << "No hay ciudades registradas." << std::endl;
        return nullptr;
    }
    
    std::cout << "\n=== SELECCIONAR CIUDAD ===" << std::endl;
    for (int i = 0; i < ciudades.size(); i++) {
        std::cout << i + 1 << ". " << ciudades[i]->nombre << std::endl;
    }
    
    int opcion;
    std::cout << "Seleccione (0 para cancelar): ";
    std::cin >> opcion;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    if (opcion < 1 || opcion > ciudades.size()) {
        return nullptr;
    }
    
    return ciudades[opcion - 1];
}

Partido* seleccionarPartido(DatosElectoral& sistema) {
    auto& partidos = sistema.obtenerListaPartidos();
    if (partidos.empty()) {
        std::cout << "No hay partidos registrados." << std::endl;
        return nullptr;
    }
    
    std::cout << "\n=== SELECCIONAR PARTIDO ===" << std::endl;
    for (int i = 0; i < partidos.size(); i++) {
        std::cout << i + 1 << ". " << partidos[i]->nombre << std::endl;
    }
    
    int opcion;
    std::cout << "Seleccione (0 para cancelar): ";
    std::cin >> opcion;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    if (opcion < 1 || opcion > partidos.size()) {
        return nullptr;
    }
    
    return partidos[opcion - 1];
}

Region* seleccionarRegion(DatosElectoral& sistema) {
    auto& regiones = sistema.obtenerListaRegiones();
    if (regiones.empty()) {
        std::cout << "No hay regiones registradas." << std::endl;
        return nullptr;
    }
    
    std::cout << "\n=== SELECCIONAR REGION ===" << std::endl;
    for (int i = 0; i < regiones.size(); i++) {
        std::cout << i + 1 << ". " << regiones[i]->nombre << std::endl;
    }
    
    int opcion;
    std::cout << "Seleccione (0 para cancelar): ";
    std::cin >> opcion;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    if (opcion < 1 || opcion > regiones.size()) {
        return nullptr;
    }
    
    return regiones[opcion - 1];
}

// =========================
// GESTION DE CANDIDATOS
// =========================

void agregarCandidato(DatosElectoral& sistema, Pais* pais) {
    std::string nombre, apellido, identificacion;

    std::cout << "\n=== AGREGAR CANDIDATO ===" << std::endl;

    std::cout << "Nombre: ";
    std::getline(std::cin, nombre);

    std::cout << "Apellido: ";
    std::getline(std::cin, apellido);

    std::cout << "Identificacion: ";
    std::getline(std::cin, identificacion);

    // ============================
    // VALIDAR SEXO (en utilidadesAnalisis)
    // ============================
    char sexoChar = UtilidadesAnalisis::leerSexo();
    Sexo sexo = UtilidadesAnalisis::parsearSexo(sexoChar);

    // ============================
    // VALIDAR ESTADO CIVIL (en utilidadesAnalisis)
    // ============================
    int estadoCivilOpt = UtilidadesAnalisis::leerEstadoCivil();
    EstadoCivil estadoCivil = UtilidadesAnalisis::parsearEstadoCivil(estadoCivilOpt);

    // ============================
    // VALIDAR FECHA DE NACIMIENTO (en utilidadesAnalisis)
    // ============================
    std::tm fechaNacimiento = UtilidadesAnalisis::leerFechaNacimiento();

    // ============================
    // SELECCIONAR CIUDADES Y PARTIDO
    // ============================
    Ciudad* ciudadNacimiento = seleccionarCiudad(sistema);
    if (!ciudadNacimiento) return;

    Ciudad* ciudadResidencia = seleccionarCiudad(sistema);
    if (!ciudadResidencia) return;

    Partido* partido = seleccionarPartido(sistema);
    if (!partido) return;

    // ============================
    // VALIDAR TIPO DE CANDIDATO (en utilidadesAnalisis::validarOpcion)
    // ============================
    int tipoCandidatoOpt = UtilidadesAnalisis::leerTipoCandidato();
    TipoCandidato tipo = UtilidadesAnalisis::parsearTipoCandidato(tipoCandidatoOpt);

    Ciudad* ciudadAspirante = nullptr;
    if (tipo == TipoCandidato::ALCALDE) {
        std::cout << "\nSeleccione ciudad donde aspira a la alcadia:\n";
        ciudadAspirante = seleccionarCiudad(sistema);
        if (!ciudadAspirante) return;
    }

    // ============================
    // CREAR CANDIDATO
    // ============================
    Candidato* candidato = sistema.crearCandidato(
        nombre, apellido, identificacion, sexo, estadoCivil,
        fechaNacimiento, ciudadNacimiento, ciudadResidencia,
        partido, tipo, pais, ciudadAspirante, nullptr
    );

    if (candidato) {
        std::cout << "\n> Candidato agregado exitosamente." << std::endl;
    } else {
        std::cout << "\n> Error al agregar candidato." << std::endl;
    }
}


void eliminarCandidato(DatosElectoral& sistema) {
    std::cout << "\n=== ELIMINAR CANDIDATO ===" << std::endl;
    std::cout << "Identificacion del candidato a eliminar: ";
    std::string identificacion;
    std::getline(std::cin, identificacion);
    
    auto& candidatosAlcaldia = sistema.obtenerListaCandidatos();
    
    bool encontrado = false;
    for (auto it = candidatosAlcaldia.begin(); it != candidatosAlcaldia.end(); ++it) {
        if ((*it)->identificacion == identificacion) {
            Candidato* cand = *it;
            if (cand->ciudadAspirante) {
                for (auto it2 = cand->ciudadAspirante->candidatosAlcaldia.begin(); 
                     it2 != cand->ciudadAspirante->candidatosAlcaldia.end(); ++it2) {
                    if (*it2 == cand) {
                        cand->ciudadAspirante->candidatosAlcaldia.erase(it2);
                        break;
                    }
                }
            }
            delete cand;
            candidatosAlcaldia.erase(it);
            encontrado = true;
            break;
        }
    }
    
    if (encontrado) {
        std::cout << "\n> Candidato eliminado." << std::endl;
    } else {
        std::cout << "\n> Candidato no encontrado." << std::endl;
    }
}

void modificarCandidato(DatosElectoral& sistema) {
    std::cout << "\n=== MODIFICAR CANDIDATO ===" << std::endl;
    std::cout << "Identificacion del candidato: ";
    std::string identificacion;
    std::getline(std::cin, identificacion);
    
    Candidato* candidato = nullptr;
    auto& candidatosAlcaldia = sistema.obtenerListaCandidatos();
    for (auto c : candidatosAlcaldia) {
        if (c->identificacion == identificacion) {
            candidato = c;
            break;
        }
    }
    
    if (!candidato) {
        std::cout << "\n> Candidato no encontrado." << std::endl;
        return;
    }
    
    std::cout << "\nQue desea modificar?\n1. Nombre\n2. Apellido\n3. Sexo\n4. Estado Civil\n5. Volver\nOpcion: ";
    int opcion;
    std::cin >> opcion;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    switch(opcion) {
        case 1:
            std::cout << "Nuevo nombre: ";
            std::getline(std::cin, candidato->nombre);
            std::cout << "> Actualizado." << std::endl;
            break;
        case 2:
            std::cout << "Nuevo apellido: ";
            std::getline(std::cin, candidato->apellido);
            std::cout << "> Actualizado." << std::endl;
            break;
        case 3: {            
            std::cout << "Nuevo sexo (M/F): ";
            char sexoChar = UtilidadesAnalisis::leerSexo();                    
            candidato->sexo = UtilidadesAnalisis::parsearSexo(sexoChar);;
            std::cout << "> Actualizado." << std::endl;
            break;
        }
        case 4: {            
            std::cout << "Nuevo estado civil (1.Soltero 2.Casado 3.Divorciado 4.Union Libre): ";
            int opc = UtilidadesAnalisis::leerEstadoCivil();
            candidato->estadoCivil = UtilidadesAnalisis::parsearEstadoCivil(opc);
            std::cout << "> Actualizado." << std::endl;
            break;
        }
        case 5:
            break;
        default:
            std::cout << "Opcion no valida." << std::endl;
    }
}

void menuGestionCandidatos(DatosElectoral& sistema, Pais* pais) {
    int opcion = -1;
    
    while (opcion != 0) {
        std::cout << "\n";
        std::cout << "+----------------------------------------+" << std::endl;
        std::cout << "|  MENU DE GESTION DE CANDIDATOS         |" << std::endl;
        std::cout << "|----------------------------------------|" << std::endl;
        std::cout << "|  1. Agregar candidato                  |" << std::endl;
        std::cout << "|  2. Eliminar candidato                 |" << std::endl;
        std::cout << "|  3. Modificar candidato                |" << std::endl;
        std::cout << "|  4. Listar candidatos                  |" << std::endl;
        std::cout << "|  0. Volver                             |" << std::endl;
        std::cout << "+----------------------------------------+" << std::endl;
        std::cout << "\nSeleccione opcion: ";
        std::cin >> opcion;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        switch (opcion) {
            case 1:
                agregarCandidato(sistema, pais);
                break;
            case 2:
                eliminarCandidato(sistema);
                break;
            case 3:
                modificarCandidato(sistema);
                break;
            case 4: {
                auto& cand = sistema.obtenerListaCandidatos();
                std::cout << "\n=== CANDIDATOS A ALCALDIA ===" << std::endl;
                for (auto c : cand) {
                    std::cout << "- " << c->nombre << " " << c->apellido 
                              << " (ID: " << c->identificacion << ")" << std::endl;
                }
                break;
            }
            case 0:
                std::cout << "Volviendo..." << std::endl;
                break;
            default:
                std::cout << "Opcion no valida." << std::endl;
        }
    }
}

// =========================
// MENU DE CONSULTAS
// =========================

void menuConsultas(DatosElectoral& sistema, Pais* pais) {
    int opcion = -1;
    
    while (opcion != 0) {
        std::cout << "\n";
        std::cout << "+----------------------------------------+" << std::endl;
        std::cout << "|      MENU DE CONSULTAS                 |" << std::endl;
        std::cout << "|----------------------------------------|" << std::endl;
        std::cout << "|  1. Ciudades electorales               |" << std::endl;
        std::cout << "|  2. Partidos legales                   |" << std::endl;
        std::cout << "|  3. Candidatos por ciudad              |" << std::endl;
        std::cout << "|  4. Candidatos presidenciales          |" << std::endl;
        std::cout << "|  5. Tarjeton de alcaldia               |" << std::endl;
        std::cout << "|  6. Tarjeton de presidencia            |" << std::endl;
        std::cout << "|  7. Censo electoral                    |" << std::endl;
        std::cout << "|  0. Volver                             |" << std::endl;
        std::cout << "+----------------------------------------+" << std::endl;
        std::cout << "\nSeleccione opcion: ";
        std::cin >> opcion;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        switch (opcion) {
            case 1: {
                auto ciudades = sistema.obtenerCiudadesElectorales();
                std::cout << "\n=== CIUDADES ELECTORALES ===" << std::endl;
                for (auto c : ciudades) {
                    std::cout << "- " << c->nombre << " (Region: " << c->region->nombre << ")" << std::endl;
                }
                break;
            }
            
            case 2: {
                auto partidos = sistema.obtenerPartidosLegales();
                std::cout << "\n=== PARTIDOS LEGALES ===" << std::endl;
                for (auto p : partidos) {
                    std::cout << "- " << p->nombre << std::endl;
                }
                break;
            }
            
            case 3: {
                Ciudad* ciudad = seleccionarCiudad(sistema);
                if (ciudad) {
                    std::cout << "\n=== CANDIDATOS EN " << ciudad->nombre << " ===" << std::endl;
                    for (auto c : ciudad->candidatosAlcaldia) {
                        int edad = UtilidadesAnalisis::calcularEdad(c->fechaNacimiento);
                        std::string sexo = (c->sexo == Sexo::Masculino) ? "M" : "F";
                        std::cout << "- " << c->nombre << " " << c->apellido 
                                  << " (Edad: " << edad << ", Sexo: " << sexo << ")" << std::endl;
                    }
                }
                break;
            }
            
            case 4: {
                auto candidatos = sistema.candidatosPresidenciales();
                std::cout << "\n=== CANDIDATOS PRESIDENCIALES ===" << std::endl;
                for (const auto& par : candidatos) {
                    auto pres = par.first;
                    auto vice = par.second;
                    std::cout << "- " << pres->nombre << " - " << vice->nombre << std::endl;
                }
                break;
            }
            
            case 5: {
                Ciudad* ciudad = seleccionarCiudad(sistema);
                if (ciudad) {
                    UtilidadesAnalisis::imprimirTarjetonAlcaldia(ciudad);
                }
                break;
            }
            
            case 6: {
                UtilidadesAnalisis::imprimirTarjetonPresidencia(pais);
                break;
            }
            
            case 7: {
                auto& ciudades = sistema.obtenerListaCiudades();
                UtilidadesAnalisis::imprimirCensoElectoral(ciudades);
                break;
            }
            
            case 0:
                std::cout << "Volviendo..." << std::endl;
                break;
            default:
                std::cout << "Opcion no valida." << std::endl;
        }
    }
}

} 
