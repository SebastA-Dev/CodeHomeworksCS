#include "Lista.h" 
#include "DatosElectoral.h"
#include "UtilidadesAnalisis.h"

#include <sstream>
#include <tuple>

DatosElectoral::DatosElectoral() {}

DatosElectoral::~DatosElectoral() {
}

// Helper: suma ascii de string
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

 // Crear Pais (se inserta ordenado por peso)
 Pais* DatosElectoral::crearPais(const std::string& nombre, Lista<Lista<Candidato*>> candidatosPresidencia) {
     Pais* p = new Pais();
     p->nombre = nombre;
     p->codigo = parses.stringToHex(nombre);
     p->candidatosPresidencia = candidatosPresidencia;

     paises.insertarOrden(p, true);
     return p;
 }

// Crear Region; si paisPadre != nullptr, la agregamos a ese pais tambien
Region* DatosElectoral::crearRegion(const std::string& nombre, Lista<Ciudad*> ciudades, Pais* padre) {
    if(padre == nullptr){
        return new Region();
    }

    Region* r = new Region();
    r->nombre = nombre;
    r->codigo = parses.stringToHex(nombre);
    r->ciudades = ciudades;
    r->censoElectoral = 0;
    
    regiones.insertarOrden(r,true);

    return r;
}

// Crear Ciudad; si regionPadre o paisPadre se pasan, se asocian
Ciudad* DatosElectoral::crearCiudad(const std::string& nombre, Region* regionPadre, Lista<Candidato*> candidatos) {

    if(regionPadre == nullptr){
        return new Ciudad();
    }

    Ciudad* c = new Ciudad();
    c->nombre = nombre;
    c->codigo = parses.stringToHex(nombre);
    c->region = regionPadre;
    c->candidatosAlcaldia = candidatos;
    c->censoElectoral = 0;

    ciudades.insertarOrden(c, true);

    return c;
}

// Agregar candidato a ciudad (ordenado por identificacion -> parse int)
//TODO: Manejar de mejor manera esos condicionales, que sean mas explicitos a la hora de devolver las cosas
void DatosElectoral::agregarCandidatoACiudad(Candidato* candidato, Ciudad* ciudad, Region* region) {
    if (ciudades.seEncuentra(ciudad) == false){
        ciudades.insertarOrden(ciudad);
    }

    if(region->ciudades.seEncuentra(ciudad) == false)
        return;

    if(ciudad->candidatosAlcaldia.seEncuentra(candidato))
        return;

    ciudad->candidatosAlcaldia.insertarOrden(candidato);
    region->candidatosAlcaldias.insertarOrden(candidato);
}


// ---------- Consultas ----------

// 1. Ciudades para las cuales se realizar� el proceso electoral.
//    Aqu� devuelvo todas las ciudades registradas en la lista global.
std::vector<Ciudad*> DatosElectoral::obtenerCiudadesElectorales() {
    std::vector<Ciudad*> res;
    int n = ciudades.obtenerTam();
    for (int i = 0; i < n; ++i) {
        res.push_back(ciudades.obtenerInfo(i));
    }
    return res;
}

// 2. Partidos legalmente reconocidos.
//    Recorremos ciudades/regiones/paises buscando partidos (esto asume que los partidos est�n almacenados en candidatos->partido)
std::vector<Partido*> DatosElectoral::obtenerPartidosLegales() {
    std::vector<Partido*> encontrados;
    auto pushIfNuevo = [&](Partido* p) {
        if (!p) return;
        if (!p->legal) return;
        for (auto &pp : encontrados) if (pp == p) return;
        encontrados.push_back(p);
    };

    // Recorremos candidatos por ciudad
    int nCiudades = ciudades.obtenerTam();
    for (int i = 0; i < nCiudades; ++i) {
        Ciudad* c = ciudades.obtenerInfo(i);
        int nc = c->candidatosAlcaldia.obtenerTam();
        for (int j = 0; j < nc; ++j) {
            Candidato* cand = c->candidatosAlcaldia.obtenerInfo(j);
            pushIfNuevo(cand->partido);
        }
    }

    // Recorremos regiones
    int nReg = regiones.obtenerTam();
    for (int i = 0; i < nReg; ++i) {
        Region* r = regiones.obtenerInfo(i);
        int nr = r->candidatosAlcaldias.obtenerTam();
        for (int j = 0; j < nr; ++j) {
            Candidato* cand = r->candidatosAlcaldias.obtenerInfo(j);
            pushIfNuevo(cand->partido);
        }
    }

    // Nota: Si mantienes una lista expl�cita de partidos, este m�todo ser�a trivial.
    return encontrados;
}

