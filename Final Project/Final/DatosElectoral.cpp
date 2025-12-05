#include "DatosElectoral.h"
#include "UtilidadesAnalisis.h"
#include <iostream>    

#include <sstream>
#include <memory>
#include <tuple>
#include <algorithm>
#include <vector> 

DatosElectoral::DatosElectoral() {}

DatosElectoral::~DatosElectoral() {
    for (auto p : paises) {
        delete p;
    }
    for (auto r : regiones) {
        delete r;
    }
    for (auto c : ciudades) {
        delete c;
    }
    for (auto cand : candidatosAlcaldia) {
        delete cand;
    }
    for (auto cand : candidatosPresidenciaLista) {
        delete cand;
    }

}

unsigned long DatosElectoral::calcularPesoAscii(const std::string& texto) const {
    unsigned long suma = 0;
    for (unsigned char c : texto) {
        suma += static_cast<unsigned long>(c);
    }
    return suma;
}

// Helper: hex string de la suma ascii
std::string DatosElectoral::pesoHex(const std::string& texto) const {
    unsigned long peso = calcularPesoAscii(texto);
    std::stringstream ss;
    ss << std::hex << std::uppercase << peso;
    return ss.str();
}


// ============================================================================
// M�TODOS DE CREACI�N (crear...)
// ============================================================================

// Crear Pa�s
// Crea un nuevo pa�s y lo agrega a la lista de pa�ses
// Par�metros: nombre, candidatos a presidencia, candidatos a vicepresidencia
// Retorna: puntero al Pa�s creado
Pais* DatosElectoral::crearPais(std::string& nombre, 
                                 std::vector<Candidato*> candidatosPresidencia, 
                                 std::vector<Candidato*> candidatosVicepresidencia) {
    Pais* p = new Pais();
    p->nombre = nombre;
    p->codigo = parses.hashToHex(nombre);
    p->candidatosPresidencia = candidatosPresidencia;    
    paises.push_back(p);
    return p;
}

// Crear Regi�n
// Crea una nueva regi�n asociada a un pa�s padre
// Par�metros: nombre, lista de ciudades, pa�s padre
// Retorna: puntero a la Regi�n creada
Region* DatosElectoral::crearRegion(std::string nombre, 
                                     std::vector<Ciudad*> ciudades, 
                                     Pais* padre) {
    if (padre == nullptr) {
        return new Region();
    }
    
    Region* r = new Region();
    r->nombre = nombre;
    r->codigo = parses.hashToHex(nombre);
    r->ciudades = ciudades;
    r->censoElectoral = 0;
    r->pais = padre;
    
    padre->regiones.push_back(r);
    regiones.push_back(r);
    return r;
}

// Crear Ciudad
// Crea una nueva ciudad asociada a una regi�n padre
// Par�metros: nombre, regi�n padre, candidatos a alcald�a
// Retorna: puntero a la Ciudad creada
Ciudad* DatosElectoral::crearCiudad(std::string nombre, 
                                     Region* regionPadre, 
                                     std::vector<Candidato*> candidatos) {
    Ciudad* c = new Ciudad();
    c->nombre = nombre;
    c->codigo = parses.hashToHex(nombre);
    c->region = regionPadre;
    c->candidatosAlcaldia = candidatos;
    c->censoElectoral = 0;
    
    ciudades.push_back(c);
    return c;
}

