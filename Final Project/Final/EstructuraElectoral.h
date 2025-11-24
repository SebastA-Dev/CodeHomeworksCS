#ifndef SISTEMA_ELECTORAL_H
#define SISTEMA_ELECTORAL_H

#include <string>
#include "Lista.h"   // Ahora correctamente incluido

struct Ciudad;     // Declaraciones adelantadas internas
struct Region;
struct Partido;
struct Candidato;

// ---------------------------------------------------------------------------
// REGIÓN
// ---------------------------------------------------------------------------
struct Region {
    std::string nombre;
    Lista<Ciudad*> ciudades;   // Cambiado de vector a Lista
    int censoElectoral = 0;
};

// ---------------------------------------------------------------------------
// CIUDAD
// ---------------------------------------------------------------------------
struct Ciudad {
    std::string nombre;
    Region* region = nullptr;
    int censoElectoral = 0;
};

// ---------------------------------------------------------------------------
// PARTIDO POLÍTICO
// ---------------------------------------------------------------------------
struct Partido {
    std::string nombre;
    std::string representanteLegal;
};

// ---------------------------------------------------------------------------
// CANDIDATO
// ---------------------------------------------------------------------------
struct Candidato {
    std::string nombre;
    std::string apellido;
    std::string identificacion;

    char sexo;                       
    std::string estadoCivil;

    std::string fechaNacimiento;
    Ciudad* ciudadNacimiento = nullptr;
    Ciudad* ciudadResidencia = nullptr;

    Partido* partido = nullptr;

    bool esAlcaldia = false;
    bool esPresidencia = false;

    Candidato* formulaVicepresidencial = nullptr;
};

#endif

