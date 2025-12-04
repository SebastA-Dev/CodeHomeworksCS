#include "DatosElectoral.h"
#include "UtilidadesAnalisis.h"

#include <sstream>
#include <memory>
#include <tuple>
#include <algorithm>

DatosElectoral::DatosElectoral() {}

DatosElectoral::~DatosElectoral() {
    for (auto p : paises) delete p;
    for (auto r : regiones) delete r;
    for (auto c : ciudades) delete c;
    for (auto cand : candidatosAlcaldia) delete cand;
    for (auto cand : candidatosPresidenciaLista) delete cand;
    for (auto p : partidos) delete p;
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

// Helper: insert into an ordered vector by ->codigo (numeric ascending by hex value)
template<typename T>
static void insertOrdered(std::vector<T>& vec, T item, bool asc = true) {
    // Convert hex string to unsigned long long (base 16)
    auto hexVal = [](const std::string &s) -> unsigned long long {
        if (s.empty()) return 0ULL;
        try {
            return std::stoull(s, nullptr, 16);
        } catch (...) {
            return 0ULL;
        }
    };

    auto ullToHex = [](unsigned long long v) -> std::string {
        std::stringstream ss;
        ss << std::hex << std::uppercase << v;
        return ss.str();
    };

    // Ensure unique numeric code: if collision, add a single bit (1<<k) to make it unique
    unsigned long long base = hexVal(item->codigo);
    unsigned long long assigned = base;
    unsigned int k = 0;
    auto exists = [&](unsigned long long v) {
        for (auto &e : vec) {
            if (hexVal(e->codigo) == v) return true;
        }
        return false;
    };

    while (exists(assigned)) {
        if (k < 63) assigned = base + (1ULL << k);
        else assigned = base + (unsigned long long)(k + 1);
        ++k;
    }

    // Update the item's codigo to the unique hex form
    item->codigo = ullToHex(assigned);

    // Comparator by numeric hex value
    auto compAsc = [&](const T& a, const T& b) { return hexVal(a->codigo) < hexVal(b->codigo); };
    auto compDesc = [&](const T& a, const T& b) { return hexVal(a->codigo) > hexVal(b->codigo); };

    if (asc) {
        // use upper_bound to keep existing equal (older) items before the new one
        auto it = std::upper_bound(vec.begin(), vec.end(), item, compAsc);
        vec.insert(it, item);
    } else {
        auto it = std::upper_bound(vec.begin(), vec.end(), item, compDesc);
        vec.insert(it, item);
    }
}

// ============================================================================
// MÉTODOS DE CREACIÓN (crear...)
// ============================================================================

// Crear País
// Crea un nuevo país y lo agrega a la lista de países
// Parámetros: nombre, candidatos a presidencia, candidatos a vicepresidencia
// Retorna: puntero al País creado
Pais* DatosElectoral::crearPais(std::string& nombre, 
                                 std::vector<Candidato*> candidatosPresidencia, 
                                 std::vector<Candidato*> candidatosVicepresidencia) {
    Pais* p = new Pais();
    p->nombre = nombre;
    p->codigo = parses.hashToHex(nombre);
    p->candidatosPresidencia = candidatosPresidencia;
    p->candidatosVicepresidencia = candidatosVicepresidencia;
    paises.push_back(p);
    return p;
}

// Crear Región
// Crea una nueva región asociada a un país padre
// Parámetros: nombre, lista de ciudades, país padre
// Retorna: puntero a la Región creada
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
// Crea una nueva ciudad asociada a una región padre
// Parámetros: nombre, región padre, candidatos a alcaldía
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
// Valida automáticamente que el candidato sea válido
// Parámetros: datos personales, ubicación, información electoral
// Retorna: puntero al Candidato (nullptr si es inválido)
Candidato* DatosElectoral::crearCandidato(std::string nombre, 
                                           std::string apellido,
                                           std::string identificacion,
                                           Sexo sexo,
                                           EstadoCivil estadoCivil,
                                           std::tm fechaNacimiento,
                                           Ciudad* ciudadNacimiento,
                                           Ciudad* ciudadResidencia,
                                           std::shared_ptr<Partido> partido,
                                           TipoCandidato tipo,
                                           Ciudad* ciudadAspirante,
                                           std::shared_ptr<Candidato> vicepresidente) {
    
    Candidato* candidato = new Candidato();
    
    // Datos personales
    candidato->nombre = nombre;
    candidato->apellido = apellido;
    candidato->identificacion = identificacion;
    candidato->sexo = sexo;
    candidato->estadoCivil = estadoCivil;
    candidato->fechaNacimiento = fechaNacimiento;
    
    // Ubicación geográfica
    candidato->ciudadNacimiento = ciudadNacimiento;
    candidato->ciudadResidencia = ciudadResidencia;
    
    // Información electoral
    candidato->codigo = parses.hashToHex(identificacion);
    candidato->partido = partido;
    candidato->tipo = tipo;
    candidato->ciudadAspirante = ciudadAspirante;
    candidato->vicepresidente = vicepresidente;
    
    // Validar integridad del candidato
    if (!candidato->esValido()) {
        delete candidato;
        return nullptr;
    }
    
    return candidato;
}

// Crear Partido
// Crea un nuevo partido político
// Parámetros: nombre, representante legal, estado legal
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
// MÉTODOS DE AGREGACIÓN (agregar...)
// ============================================================================

// Agregar candidato a ciudad
// Agrega un candidato a la lista de candidatos de una ciudad específica
// Parámetros: candidato, ciudad, región
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
    
    // Asegurar que la ciudad está en la región
    if (std::find(region->ciudades.begin(), region->ciudades.end(), ciudad) == region->ciudades.end()) {
        region->ciudades.push_back(ciudad);
    }
    
    // Verificar si el candidato ya está presente
    if (std::find(ciudad->candidatosAlcaldia.begin(), ciudad->candidatosAlcaldia.end(), candidato) 
        != ciudad->candidatosAlcaldia.end()) {
        return;
    }
    
    ciudad->candidatosAlcaldia.push_back(candidato);
}

