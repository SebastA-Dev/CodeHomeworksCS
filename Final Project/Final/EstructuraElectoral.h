#ifndef ESTRUCTURA_ELECTORAL_H
#define ESTRUCTURA_ELECTORAL_H

#include <string>
#include "Lista.h"   // usa tu implementaci�n de Lista<T>

struct Ciudad;
struct Region;
struct Partido;
struct Candidato;
struct Pais;

// ---------------------------------------------------------------------------
// PAIS
// ---------------------------------------------------------------------------
struct Pais {
    std::string codigo;
    std::string nombre;        
    Lista<Lista<Candidato*>> candidatosPresidencia;
};

// ---------------------------------------------------------------------------
// REGI�N
// ---------------------------------------------------------------------------
struct Region {
    std::string codigo;
    std::string nombre;
    Lista<Ciudad*> ciudades;
    Lista<Candidato*> candidatosAlcaldias;
    int censoElectoral = 0;
    Pais* pais = nullptr;
};

// ---------------------------------------------------------------------------
// CIUDAD
// ---------------------------------------------------------------------------
struct Ciudad {
    std::string codigo;
    std::string nombre;    
    Region* region = nullptr;
    Lista<Candidato*> candidatosAlcaldia;
    int censoElectoral = 0;
};

// ---------------------------------------------------------------------------
// PARTIDO POL�TICO
// ---------------------------------------------------------------------------
struct Partido {
    std::string codigo;
    std::string nombre;    
    std::string representanteLegal;
    bool legal = false;
};

// ---------------------------------------------------------------------------
// CANDIDATO
// ---------------------------------------------------------------------------
struct Candidato {
    std::string nombre;
    std::string apellido;
    std::string codigo; // Es la identficacion de la persona

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