// 3. Todos los candidatos a la alcald�a de cada una de las ciudades.
std::vector<std::pair<Ciudad*, Lista<Candidato*>*>> DatosElectoral::candidatosPorCiudad() {
    std::vector<std::pair<Ciudad*, Lista<Candidato*>*>> res;
    int n = ciudades.obtenerTam();
    for (int i = 0; i < n; ++i) {
        Ciudad* c = ciudades.obtenerInfo(i);
        res.push_back({c, &c->candidatosAlcaldia});
    }
    return res;
}

// 4. Todos los candidatos a la presidencia y vicepresidencia
std::vector<std::pair<Candidato*, Candidato*>> DatosElectoral::candidatosPresidenciales() {
    std::vector<std::pair<Candidato*, Candidato*>> res;
    int np = paises.obtenerTam();
    for (int i = 0; i < np; ++i) {
        Pais* p = paises.obtenerInfo(i);
        int nlistas = p->candidatosPresidencia.obtenerTam();
        for (int j = 0; j < nlistas; ++j) {
            Lista<Candidato*> listaFormula = p->candidatosPresidencia.obtenerInfo(j);
            if (listaFormula.obtenerTam() >= 2) {
                Candidato* pres = listaFormula.obtenerInfo(0);
                Candidato* vice = listaFormula.obtenerInfo(1);
                res.push_back({pres, vice});
            } else if (listaFormula.obtenerTam() == 1) {
                Candidato* pres = listaFormula.obtenerInfo(0);
                res.push_back({pres, nullptr});
            }
        }
    }
    return res;
}

// 5. Candidatos a cada una de las alcald�as, por partido.
std::vector<std::tuple<Ciudad*, Partido*, std::vector<Candidato*>>> DatosElectoral::candidatosAlcaldiaPorPartido() {
    std::vector<std::tuple<Ciudad*, Partido*, std::vector<Candidato*>>> res;

    int nc = ciudades.obtenerTam();
    for (int i = 0; i < nc; ++i) {
        Ciudad* ciudad = ciudades.obtenerInfo(i);
        int numCand = ciudad->candidatosAlcaldia.obtenerTam();
        // mapa simple por partido (iteramos y agrupamos)
        for (int j = 0; j < numCand; ++j) {
            Candidato* cand = ciudad->candidatosAlcaldia.obtenerInfo(j);
            Partido* p = cand->partido;
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

    int np = paises.obtenerTam();
    for (int i = 0; i < np; ++i) {
        Pais* p = paises.obtenerInfo(i);
        int nlistas = p->candidatosPresidencia.obtenerTam();
        for (int j = 0; j < nlistas; ++j) {
            Lista<Candidato*> listaFormula = p->candidatosPresidencia.obtenerInfo(j);
            if (listaFormula.obtenerTam() >= 1) {
                Candidato* pres = listaFormula.obtenerInfo(0);
                Candidato* vice = (listaFormula.obtenerTam() >= 2) ? listaFormula.obtenerInfo(1) : nullptr;
                if (pres->partido == partido) {
                    return {pres, vice};
                }
            }
        }
    }

    return {nullptr, nullptr};
}

// Accesores
Lista<Pais*>& DatosElectoral::obtenerListaPaises() {
    return paises;
}
Lista<Region*>& DatosElectoral::obtenerListaRegiones() {
    return regiones;
}
Lista<Ciudad*>& DatosElectoral::obtenerListaCiudades() {
    return ciudades;
}

