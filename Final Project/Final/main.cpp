#include "DatosElectoral.h"
#include "CargarDatos.h"
#include "UtilidadesAnalisis.h"
#include "SimulacionElectoral.h"
#include <iostream>
#include <limits>
#include <ctime>
#include <cstdlib>

// ============================================================================
// FUNCIONES AUXILIARES DE SELECCIÓN
// ============================================================================

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
    
    std::cout << "\n=== SELECCIONAR REGIÓN ===" << std::endl;
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

// ============================================================================
// PARSERS PARA DATOS
// ============================================================================

std::tm parsearFecha(const std::string& fechaStr) {
    std::tm fecha = {};
    int year, month, day;
    if (sscanf(fechaStr.c_str(), "%d-%d-%d", &year, &month, &day) != 3) {
        std::cerr << "Formato de fecha inválido. Use YYYY-MM-DD" << std::endl;
        return fecha;
    }
    fecha.tm_year = year - 1900;
    fecha.tm_mon = month - 1;
    fecha.tm_mday = day;
    return fecha;
}

Sexo parsearSexo(char opcion) {
    return (opcion == 'M' || opcion == 'm') ? Sexo::Masculino : Sexo::Femenino;
}

EstadoCivil parsearEstadoCivil(int opcion) {
    switch(opcion) {
        case 1: return EstadoCivil::Soltero;
        case 2: return EstadoCivil::Casado;
        case 3: return EstadoCivil::Divorciado;
        case 4: return EstadoCivil::UnionLibre;
        default: return EstadoCivil::Soltero;
    }
}

TipoCandidato parsearTipoCandidato(int opcion) {
    switch(opcion) {
        case 1: return TipoCandidato::ALCALDE;
        case 2: return TipoCandidato::PRESIDENTE;
        case 3: return TipoCandidato::VICEPRESIDENTE;
        default: return TipoCandidato::ALCALDE;
    }
}

// ============================================================================
// GESTIÓN DE CANDIDATOS
// ============================================================================

void agregarCandidato(DatosElectoral& sistema) {
    std::string nombre, apellido, identificacion;
    char sexoChar;
    int estadoCivilOpt, tipoCandidatoOpt;
    std::string fechaStr;
    
    std::cout << "\n=== AGREGAR CANDIDATO ===" << std::endl;
    
    std::cout << "Nombre: ";
    std::getline(std::cin, nombre);
    
    std::cout << "Apellido: ";
    std::getline(std::cin, apellido);
    
    std::cout << "Identificación: ";
    std::getline(std::cin, identificacion);
    
    std::cout << "Sexo (M/F): ";
    std::cin >> sexoChar;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    Sexo sexo = parsearSexo(sexoChar);
    
    std::cout << "\nEstado Civil:\n1. Soltero\n2. Casado\n3. Divorciado\n4. Unión Libre\nOpción: ";
    std::cin >> estadoCivilOpt;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    EstadoCivil estadoCivil = parsearEstadoCivil(estadoCivilOpt);
    
    std::cout << "Fecha de Nacimiento (YYYY-MM-DD): ";
    std::getline(std::cin, fechaStr);
    std::tm fechaNacimiento = parsearFecha(fechaStr);
    
    Ciudad* ciudadNacimiento = seleccionarCiudad(sistema);
    if (!ciudadNacimiento) return;
    
    Ciudad* ciudadResidencia = seleccionarCiudad(sistema);
    if (!ciudadResidencia) return;
    
    Partido* partido = seleccionarPartido(sistema);
    if (!partido) return;
    
    std::cout << "\nTipo de Candidato:\n1. Alcalde\n2. Presidente\n3. Vicepresidente\nOpción: ";
    std::cin >> tipoCandidatoOpt;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    TipoCandidato tipo = parsearTipoCandidato(tipoCandidatoOpt);
    
    Ciudad* ciudadAspirante = nullptr;
    if (tipo == TipoCandidato::ALCALDE) {
        std::cout << "\nSeleccione ciudad donde aspira a alcaldía:" << std::endl;
        ciudadAspirante = seleccionarCiudad(sistema);
        if (!ciudadAspirante) return;
    }
    
    Candidato* candidato = sistema.crearCandidato(
        nombre, apellido, identificacion, sexo, estadoCivil, fechaNacimiento,
        ciudadNacimiento, ciudadResidencia, partido, tipo, ciudadAspirante, nullptr
    );
    
    if (candidato) {
        std::cout << "\n? Candidato agregado exitosamente." << std::endl;
    } else {
        std::cout << "\n? Error al agregar candidato." << std::endl;
    }
}

