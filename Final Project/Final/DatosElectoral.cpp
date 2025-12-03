#include "DatosElectoral.h"
#include "UtilidadesAnalisis.h"

#include <sstream>
#include <memory>
#include <tuple>
#include <algorithm>

DatosElectoral::DatosElectoral() {}

DatosElectoral::~DatosElectoral() {
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

// Crear Pais (se inserta ordenado por peso)
Pais* DatosElectoral::crearPais(std::string& nombre, std::vector<Candidato*> candidatosPresidencia, std::vector<Candidato*> candidatosVicepresidencia) {
    Pais* p = new Pais();
    p->nombre = nombre;
    p->codigo = parses.hashToHex(nombre);
    p->candidatosPresidencia = candidatosPresidencia;
    p->candidatosVicepresidencia = candidatosVicepresidencia;

    insertOrdered(paises, p, true);
    return p;
}

// Crear Region; si paisPadre != nullptr, la agregamos a ese pais tambien
Region* DatosElectoral::crearRegion(std::string nombre, std::vector<Ciudad*> ciudades, Pais* padre) {
    if(padre == nullptr){
        return new Region();
    }

    Region* r = new Region();
    r->nombre = nombre;
    r->codigo = parses.hashToHex(nombre);
    r->ciudades = ciudades;
    r->censoElectoral = 0;
    
    insertOrdered(regiones, r,true);

    return r;
}

// Crear Ciudad; si regionPadre o paisPadre se pasan, se asocian
Ciudad* DatosElectoral::crearCiudad(std::string nombre, Region* regionPadre, std::vector<Candidato*> candidatos) {

    Ciudad* c = new Ciudad();
    c->nombre = nombre;
    c->codigo = parses.hashToHex(nombre);
    c->region = regionPadre;
    c->candidatosAlcaldia = candidatos;
    c->censoElectoral = 0;

    insertOrdered(ciudades, c, true);

    return c;
}

// Implementar como miembro para coincidir con la declaración en el header
Candidato* DatosElectoral::crearCandidato(Persona* persona, Partido* partido, Candidato* presidente) {
	
    Candidato* candidato = new Candidato();
    candidato->inf = std::make_shared<Persona>(*persona);
    candidato->codigo = parses.hashToHex(persona->identificacion); // 64-bit hash hex of identificacion
    candidato->partido = std::make_shared<Partido>(*partido);

    // Si se pasa un presidente, asociarlo como su vicepresidente (formula)
    if (presidente != nullptr) {
        presidente->vicepresidente = std::make_shared<Candidato>(*candidato);        
    }

    return candidato;
}

Partido* DatosElectoral::crearPartido(std::string nombre, Persona* persona, bool legal) {
    if(persona == nullptr)
        return new Partido();
    
    Partido* p = new Partido();
    p->nombre = nombre;
    p->codigo = parses.hashToHex(nombre);
    p->representanteLegal=std::make_shared<Persona>(*persona);
    p->legal = legal;

    insertOrdered(partidos, p, true);

    return p;
}

// ============================================================================
// MÉTODOS DE AGREGACIÓN (agregar...)
// ============================================================================

// Agregar candidato a ciudad (ordenado por identificacion -> parse int)
//TODO: Manejar de mejor manera esos condicionales, que sean mas explicitos a la hora de devolver las cosas
void DatosElectoral::agregarCandidatoACiudad(Candidato* candidato, Ciudad* ciudad, Region* region) {
    // ensure city exists in global cities vector
    if (std::find(ciudades.begin(), ciudades.end(), ciudad) == ciudades.end()) {
        insertOrdered(ciudades, ciudad, true);
    }

    // ensure city is in region
    if (std::find(region->ciudades.begin(), region->ciudades.end(), ciudad) == region->ciudades.end())
        return;

    // check if candidate already present
    if (std::find(ciudad->candidatosAlcaldia.begin(), ciudad->candidatosAlcaldia.end(), candidato) != ciudad->candidatosAlcaldia.end())
        return;

    insertOrdered(ciudad->candidatosAlcaldia, candidato, true);
    insertOrdered(region->candidatosAlcaldias, candidato, true);
}


// ============================================================================
// MÉTODOS DE CONSULTA (obtener... / consultar...)
// ============================================================================

// 1. Ciudades para las cuales se realizar� el proceso electoral.
//    Aqu� devuelvo todas las ciudades registradas en la lista global.
std::vector<Ciudad*> DatosElectoral::obtenerCiudadesElectorales() {
    std::vector<Ciudad*> res;
    for (auto c : ciudades) res.push_back(c);
    return res;
}

// 2. Partidos legalmente reconocidos.
//    Recorremos ciudades/regiones/paises buscando partidos (esto asume que los partidos est�n almacenados en candidatos->partido)
std::vector<Partido*> DatosElectoral::obtenerPartidosLegales() {
    std::vector<Partido*> encontrados;
    auto pushIfNuevo = [&](std::shared_ptr<Partido> p) {
        if (!p) return;
        if (!p->legal) return;
        Partido* ptr = p.get();
        for (auto &pp : encontrados) if (pp == ptr) return;   //------>> Se puede optimizar con otro tipo de busqueda
        encontrados.push_back(ptr);
    };

    // Recorremos candidatos por ciudad
    for (auto c : ciudades) {
        for (auto cand : c->candidatosAlcaldia) pushIfNuevo(cand->partido);
    }

    // Recorremos regiones
    for (auto r : regiones) {
        for (auto cand : r->candidatosAlcaldias) pushIfNuevo(cand->partido);
    }

    // Nota: Si mantienes una lista expl�cita de partidos, este m�todo ser�a trivial.
    return encontrados;
}

// 3. Todos los candidatos a la alcald�a de cada una de las ciudades.
std::vector<std::pair<Ciudad*, std::vector<Candidato*>*>> DatosElectoral::candidatosPorCiudad() {
    std::vector<std::pair<Ciudad*, std::vector<Candidato*>*>> res;
    for (auto c : ciudades) res.push_back({c, &c->candidatosAlcaldia});
    return res;
}

// 4. Todos los candidatos a la presidencia y vicepresidencia
std::vector<std::pair<Candidato*, Candidato*>> DatosElectoral::candidatosPresidenciales() {
    std::vector<std::pair<Candidato*, Candidato*>> res;
    int np = static_cast<int>(paises.size());
    for (int i = 0; i < np; ++i) {
        Pais* p = paises[i];
        int nPresidentes = static_cast<int>(p->candidatosPresidencia.size());
        int nVicepresidentes = static_cast<int>(p->candidatosVicepresidencia.size());
        
        // Emparejar presidentes con vicepresidentes
        int minSize = (nPresidentes < nVicepresidentes) ? nPresidentes : nVicepresidentes;
        for (int j = 0; j < minSize; ++j) {
            Candidato* pres = p->candidatosPresidencia[j];
            Candidato* vice = p->candidatosVicepresidencia[j];
            res.push_back({pres, vice});
        }
        
        // Si hay presidentes sin vicepresidente
        for (int j = minSize; j < nPresidentes; ++j) {
            Candidato* pres = p->candidatosPresidencia[j];
            res.push_back({pres, nullptr});
        }
    }
    return res;
}

// 5. Candidatos a cada una de las alcald�as, por partido.
std::vector<std::tuple<Ciudad*, Partido*, std::vector<Candidato*>>> DatosElectoral::candidatosAlcaldiaPorPartido() {
    std::vector<std::tuple<Ciudad*, Partido*, std::vector<Candidato*>>> res;

    int nc = static_cast<int>(ciudades.size());
    for (int i = 0; i < nc; ++i) {
        Ciudad* ciudad = ciudades[i];
        int numCand = static_cast<int>(ciudad->candidatosAlcaldia.size());
        // mapa simple por partido (iteramos y agrupamos)
        for (int j = 0; j < numCand; ++j) {
            Candidato* cand = ciudad->candidatosAlcaldia[j];
            Partido* p = cand->partido.get(); // Obtener raw pointer del shared_ptr
            // Buscar si ya existe tupla (ciudad, p)
            bool found = false;
            for (auto &t : res) {
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

// 6. Candidato a la presidencia y vicepresidencia por un partido dado.
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

// Accesores
std::vector<Pais*>& DatosElectoral::obtenerListaPaises() {
    return paises;
}
std::vector<Region*>& DatosElectoral::obtenerListaRegiones() {
    return regiones;
}
std::vector<Ciudad*>& DatosElectoral::obtenerListaCiudades() {
    return ciudades;
}

