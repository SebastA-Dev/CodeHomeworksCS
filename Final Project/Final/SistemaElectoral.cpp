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

    std::cout << "\n";
    std::cout << "+======================================================================+" << std::endl;
    std::cout << "        CIUDAD DE NACIMIENTO" << std::endl;
    std::cout << "+======================================================================+" << std::endl;
    Ciudad* ciudadNacimiento = seleccionarCiudad(sistema);
    if (!ciudadNacimiento) return;

    std::cout << "\n";
    std::cout << "+======================================================================+" << std::endl;
    std::cout << "        CIUDAD DE RESIDENCIA" << std::endl;
    std::cout << "+======================================================================+" << std::endl;
    Ciudad* ciudadResidencia = seleccionarCiudad(sistema);    
    if (!ciudadResidencia) return;

    Partido* partido = seleccionarPartido(sistema);
    if (!partido) return;

    // ============================
    // VALIDAR TIPO DE CANDIDATO (en utilidadesAnalisis::validarOpcion)
    // ============================
    int tipoCandidatoOpt = UtilidadesAnalisis::leerTipoCandidato();
    TipoCandidato tipo = UtilidadesAnalisis::parsearTipoCandidato(tipoCandidatoOpt);

    // Debido a que el Alcalde tiene que ser si o si ser residente de la ciudad, se establece directamente
    Ciudad* ciudadAspirante = nullptr;
    if (tipo == TipoCandidato::ALCALDE) {        
        ciudadAspirante = ciudadResidencia;        
    }

    // ============================
    // CREAR CANDIDATO
    // ============================
    Candidato* candidato = sistema.crearCandidato(true,
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
    
    UtilidadesAnalisis::eliminarCandidatoDelArchivo(".Candidatos.txt",identificacion);

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
        std::cout << "+------------------------------------------------+" << std::endl;
        std::cout << "|           MENU DE CONSULTAS                   |" << std::endl;
        std::cout << "+------------------------------------------------+" << std::endl;
        std::cout << "|  1. Ciudades electorales                      |" << std::endl;
        std::cout << "|  2. Partidos legales                          |" << std::endl;
        std::cout << "|  3. Candidatos por ciudad                     |" << std::endl;
        std::cout << "|  4. Candidatos presidenciales                 |" << std::endl;
        std::cout << "|  5. Tarjeton de alcaldia                      |" << std::endl;
        std::cout << "|  6. Tarjeton de presidencia                   |" << std::endl;
        std::cout << "|  7. Censo electoral                           |" << std::endl;
        std::cout << "|  8. Candidatos a alcaldia por partido y region|" << std::endl;
        std::cout << "|  9. Candidatos a alcaldia por partido         |" << std::endl;
        std::cout << "|     (todas las ciudades)                      |" << std::endl;
        std::cout << "| 10. Candidatos por ciudad por partido         |" << std::endl;
        std::cout << "|  0. Volver                                    |" << std::endl;
        std::cout << "+------------------------------------------------+" << std::endl;
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
                        if(!c->persiste)
                            continue;
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
                    if(!pres->persiste)
                        continue;
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
            
            case 8: {
                // ============================================
                // CANDIDATOS A ALCALDIA POR PARTIDO Y REGIÓN
                // ============================================
                
                std::cout << "\n";
                std::cout << "+===================================================+" << std::endl;
                std::cout << "|  CANDIDATOS A ALCALDIA POR PARTIDO Y REGIÓN     |" << std::endl;
                std::cout << "+===================================================+" << std::endl;
                
                // 1. Obtener y mostrar todas las regiones
                std::cout << "\n--- REGIONES DISPONIBLES ---" << std::endl;
                                
                // Obtener regiones directamente del sistema
                auto regiones = sistema.obtenerListaRegiones();

                if (regiones.empty()) {
                    std::cout << "No hay regiones disponibles." << std::endl;
                    break;
                }

                // Mostrar regiones numeradas
                std::cout << "\n--- REGIONES DISPONIBLES ---" << std::endl;
                for (size_t i = 0; i < regiones.size(); i++) {
                    std::cout << "  " << i + 1 << ". " << regiones[i]->nombre 
                            << " (" << regiones[i]->ciudades.size() << " ciudades)" << std::endl;
                }

                // Seleccionar región
                int opcionRegion;
                std::cout << "\nSeleccione una región (1-" << regiones.size() << "): ";
                std::cin >> opcionRegion;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                if (opcionRegion < 1 || opcionRegion > static_cast<int>(regiones.size())) {
                    std::cout << "Selección inválida. Operación cancelada." << std::endl;
                    break;
                }
                
                Region* regionSeleccionada = regiones[opcionRegion - 1];
                
                // 2. Obtener y mostrar todos los partidos
                std::cout << "\n--- PARTIDOS LEGALES ---" << std::endl;
                auto partidos = sistema.obtenerPartidosLegales();
                
                if (partidos.empty()) {
                    std::cout << "No hay partidos legales registrados." << std::endl;
                    break;
                }
                
                for (size_t i = 0; i < partidos.size(); i++) {
                    std::cout << "  " << i + 1 << ". " << partidos[i]->nombre << std::endl;
                }
                
                // Seleccionar partido
                int opcionPartido;
                std::cout << "\nSeleccione un partido (1-" << partidos.size() << "): ";
                std::cin >> opcionPartido;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                
                if (opcionPartido < 1 || opcionPartido > static_cast<int>(partidos.size())) {
                    std::cout << "Selección inválida. Operación cancelada." << std::endl;
                    break;
                }
                
                Partido* partidoSeleccionado = partidos[opcionPartido - 1];
                
                // 3. Obtener candidatos usando el método
                std::cout << "\n";
                std::cout << "+===================================================+" << std::endl;
                std::cout << "|            CANDIDATOS ALCALDIA P/R                |" << std::endl;
                std::cout << "|  REGION: " << std::setw(36) << std::left << regionSeleccionada->nombre << "|" << std::endl;
                std::cout << "|  PARTIDO: " << std::setw(35) << std::left << partidoSeleccionado->nombre << "|" << std::endl;
                std::cout << "+===================================================+" << std::endl;
                
                // Llamar al método que implementaste
               auto resultados = sistema.candidatosAlcaldiaPorPartidoYRegion(partidoSeleccionado, regionSeleccionada);

                if (resultados.empty()) {
                    std::cout << "\nNo hay candidatos de " << partidoSeleccionado->nombre 
                            << " en " << regionSeleccionada->nombre << std::endl;
                    break;
                }

                for (size_t i = 0; i < resultados.size(); i++) {
                    Ciudad* ciudad = std::get<0>(resultados[i]);  // Extraer ciudad de la tupla
                    Candidato* cand = std::get<1>(resultados[i]); // Extraer candidato de la tupla
                    
                    int edad = UtilidadesAnalisis::calcularEdad(cand->fechaNacimiento);
                    std::string sexo = (cand->sexo == Sexo::Masculino) ? "M" : "F";
                    std::string ciudadNombre = ciudad ? ciudad->nombre : "N/A";
                    
                    std::cout << i + 1 << ". " << cand->nombre << " " << cand->apellido << std::endl;
                    std::cout << "   Ciudad: " << ciudadNombre 
                            << " | Edad: " << edad 
                            << " | Sexo: " << sexo << std::endl;
                }

                std::cout << "\nTotal: " << resultados.size() << " candidato(s)" << std::endl;
                break;
            }

            case 9: {
                auto partidos = sistema.obtenerPartidosLegales();
                
                if (partidos.empty()) {
                    std::cout << "\nNo hay partidos legales registrados." << std::endl;
                    break;
                }
                
                std::cout << "\n+------------------------------------------------+" << std::endl;
                std::cout << "|      CANDIDATOS A ALCALDIA POR PARTIDO        |" << std::endl;
                std::cout << "+------------------------------------------------+" << std::endl;
                std::cout << "|  PARTIDOS DISPONIBLES:                        |" << std::endl;
                
                for (size_t i = 0; i < partidos.size(); i++) {
                    std::cout << "|  " << std::setw(2) << std::left << i + 1 
                            << ". " << std::setw(35) << std::left 
                            << partidos[i]->nombre << " |" << std::endl;
                }
                
                std::cout << "|   0. Cancelar                                 |" << std::endl;
                std::cout << "+------------------------------------------------+" << std::endl;
                
                int opcion;
                std::cout << "\nSeleccione partido (1-" << partidos.size() << "): ";
                std::cin >> opcion;
                std::cin.ignore();
                
                if (opcion < 1 || opcion > static_cast<int>(partidos.size())) {
                    std::cout << "Operación cancelada." << std::endl;
                    break;
                }
                
                Partido* partidoElegido = partidos[opcion - 1];
                
                // Obtener candidatos del partido
                auto candidatosData = sistema.candidatosAlcaldiaPorPartido(partidoElegido);
                
                if (candidatosData.empty()) {
                    std::cout << "\nEl partido no tiene candidatos a alcaldia." << std::endl;
                    break;
                }
                
                auto& candidatos = std::get<1>(candidatosData[0]);
                
                // Imprimir resultados
                std::cout << "\n";
                std::cout << "+================================================================+" << std::endl;
                std::cout << "|  CANDIDATOS A ALCALDIA - " 
                        << std::setw(35) << std::left << partidoElegido->nombre << "|" << std::endl;
                std::cout << "|  Total: " << std::setw(52) << std::left 
                        << std::to_string(candidatos.size()) + " candidato(s)" << "|" << std::endl;
                std::cout << "+================================================================+" << std::endl;
                
                for (size_t i = 0; i < candidatos.size(); i++) {
                    Candidato* c = candidatos[i];                    
                    std::string sexo = (c->sexo == Sexo::Masculino) ? "M" : "F";
                    std::string ciudad = c->ciudadAspirante ? c->ciudadAspirante->nombre : "N/A";
                    
                    std::cout << "| " << std::setw(2) << std::right << i + 1 << ". " 
                            << std::setw(25) << std::left << (c->nombre + " " + c->apellido)
                            << " | Ciudad: " << std::setw(15) << std::left << ciudad                            
                            << " | Sexo: " << sexo << " |" << std::endl;
                }
                
                std::cout << "+================================================================+" << std::endl;
                break;
            }

            case 10: {
                std::cout << "\n=== CANDIDATOS A ALCALDIA POR CIUDAD Y PARTIDO ===" << std::endl;
                
                // 1. Seleccionar ciudad
                auto ciudades = sistema.obtenerCiudadesElectorales();
                auto partidos = sistema.obtenerPartidosLegales();

                if (ciudades.empty() || partidos.empty()) {
                    std::cout << "No hay ciudades o partidos disponibles." << std::endl;
                    break;
                }
                
                std::cout << "\nSeleccione ciudad:" << std::endl;
                for (size_t i = 0; i < ciudades.size(); i++) {
                    std::cout << i + 1 << ". " << ciudades[i]->nombre 
                            << " (" << ciudades[i]->candidatosAlcaldia.size() << " candidatos)" << std::endl;
                }
                
                int opcionCiudad;
                std::cout << "Opción: ";
                std::cin >> opcionCiudad;
                std::cin.ignore();
                
                if (opcionCiudad < 1 || opcionCiudad > static_cast<int>(ciudades.size())) {
                    std::cout << "Opción inválida." << std::endl;
                    break;
                }
                
                Ciudad* ciudadSeleccionada = ciudades[opcionCiudad - 1];
                
                
                std::cout << "\nSeleccione partido:" << std::endl;
                for (size_t i = 0; i < partidos.size(); i++) {
                    std::cout << i + 1 << ". " << partidos[i]->nombre << std::endl;
                }
                
                int opcionPartido;
                std::cout << "Opción: ";
                std::cin >> opcionPartido;
                std::cin.ignore();
                
                if (opcionPartido < 1 || opcionPartido > static_cast<int>(partidos.size())) {
                    std::cout << "Opción inválida." << std::endl;
                    break;
                }
                
                Partido* partidoSeleccionado = partidos[opcionPartido - 1];
                
                // 3. Obtener y mostrar resultados
                auto candidatos = sistema.candidatosAlcaldiaPorCiudadYPartido(ciudadSeleccionada, partidoSeleccionado);
                
                std::cout << "\n==================================================" << std::endl;
                std::cout << "CIUDAD: " << ciudadSeleccionada->nombre << std::endl;
                std::cout << "PARTIDO: " << partidoSeleccionado->nombre << std::endl;
                std::cout << "==================================================" << std::endl;
                
                if (candidatos.empty()) {
                    std::cout << "\nNo hay candidatos para esta combinación." << std::endl;
                } else {
                    std::cout << "\nCandidatos encontrados: " << candidatos.size() << "\n" << std::endl;
                    
                    for (size_t i = 0; i < candidatos.size(); i++) {
                        Candidato* cand = candidatos[i];
                        int edad = UtilidadesAnalisis::calcularEdad(cand->fechaNacimiento);
                        std::string sexo = (cand->sexo == Sexo::Masculino) ? "Masculino" : "Femenino";
                        
                        std::cout << i + 1 << ". " << cand->nombre << " " << cand->apellido << std::endl;
                        std::cout << "   Edad: " << edad << " años" << std::endl;
                        std::cout << "   Sexo: " << sexo << std::endl;
                        std::cout << "   Estado Civil: ";
                        std::cout << std::endl;                        
                        std::cout << std::string(40, '-') << std::endl;
                    }
                }
                
                std::cout << "\n==================================================" << std::endl;
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