void eliminarCandidato(DatosElectoral& sistema) {
    std::cout << "\n=== ELIMINAR CANDIDATO ===" << std::endl;
    std::cout << "Identificación del candidato a eliminar: ";
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
        std::cout << "\n? Candidato eliminado." << std::endl;
    } else {
        std::cout << "\n? Candidato no encontrado." << std::endl;
    }
}

void modificarCandidato(DatosElectoral& sistema) {
    std::cout << "\n=== MODIFICAR CANDIDATO ===" << std::endl;
    std::cout << "Identificación del candidato: ";
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
        std::cout << "\n? Candidato no encontrado." << std::endl;
    }
    
    std::cout << "\n¿Qué modificar?\n1. Nombre\n2. Apellido\n3. Sexo\n4. Estado Civil\n5. Volver\nOpción: ";
    int opcion;
    std::cin >> opcion;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    switch(opcion) {
        case 1:
            std::cout << "Nuevo nombre: ";
            std::getline(std::cin, candidato->nombre);
            std::cout << "? Actualizado." << std::endl;
            break;
        case 2:
            std::cout << "Nuevo apellido: ";
            std::getline(std::cin, candidato->apellido);
            std::cout << "? Actualizado." << std::endl;
            break;
        case 3: {
            char sexoChar;
            std::cout << "Nuevo sexo (M/F): ";
            std::cin >> sexoChar;
            candidato->sexo = parsearSexo(sexoChar);
            std::cout << "? Actualizado." << std::endl;
            break;
        }
        case 4: {
            int estadoOpt;
            std::cout << "Nuevo estado civil (1.Soltero 2.Casado 3.Divorciado 4.Unión Libre): ";
            std::cin >> estadoOpt;
            candidato->estadoCivil = parsearEstadoCivil(estadoOpt);
            std::cout << "? Actualizado." << std::endl;
            break;
        }
        case 5:
            break;
        default:
            std::cout << "Opción no válida." << std::endl;
    }
}