// Crear Candidato
// Crea un nuevo candidato con todos sus datos personales y electorales
// Valida autom�ticamente que el candidato sea v�lido
// Par�metros: datos personales, ubicaci�n, informaci�n electoral
// Retorna: puntero al Candidato (nullptr si es inv�lido)
Candidato* DatosElectoral::crearCandidato(std::string nombre, std::string apellido, std::string identificacion, Sexo sexo, EstadoCivil estadoCivil, std::tm fechaNacimiento,
                                           Ciudad* ciudadNacimiento, Ciudad* ciudadResidencia, Partido* partido, TipoCandidato tipo, Pais* pais, Ciudad* ciudadAspirante, 
                                           Candidato* vicepresidente) {
    
    Candidato* candidato = new Candidato();
    
    // Datos personales
    candidato->nombre = nombre;
    candidato->apellido = apellido;
    candidato->identificacion = identificacion;
    candidato->sexo = sexo;
    candidato->estadoCivil = estadoCivil;
    candidato->fechaNacimiento = fechaNacimiento;
    
    // Ubicaci�n geogr�fica
    candidato->ciudadNacimiento = ciudadNacimiento;
    candidato->ciudadResidencia = ciudadResidencia;
    
    // Informaci�n electoral
    candidato->partido = partido;
    candidato->tipo = tipo;
    candidato->ciudadAspirante = ciudadAspirante;
    candidato->vicepresidente = vicepresidente;
    
    // Validar integridad del candidato
    if (!candidato->esValido()) {
        std::cerr << "Candidato invalido: " << nombre << " " << apellido << std::endl;
        delete candidato;
        return nullptr;
    }
    
    // Agregar a listas apropiadas
    if (tipo == TipoCandidato::ALCALDE) {
        ciudadAspirante->candidatosAlcaldia.push_back(candidato);
        candidatosAlcaldia.push_back(candidato);
        
    } else {
        candidatosPresidenciaLista.push_back(candidato);
        pais->candidatosPresidencia.push_back(candidato);
    }    
    return candidato;
}


// Crear Partido
// Crea un nuevo partido pol�tico
// Par�metros: nombre, representante legal, estado legal
// Retorna: puntero al Partido creado
Partido* DatosElectoral::crearPartido(std::string nombre, 
                                       std::string representanteLegal, 
                                       bool legal) {
    Partido* p = new Partido();
    p->nombre = nombre;
    p->codigo = parses.hashToHex(nombre);
    p->representanteLegal = representanteLegal;
    p->legal = legal;
    
    partidos.push_back(p);
    return p;
}

// ============================================================================
// M�TODOS DE AGREGACI�N (agregar...)
// ============================================================================

// Agregar candidato a ciudad
// Agrega un candidato a la lista de candidatos de una ciudad espec�fica
// Par�metros: candidato, ciudad, regi�n
void DatosElectoral::agregarCandidatoACiudad(Candidato* candidato, 
                                              Ciudad* ciudad, 
                                              Region* region) {
    if (!candidato || !ciudad || !region) {
        return;
    }
    
    // Asegurar que la ciudad existe en el vector global
    if (std::find(ciudades.begin(), ciudades.end(), ciudad) == ciudades.end()) {
        ciudades.push_back(ciudad);
    }
    
    // Asegurar que la ciudad est� en la regi�n
    if (std::find(region->ciudades.begin(), region->ciudades.end(), ciudad) == region->ciudades.end()) {
        region->ciudades.push_back(ciudad);
    }
    
    // Verificar si el candidato ya est� presente
    if (std::find(ciudad->candidatosAlcaldia.begin(), ciudad->candidatosAlcaldia.end(), candidato) 
        != ciudad->candidatosAlcaldia.end()) {
        return;
    }
    
    ciudad->candidatosAlcaldia.push_back(candidato);
}

// ============================================================================
// M�TODOS DE CONSULTA (obtener... / consultar...)
// ============================================================================

// 1. Obtener ciudades electorales
// Devuelve todas las ciudades registradas en el sistema
std::vector<Ciudad*> DatosElectoral::obtenerCiudadesElectorales() {
    std::vector<Ciudad*> res;
    for (auto c : ciudades) {
        res.push_back(c);
    }
    return res;
}

// 2. Obtener partidos legalmente reconocidos
// Devuelve solo los partidos que tienen estatus legal = true
std::vector<Partido*> DatosElectoral::obtenerPartidosLegales() {
    std::vector<Partido*> encontrados;
    
    for (auto p : partidos) {
        if (p->legal) {
            encontrados.push_back(p);
        }
    }
    
    return encontrados;
}

