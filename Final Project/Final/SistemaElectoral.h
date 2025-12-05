#ifndef SISTEMA_ELECTORAL_H
#define SISTEMA_ELECTORAL_H

#include "DatosElectoral.h"
#include <ctime>

// Namespace para agrupar las funciones migradas desde main.cpp
namespace SistemaElectoral {

    // Selecciones (devuelven punteros dentro de 'sistema')
    Ciudad* seleccionarCiudad(DatosElectoral& sistema);
    Partido* seleccionarPartido(DatosElectoral& sistema);
    Region* seleccionarRegion(DatosElectoral& sistema);

    // Operaciones sobre candidatos (usando sistema)
    void agregarCandidato(DatosElectoral& sistema, Pais*);
    void eliminarCandidato(DatosElectoral& sistema);
    void modificarCandidato(DatosElectoral& sistema);

    // Menus que interactuan con 'sistema' (NOTA: menuPrincipal NO fue movido porque usa SimulacionElectoral)
    void menuGestionCandidatos(DatosElectoral& sistema, Pais* pais);
    void menuConsultas(DatosElectoral& sistema, Pais* pais);

} // namespace SistemaElectoral

#endif // SISTEMA_ELECTORAL_H