void menuGestionCandidatos(DatosElectoral& sistema) {
    int opcion = -1;
    
    while (opcion != 0) {
        std::cout << "\n";
        std::cout << "+----------------------------------------+" << std::endl;
        std::cout << "¦  MENÚ DE GESTIÓN DE CANDIDATOS         ¦" << std::endl;
        std::cout << "¦----------------------------------------¦" << std::endl;
        std::cout << "¦  1. Agregar candidato                  ¦" << std::endl;
        std::cout << "¦  2. Eliminar candidato                 ¦" << std::endl;
        std::cout << "¦  3. Modificar candidato                ¦" << std::endl;
        std::cout << "¦  4. Listar candidatos                  ¦" << std::endl;
        std::cout << "¦  0. Volver                             ¦" << std::endl;
        std::cout << "+----------------------------------------+" << std::endl;
        std::cout << "\nSeleccione opción: ";
        std::cin >> opcion;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        switch (opcion) {
            case 1:
                agregarCandidato(sistema);
                break;
            case 2:
                eliminarCandidato(sistema);
                break;
            case 3:
                modificarCandidato(sistema);
                break;
            case 4: {
                auto& cand = sistema.obtenerListaCandidatos();
                std::cout << "\n=== CANDIDATOS A ALCALDÍA ===" << std::endl;
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
                std::cout << "Opción no válida." << std::endl;
        }
    }
}

// ============================================================================
// MENÚ DE CONSULTAS
// ============================================================================

void menuConsultas(DatosElectoral& sistema, Pais* pais) {
    int opcion = -1;
    
    while (opcion != 0) {
        std::cout << "\n";
        std::cout << "+----------------------------------------+" << std::endl;
        std::cout << "¦      MENÚ DE CONSULTAS                 ¦" << std::endl;
        std::cout << "¦----------------------------------------¦" << std::endl;
        std::cout << "¦  1. Ciudades electorales               ¦" << std::endl;
        std::cout << "¦  2. Partidos legales                   ¦" << std::endl;
        std::cout << "¦  3. Candidatos por ciudad              ¦" << std::endl;
        std::cout << "¦  4. Candidatos presidenciales          ¦" << std::endl;
        std::cout << "¦  5. Tarjetón de alcaldía               ¦" << std::endl;
        std::cout << "¦  6. Tarjetón de presidencia            ¦" << std::endl;
        std::cout << "¦  7. Censo electoral                    ¦" << std::endl;
        std::cout << "¦  0. Volver                             ¦" << std::endl;
        std::cout << "+----------------------------------------+" << std::endl;
        std::cout << "\nSeleccione opción: ";
        std::cin >> opcion;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        switch (opcion) {
            case 1: {
                auto ciudades = sistema.obtenerCiudadesElectorales();
                std::cout << "\n=== CIUDADES ELECTORALES ===" << std::endl;
                for (auto c : ciudades) {
                    std::cout << "- " << c->nombre << " (Región: " << c->region->nombre << ")" << std::endl;
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
                std::cout << "Opción no válida." << std::endl;
        }
    }
}

// ============================================================================
// MENÚ PRINCIPAL
// ============================================================================

void menuPrincipal(DatosElectoral& sistema, Pais* pais) {
    int opcion = -1;
    
    while (opcion != 0) {
        std::cout << "\n";
        std::cout << "+----------------------------------------+" << std::endl;
        std::cout << "¦  SISTEMA ELECTORAL - MENÚ PRINCIPAL    ¦" << std::endl;
        std::cout << "¦----------------------------------------¦" << std::endl;
        std::cout << "¦  1. Consultas                          ¦" << std::endl;
        std::cout << "¦  2. Gestión de Candidatos              ¦" << std::endl;
        std::cout << "¦  3. Simulación de Elecciones (próximo) ¦" << std::endl;
        std::cout << "¦  0. Salir                              ¦" << std::endl;
        std::cout << "+----------------------------------------+" << std::endl;
        std::cout << "\nSeleccione opción: ";
        std::cin >> opcion;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        switch (opcion) {
            case 1:
                menuConsultas(sistema, pais);
                break;
            case 2:
                menuGestionCandidatos(sistema);
                break;
            case 3: {
    			std::cout << "\n¿Esta seguro de que desea iniciar la simulacion?" << std::endl;
    			std::cout << "(No podra agregar/eliminar candidatos despues) [S/N]: ";
    			char confirm;
    			std::cin >> confirm;
    			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    			if (confirm == 'S' || confirm == 's') {
        			SimulacionElectoral::simularElecciones(sistema, pais);
    			}
    			break;
}
            case 0:
                std::cout << "\n¡Hasta luego!" << std::endl;
                break;
            default:
                std::cout << "Opción no válida." << std::endl;
        }
    }
}

// ============================================================================
// MAIN
// ============================================================================

int main() {
    DatosElectoral sistema;
    
    Pais* pais = new Pais();
    pais->nombre = "Colombia";
    pais->codigo = "COL_001";
    sistema.obtenerListaPaises().push_back(pais);
    
    bool ok = CargarDatos::cargarTodosLosDatos(
        sistema,
        "Partidos.txt",
        "Regiones.txt",
        "Ciudades.txt",
        "Candidatos.txt"
    );
    
    if (!ok) {
        std::cerr << "Error al cargar datos." << std::endl;
        delete pais;
        return 1;
    }
    
    auto& regiones = sistema.obtenerListaRegiones();
    for (auto region : regiones) {
        region->censoElectoral = 0;
        for (auto ciudad : region->ciudades) {
            region->censoElectoral += ciudad->censoElectoral;
        }
    }
    
    menuPrincipal(sistema, pais);
    
    return 0;
}