// 3. Obtener candidatos por ciudad
// Retorna pares (Ciudad, vector de candidatos a alcald�a)
std::vector<std::pair<Ciudad*, std::vector<Candidato*>*>> DatosElectoral::candidatosPorCiudad() {
    std::vector<std::pair<Ciudad*, std::vector<Candidato*>*>> res;
    
    for (auto c : ciudades) {
        res.push_back({c, &c->candidatosAlcaldia});
    }
    
    return res;
}

// 4. Obtener candidatos presidenciales
// Retorna pares (Presidente, Vicepresidente) de cada pa�s
std::vector<std::pair<Candidato*, Candidato*>> DatosElectoral::candidatosPresidenciales() {
    std::vector<std::pair<Candidato*, Candidato*>> res;
    
    for (auto p : paises) {
        int nPresidentes = static_cast<int>(p->candidatosPresidencia.size());        
        
        for (int j = 0; j < nPresidentes; ++j) {
            Candidato* pres = p->candidatosPresidencia[j];
            Candidato* vice = pres->vicepresidente;
            res.push_back({pres, vice});
        }
    }
    
    return res;
}

// 5. Obtener candidatos a alcald�a por partido
// Retorna tuplas (Ciudad, Partido, vector de candidatos)
std::vector<std::tuple<Ciudad*, Partido*, std::vector<Candidato*>>> DatosElectoral::candidatosAlcaldiaPorPartido() {
    std::vector<std::tuple<Ciudad*, Partido*, std::vector<Candidato*>>> res;
    
    for (auto ciudad : ciudades) {
        for (auto cand : ciudad->candidatosAlcaldia) {
            Partido* p = cand->partido;
            
            // Buscar si ya existe tupla (ciudad, partido)
            bool found = false;
            for (auto& t : res) {
                if (std::get<0>(t) == ciudad && std::get<1>(t) == p) {
                    std::get<2>(t).push_back(cand);
                    found = true;
                    break;
                }
            }
            
            if (!found) {
                std::vector<Candidato*> v;
                v.push_back(cand);
                res.push_back(std::make_tuple(ciudad, p, v));
            }
        }
    }
    
    return res;
}

// 6. Obtener candidatos presidenciales por partido
// Retorna par (Presidente, Vicepresidente) de un partido espec�fico
std::pair<Candidato*, Candidato*> DatosElectoral::candidatosPresidenciaPorPartido(Partido* partido) {
    if (!partido) return {nullptr, nullptr};
    
    for (auto p : paises) {
        int nPresidentes = static_cast<int>(p->candidatosPresidencia.size());
        
        for (int j = 0; j < nPresidentes; ++j) {
            Candidato* pres = p->candidatosPresidencia[j];
            if (pres->partido != partido)
                continue;
            Candidato* vice = pres->vicepresidente;                            
            return {pres, vice};
        }
    }
    
    return {nullptr, nullptr};
}

// ============================================================================
// M�TODOS ACCESORES (obtenerLista...)
// ============================================================================

// Obtener lista de pa�ses
std::vector<Pais*>& DatosElectoral::obtenerListaPaises() {
    return paises;
}

// Obtener lista de regiones
std::vector<Region*>& DatosElectoral::obtenerListaRegiones() {
    return regiones;
}

// Obtener lista de ciudades
std::vector<Ciudad*>& DatosElectoral::obtenerListaCiudades() {
    return ciudades;
}

// Obtener lista de candidatos a alcald�a
std::vector<Candidato*>& DatosElectoral::obtenerListaCandidatos() {
    return candidatosAlcaldia;
}

// Obtener lista de partidos
std::vector<Partido*>& DatosElectoral::obtenerListaPartidos() {
 

    return partidos;
}

