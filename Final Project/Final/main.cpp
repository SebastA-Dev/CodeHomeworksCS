#include "DatosElectoral.h"
#include "CargarDatos.h"
#include "UtilidadesAnalisis.h"
#include "SimulacionElectoral.h"
#include "SistemaElectoral.h"   // <<<<< nuevo
#include <iostream>
#include <limits>
#include <ctime>
#include <cstdlib>

// NOTA: se ha movido al archivo SistemaElectoral.*:
// - seleccionarCiudad, seleccionarPartido, seleccionarRegion
// - parsearFecha, parsearSexo, parsearEstadoCivil, parsearTipoCandidato
// - agregarCandidato, eliminarCandidato, modificarCandidato
// - menuGestionCandidatos, menuConsultas

// ============================================================================
// MENU PRINCIPAL (SE MANTIENE EN main.cpp PORQUE USA SimulacionElectoral)
// ============================================================================

void menuPrincipal(DatosElectoral& sistema, Pais* pais) {
    int opcion = -1;
    
    while (opcion != 0) {
        std::cout << "\n";
        std::cout << "+----------------------------------------+" << std::endl;
        std::cout << "|  SISTEMA ELECTORAL - MENU PRINCIPAL    |" << std::endl;
        std::cout << "|----------------------------------------|" << std::endl;
        std::cout << "|  1. Consultas                          |" << std::endl;
        std::cout << "|  2. Gestion de Candidatos              |" << std::endl;
        std::cout << "|  3. Simulacion de Elecciones (proximo) |" << std::endl;
        std::cout << "|  0. Salir                              |" << std::endl;
        std::cout << "+----------------------------------------+" << std::endl;
        std::cout << "\nSeleccione opcion: ";
        std::cin >> opcion;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        switch (opcion) {
            case 1:
                SistemaElectoral::menuConsultas(sistema, pais);
                break;
            case 2:
                SistemaElectoral::menuGestionCandidatos(sistema, pais);
                break;
            case 3: {
                std::cout << "\nEsta seguro de que desea iniciar la simulacion?" << std::endl;
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
                std::cout << "\nHasta luego!" << std::endl;
                break;
            default:
                std::cout << "Opcion no valida." << std::endl;
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
        "Candidatos.txt",
        pais
    );
    
    if (!ok) {
        std::cerr << "Error al cargar datos." << std::endl;
        delete pais;
        return 1;
    }
    
    std::vector<std::pair<Candidato *, Candidato *>> test = sistema.candidatosPresidenciales();    

    menuPrincipal(sistema, pais);
    
    return 0;
}
