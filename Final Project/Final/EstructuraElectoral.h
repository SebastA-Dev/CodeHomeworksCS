#ifndef ESTRUCTURA_ELECTORAL_H
#define ESTRUCTURA_ELECTORAL_H

#include <string>
#include <memory>
#include <ctime>
#include <vector>

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
    std::vector<Candidato*> candidatosPresidencia;
    std::vector<Candidato*> candidatosVicepresidencia;
};

// ---------------------------------------------------------------------------
// REGI�N
// ---------------------------------------------------------------------------
struct Region {
    std::string codigo;
    std::string nombre;
    std::vector<Ciudad*> ciudades;
    std::vector<Candidato*> candidatosAlcaldias;
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
    std::vector<Candidato*> candidatosAlcaldia;
    int censoElectoral = 0;
};

// ---------------------------------------------------------------------------
// PERSONA
// ---------------------------------------------------------------------------

enum class Sexo { Masculino, Femenino, Otro };
enum class EstadoCivil { Soltero, Casado, Divorciado, Viudo };
struct Persona {
    std::string nombre;
    std::string apellido;
    std::string identificacion;
    std::tm fechaNacimiento;
    Sexo sexo;
    EstadoCivil estadoCivil;
    Ciudad* ciudadNacimiento = nullptr;
    Ciudad* ciudadResidencia = nullptr;
};

// ---------------------------------------------------------------------------
// PARTIDO POL�TICO
// ---------------------------------------------------------------------------
struct Partido {
    std::string codigo;
    std::string nombre;    
    std::shared_ptr<Persona> representanteLegal;
    bool legal = false;
};

// ---------------------------------------------------------------------------
// CANDIDATO
// ---------------------------------------------------------------------------

struct Candidato {
    std::shared_ptr<Persona> inf;
    std::string codigo;
    std::shared_ptr<Partido> partido;
    std::shared_ptr<Candidato> formulaVicepresidencial;
    
    // RECTIFICAR EL TIPO DE CANDIDATO (ALCALDE O PRESIDENTE)
    enum class TipoCandidato { ALCALDIA, PRESIDENCIA };
    TipoCandidato tipo = TipoCandidato::ALCALDIA;
    Ciudad* ciudadAspirante = nullptr;  // Para alcaldes
};
#endif
