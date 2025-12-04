#include "DatosElectoral.h"
#include "CargarDatos.h"
#include "UtilidadesAnalisis.h"
#include <iostream>
#include <limits>

// ============================================================================
// FUNCIONES DEL MENÚ
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
// MENÚ DE CONSULTAS
// ============================================================================

void menuConsultas(DatosElectoral& sistema, Pais* pais) {
    int opcion = -1;
    
    while (opcion != 0) {
        std::cout << "\n";
        std::cout << "+----------------------------------------+" << std::endl;
        std::cout << "¦         MENÚ DE CONSULTAS              ¦" << std::endl;
        std::cout << "¦----------------------------------------¦" << std::endl;
        std::cout << "¦  1. Ciudades electorales               ¦" << std::endl;
        std::cout << "¦  2. Partidos legales                   ¦" << std::endl;
        std::cout << "¦  3. Candidatos por ciudad              ¦" << std::endl;
        std::cout << "¦  4. Candidatos presidenciales          ¦" << std::endl;
        std::cout << "¦  5. Candidatos alcaldía por partido    ¦" << std::endl;
        std::cout << "¦  6. Candidatos presidencia por partido ¦" << std::endl;
        std::cout << "¦  7. Candidatos por partido y región    ¦" << std::endl;
        std::cout << "¦  8. Candidatos partido todas ciudades  ¦" << std::endl;
        std::cout << "¦  9. Tarjetón de alcaldía               ¦" << std::endl;
        std::cout << "¦ 10. Tarjetón de presidencia            ¦" << std::endl;
        std::cout << "¦ 11. Censo electoral                    ¦" << std::endl;
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
                    std::cout << "- " << p->nombre << " (Rep. Legal: " << p->representanteLegal << ")" << std::endl;
                }
                break;
            }
            
            case 3: {
                Ciudad* ciudad = seleccionarCiudad(sistema);
                if (ciudad) {
                    auto candidatos = ciudad->candidatosAlcaldia;
                    std::cout << "\n=== CANDIDATOS A ALCALDÍA EN " << ciudad->nombre << " ===" << std::endl;
                    for (auto c : candidatos) {
                        int edad = UtilidadesAnalisis::calcularEdad(c->fechaNacimiento);
                        std::string sexo = (c->sexo == Sexo::Masculino) ? "M" : "F";
                        std::cout << "- " << c->nombre << " " << c->apellido 
                                  << " (Edad: " << edad << ", Sexo: " << sexo 
                                  << ", Partido: " << c->partido->nombre << ")" << std::endl;
                    }
                }
                break;
            }
            
			case 4: {
    			auto candidatos = sistema.candidatosPresidenciales();
    			std::cout << "\n=== CANDIDATOS PRESIDENCIALES ===" << std::endl;
    			int num = 1;
    
    				for (const auto& candidato_pair : candidatos) {
        				auto pres = candidato_pair.first;
        				auto vice = candidato_pair.second;
        				int edadPres = UtilidadesAnalisis::calcularEdad(pres->fechaNacimiento);
        				int edadVice = UtilidadesAnalisis::calcularEdad(vice->fechaNacimiento);
        				std::cout << num << ". PRESIDENTE: " << pres->nombre << " (edad: " << edadPres << ")"
             			<< ", VICEPRESIDENTE: " << vice->nombre << " (edad: " << edadVice << ")" << std::endl;
        				num++;
    				}
    				break;
					}
            
				case 5: {
    				auto resultado = sistema.candidatosAlcaldiaPorPartido();
    				std::cout << "\n=== CANDIDATOS ALCALDÍA POR PARTIDO ===" << std::endl;
    				for (const auto& item : resultado) {
        				auto ciudad = std::get<0>(item);
        				auto partido = std::get<1>(item);
        				auto candidatos = std::get<2>(item);
        
        					std::cout << "\nCiudad: " << ciudad->nombre << " | Partido: " << partido->nombre << std::endl;
        					for (auto c : candidatos) {
            				std::cout << "  - " << c->nombre << " " << c->apellido << std::endl;
        				}
    				}
    				break;
				}

				case 6: {
    				Partido* partido = seleccionarPartido(sistema);
    				if (partido) {
        				auto resultado = sistema.candidatosPresidenciaPorPartido(partido);
        				auto pres = resultado.first;
        				auto vice = resultado.second;
        
				        if (pres) {
            				int edad = UtilidadesAnalisis::calcularEdad(pres->fechaNacimiento);
            				std::cout << "\n=== CANDIDATURA PRESIDENCIAL - " << partido->nombre << " ===" << std::endl;
            				std::cout << "Presidente: " << pres->nombre << " (edad: " << edad << ")" << std::endl;
            				if (vice) {
                				int edadVice = UtilidadesAnalisis::calcularEdad(vice->fechaNacimiento);
                				std::cout << "Vicepresidente: " << vice->nombre << " (edad: " << edadVice << ")" << std::endl;
            				}
        				} else {
            				std::cout << "Este partido no tiene candidatos presidenciales." << std::endl;
        				}
    				}
    				break;
				}

            
				case 7: {
    				Partido* partido = seleccionarPartido(sistema);
    				Region* region = seleccionarRegion(sistema);
    				if (partido && region) {
        				std::vector<Candidato*> candidatos;
        
        				// Buscar candidatos del partido en las ciudades de la región
        				for (auto ciudad : region->ciudades) {
            				for (auto c : ciudad->candidatosAlcaldia) {
                				if (c->partido.get() == partido) {
                    				candidatos.push_back(c);
                				}
            				}
        				}
        
        				UtilidadesAnalisis::imprimirCandidatosPartidoRegion(candidatos, partido, region);
    				}
    				break;
				}
            
				case 8: {
    				Partido* partido = seleccionarPartido(sistema);
    				if (partido) {
        				std::vector<std::tuple<Ciudad*, std::string, Sexo, int>> candidatos;
        
        				// Buscar todos los candidatos del partido en todas las ciudades
        				auto& ciudades = sistema.obtenerListaCiudades();
        				for (auto ciudad : ciudades) {
            				for (auto c : ciudad->candidatosAlcaldia) {
                				if (c->partido.get() == partido) {
                    				int edad = UtilidadesAnalisis::calcularEdad(c->fechaNacimiento);
                    				candidatos.push_back(std::make_tuple(ciudad, c->nombre + " " + c->apellido, c->sexo, edad));
                				}
            				}
        					}
        
        				UtilidadesAnalisis::imprimirCandidatosPartidoTodasCiudades(candidatos, partido);
    				}
    				break;
					}

				case 9: {
    					Ciudad* ciudad = seleccionarCiudad(sistema);
    				if (ciudad) {
        				UtilidadesAnalisis::imprimirTarjetonAlcaldia(ciudad);
    				}
    				break;
				}

				case 10: {
    				UtilidadesAnalisis::imprimirTarjetonPresidencia(pais);
    				break;
				}

				case 11: {
    				// Actualizar censo regional manualmente
    				auto& regiones = sistema.obtenerListaRegiones();
    				for (auto region : regiones) {
        				region->censoElectoral = 0;
        				for (auto ciudad : region->ciudades) {
            				region->censoElectoral += ciudad->censoElectoral;
        				}
    				}
    
    				auto& ciudades = sistema.obtenerListaCiudades();
    				UtilidadesAnalisis::imprimirCensoElectoral(ciudades);
    				break;
				}


				case 0:
    				std::cout << "Volviendo al menú principal..." << std::endl;
    				break;

				default:
    				std::cout << "Opción no válida. Intente de nuevo." << std::endl;
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
        std::cout << "¦    SISTEMA ELECTORAL - MENÚ PRINCIPAL   ¦" << std::endl;
        std::cout << "¦----------------------------------------¦" << std::endl;
        std::cout << "¦  1. Consultas                          ¦" << std::endl;
        std::cout << "¦  2. Simulación de Elecciones (próximo) ¦" << std::endl;
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
                std::cout << "Simulación no disponible en esta fase." << std::endl;
                break;
            case 0:
                std::cout << "\n¡Hasta luego!" << std::endl;
                break;
            default:
                std::cout << "Opción no válida. Intente de nuevo." << std::endl;
        }
    }
}

// ============================================================================
// MAIN
// ============================================================================

int main() {
    DatosElectoral sistema;
    
    // Crear país
    Pais* pais = new Pais();
    pais->nombre = "Colombia";
    pais->codigo = "COL_001";
    sistema.obtenerListaPaises().push_back(pais);
    
    // Cargar datos
    bool ok = CargarDatos::cargarTodosLosDatos(
        sistema,
        "Partidos.txt",
        "Regiones.txt",
        "Ciudades.txt",
        "Candidatos.txt"
    );
    
    if (!ok) {
        std::cerr << "Error al cargar datos. Abortando." << std::endl;
        delete pais;
        return 1;
    }
    
    // Actualizar censo regional
	auto& regiones = sistema.obtenerListaRegiones();
	for (auto region : regiones) {
    	region->censoElectoral = 0;
    	for (auto ciudad : region->ciudades) {
	        region->censoElectoral += ciudad->censoElectoral;
    	}
	}
    
    // Mostrar menú principal
    menuPrincipal(sistema, pais);
    
    return 0;
}
