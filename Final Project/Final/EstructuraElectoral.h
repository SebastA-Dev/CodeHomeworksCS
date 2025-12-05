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
// CLASES ENUM
// ---------------------------------------------------------------------------

enum class Sexo { Masculino, Femenino };
enum class EstadoCivil { Soltero, Casado, Divorciado, UnionLibre };
enum class TipoCandidato {ALCALDE, PRESIDENTE, VICEPRESIDENTE };

// ---------------------------------------------------------------------------
// PAIS
// ---------------------------------------------------------------------------
struct Pais {
    std::string codigo;
    std::string nombre;        
    std::vector<Candidato*> candidatosPresidencia;
    std::vector<Region*> regiones;
};

// ---------------------------------------------------------------------------
// REGI�N
// ---------------------------------------------------------------------------
struct Region {
    std::string codigo;
    std::string nombre;
    std::vector<Ciudad*> ciudades;
    int censoElectoral = 0; // Suma del censo electoral de cada ciudad
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
// PARTIDO POL�TICO
// ---------------------------------------------------------------------------
struct Partido {
    std::string codigo;
    std::string nombre; 
    std::string representanteLegal;
    bool legal;
};

// ---------------------------------------------------------------------------
// CANDIDATO
// ---------------------------------------------------------------------------

struct Candidato {
    // Identificacion personal
    bool persiste; 
    std::string nombre;
    std::string apellido;
    std::string identificacion;
    std::tm fechaNacimiento;
    Sexo sexo;
    EstadoCivil estadoCivil;
    
    // Ubicacion geografica 
    Ciudad* ciudadNacimiento = nullptr;
    Ciudad* ciudadResidencia = nullptr;
    
    // Informacion electoral
    
    Partido* partido;
    TipoCandidato tipo;    
    Ciudad* ciudadAspirante = nullptr;  // Para alcaldes
    Candidato* vicepresidente = nullptr; // Solo para el presidente

    // Validaciones
    bool esValido() const {
    	
        // El candidato debe tener un nombre, apellido y identificacion
        if (nombre.empty() || apellido.empty() || identificacion.empty())
            return false;
        
        // El candidato debe tener partido
        if (!partido)
            return false;
        
        // El alcalde debe vivir en su ciudad
        if ((tipo == TipoCandidato::ALCALDE) && (!ciudadAspirante || ciudadResidencia != ciudadAspirante))
                return false;
        
        // El presidente NO requiere vicepresidente en este momento
        // Se asigna despu�s durante la carga
        
        return true;
    }
};


#endif