// ============================================================================
// MÉTODOS DE CONSULTA (obtener... / consultar...)
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
// Retorna pares (Ciudad, vector de candidatos a alcaldía)
std::vector<std::pair<Ciudad*, std::vector<Candidato*>*>> DatosElectoral::candidatosPorCiudad() {
    std::vector<std::pair<Ciudad*, std::vector<Candidato*>*>> res;
    
    for (auto c : ciudades) {
        res.push_back({c, &c->candidatosAlcaldia});
    }
    
    return res;
}

// 4. Obtener candidatos presidenciales
// Retorna pares (Presidente, Vicepresidente) de cada país
std::vector<std::pair<Candidato*, Candidato*>> DatosElectoral::candidatosPresidenciales() {
    std::vector<std::pair<Candidato*, Candidato*>> res;
    
    for (auto p : paises) {
        int nPresidentes = static_cast<int>(p->candidatosPresidencia.size());
        int nVicepresidentes = static_cast<int>(p->candidatosVicepresidencia.size());
        
        // Emparejar presidentes con vicepresidentes
        int minSize = (nPresidentes < nVicepresidentes) ? nPresidentes : nVicepresidentes;
        
        for (int j = 0; j < minSize; ++j) {
            Candidato* pres = p->candidatosPresidencia[j];
            Candidato* vice = p->candidatosVicepresidencia[j];
            res.push_back({pres, vice});
        }
        
        // Presidentes sin vicepresidente (si existen)
        for (int j = minSize; j < nPresidentes; ++j) {
            Candidato* pres = p->candidatosPresidencia[j];
            res.push_back({pres, nullptr});
        }
    }
    
    return res;
}

// 5. Obtener candidatos a alcaldía por partido
// Retorna tuplas (Ciudad, Partido, vector de candidatos)
std::vector<std::tuple<Ciudad*, Partido*, std::vector<Candidato*>>> DatosElectoral::candidatosAlcaldiaPorPartido() {
    std::vector<std::tuple<Ciudad*, Partido*, std::vector<Candidato*>>> res;
    
    for (auto ciudad : ciudades) {
        for (auto cand : ciudad->candidatosAlcaldia) {
            Partido* p = cand->partido.get();
            
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
// Retorna par (Presidente, Vicepresidente) de un partido específico
std::pair<Candidato*, Candidato*> DatosElectoral::candidatosPresidenciaPorPartido(Partido* partido) {
    if (!partido) return {nullptr, nullptr};
    
    for (auto p : paises) {
        int nPresidentes = static_cast<int>(p->candidatosPresidencia.size());
        
        for (int j = 0; j < nPresidentes; ++j) {
            Candidato* pres = p->candidatosPresidencia[j];
            
            if (pres->partido.get() == partido) {
                Candidato* vice = nullptr;
                
                if (j < static_cast<int>(p->candidatosVicepresidencia.size())) {
                    vice = p->candidatosVicepresidencia[j];
                }
                
                return {pres, vice};
            }
        }
    }
    
    return {nullptr, nullptr};
}

// ============================================================================
// MÉTODOS ACCESORES (obtenerLista...)
// ============================================================================

// Obtener lista de países
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

// Obtener lista de candidatos a alcaldía
std::vector<Candidato*>& DatosElectoral::obtenerListaCandidatos() {
    return candidatosAlcaldia;
}

// Obtener lista de partidos
std::vector<Partido*>& DatosElectoral::obtenerListaPartidos() {
    return partidos;
}

