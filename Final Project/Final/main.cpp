#include "DatosElectoral.h"

#include <iostream>
#include <fstream>
#include <string>

void abrirArchivo(const std::string& url) {
    std::ifstream archivo(url);

    if (!archivo.is_open()) {
        std::cout << "No se pudo abrir el archivo." << std::endl;
        return;
    }

    std::string linea;
    while (std::getline(archivo, linea)) {
        std::cout << linea << std::endl;
    }

    archivo.close();
}

int main(int argc, char** argv) {
    DatosElectoral sistema;
    
    // TODO: Corregir lo de los pesos, los hexadecimales estan dando problemas

    // 1. Create parties first
    std::cout << "\n=== Creando Partidos ===" << std::endl;
    Partido* liberal = new Partido();
    liberal->nombre = "Partido Liberal";
    liberal->legal = true;
    std::cout << "Partido creado: " << liberal->nombre << std::endl;
    
    Partido* conservador = new Partido();
    conservador->nombre = "Partido Conservador";
    conservador->legal = true;
    std::cout << "Partido creado: " << conservador->nombre << std::endl;
    
    // 2. Create presidential candidates
    std::cout << "\n=== Creando Candidatos Presidenciales ===" << std::endl;
    Candidato* presidente = new Candidato();
    presidente->nombre = "Diego";
    presidente->apellido = "Fernandez";
    presidente->codigo = "333333333";
    presidente->sexo = 'M';
    presidente->partido = liberal;
    std::cout << "Candidato presidencial creado: " << presidente->nombre << " " << presidente->apellido << std::endl;
    
    Candidato* vicepresidente = new Candidato();
    vicepresidente->nombre = "Ana";
    vicepresidente->apellido = "Martinez";
    vicepresidente->codigo = "444444444";
    vicepresidente->sexo = 'F';
    vicepresidente->partido = liberal;
    std::cout << "Candidato vicepresidencial creado: " << vicepresidente->nombre << " " << vicepresidente->apellido << std::endl;
    
    // 3. Create lists for presidential candidates
    Lista<Candidato*> candidatosPresidencia;
    Lista<Candidato*> candidatosVicepresidencia;
    candidatosPresidencia.insertarOrden(presidente);
    candidatosVicepresidencia.insertarOrden(vicepresidente);
    
    // 4. Create country with presidential candidates
    std::cout << "\n=== Creando Pais ===" << std::endl;
    Pais* pais = sistema.crearPais("Colombia", candidatosPresidencia, candidatosVicepresidencia);
    std::cout << "Pais creado: " << pais->nombre << std::endl;
    
    // 5. Create regions
    std::cout << "\n=== Creando Regiones ===" << std::endl;
    Lista<Ciudad*> ciudadesRegion;
    Region* region1 = sistema.crearRegion("Bogota", ciudadesRegion, pais);
    Region* region2 = sistema.crearRegion("Medellin", ciudadesRegion, pais);
    std::cout << "Region creada: " << region1->nombre << std::endl;
    std::cout << "Region creada: " << region2->nombre << std::endl;
    
    // 6. Create cities
    std::cout << "\n=== Creando Ciudades ===" << std::endl;
    Lista<Candidato*> candidatos;
    Ciudad* ciudad1 = sistema.crearCiudad("Bogota Centro", region1, candidatos);
    Ciudad* ciudad2 = sistema.crearCiudad("Bogota Kennedy", region1, candidatos);
    Ciudad* ciudad3 = sistema.crearCiudad("Medellin Centro", region2, candidatos);
    std::cout << "Ciudad creada: " << ciudad1->nombre << std::endl;
    std::cout << "Ciudad creada: " << ciudad2->nombre << std::endl;
    std::cout << "Ciudad creada: " << ciudad3->nombre << std::endl;
    
    // 7. Create mayoral candidates and add to cities
    std::cout << "\n=== Creando Candidatos a Alcaldia ===" << std::endl;
    
    // Add candidates to city1
    Candidato* cand1 = new Candidato();
    cand1->nombre = "Juan";
    cand1->apellido = "Perez";
    cand1->codigo = "123456789";
    cand1->sexo = 'M';
    cand1->partido = liberal;
    sistema.agregarCandidatoACiudad(cand1, ciudad1, region1);
    std::cout << "Candidato agregado: " << cand1->nombre << " " << cand1->apellido << std::endl;
    
    Candidato* cand2 = new Candidato();
    cand2->nombre = "Maria";
    cand2->apellido = "Garcia";
    cand2->codigo = "987654321";
    cand2->sexo = 'F';
    cand2->partido = conservador;
    sistema.agregarCandidatoACiudad(cand2, ciudad1, region1);
    std::cout << "Candidato agregado: " << cand2->nombre << " " << cand2->apellido << std::endl;
    
    // Add candidates to city2
    Candidato* cand3 = new Candidato();
    cand3->nombre = "Pedro";
    cand3->apellido = "Lopez";
    cand3->codigo = "555555555";
    cand3->sexo = 'M';
    cand3->partido = liberal;
    sistema.agregarCandidatoACiudad(cand3, ciudad2, region1);
    std::cout << "Candidato agregado: " << cand3->nombre << " " << cand3->apellido << std::endl;
    
    // 5. Query methods
    std::cout << "\n=== Consultando Ciudades Electorales ===" << std::endl;
    auto ciudadesElectorales = sistema.obtenerCiudadesElectorales();
    std::cout << "Total ciudades electorales: " << ciudadesElectorales.size() << std::endl;
    for (auto ciudad : ciudadesElectorales) {
        std::cout << "  - " << ciudad->nombre << std::endl;
    }
    
    std::cout << "\n=== Consultando Partidos Legales ===" << std::endl;
    auto partidosLegales = sistema.obtenerPartidosLegales();
    std::cout << "Total partidos legales: " << partidosLegales.size() << std::endl;
    for (auto partido : partidosLegales) {
        std::cout << "  - " << partido->nombre << std::endl;
    }
    
    std::cout << "\n=== Consultando Candidatos por Ciudad ===" << std::endl;
    auto candPorCiudad = sistema.candidatosPorCiudad();
    for (auto par : candPorCiudad) {
        std::cout << "Ciudad: " << par.first->nombre << " - Candidatos: " << par.second->obtenerTam() << std::endl;
        for (int i = 0; i < par.second->obtenerTam(); ++i) {
            Candidato* c = par.second->obtenerInfo(i);
            std::cout << "    - " << c->nombre << " " << c->apellido << " (" << c->partido->nombre << ")" << std::endl;
        }
    }
    
    std::cout << "\n=== Consultando Candidatos Presidenciales ===" << std::endl;
    auto candPresidenciales = sistema.candidatosPresidenciales();
    std::cout << "Total formulas presidenciales: " << candPresidenciales.size() << std::endl;
    for (auto formula : candPresidenciales) {
        std::cout << "Presidente: " << formula.first->nombre << " " << formula.first->apellido;
        if (formula.second != nullptr) {
            std::cout << " | Vicepresidente: " << formula.second->nombre << " " << formula.second->apellido;
        }
        std::cout << std::endl;
    }
    
    std::cout << "\n=== Consultando Candidatos Alcaldia por Partido ===" << std::endl;
    auto candAlcaldiaPorPartido = sistema.candidatosAlcaldiaPorPartido();
    for (auto tupla : candAlcaldiaPorPartido) {
        Ciudad* c = std::get<0>(tupla);
        Partido* p = std::get<1>(tupla);
        auto candidatos = std::get<2>(tupla);
        std::cout << "Ciudad: " << c->nombre << " | Partido: " << p->nombre << " | Candidatos: " << candidatos.size() << std::endl;
        for (auto cand : candidatos) {
            std::cout << "    - " << cand->nombre << " " << cand->apellido << std::endl;
        }
    }
    
    std::cout << "\n=== Consultando Candidatos Presidenciales por Partido ===" << std::endl;
    auto candPresPartido = sistema.candidatosPresidenciaPorPartido(liberal);
    if (candPresPartido.first != nullptr) {
        std::cout << "Presidente: " << candPresPartido.first->nombre << " " << candPresPartido.first->apellido;
        if (candPresPartido.second != nullptr) {
            std::cout << " | Vicepresidente: " << candPresPartido.second->nombre << " " << candPresPartido.second->apellido;
        }
        std::cout << std::endl;
    }
    
    std::cout << "\n=== Accediendo a Listas ===" << std::endl;
    Lista<Pais*>& paises = sistema.obtenerListaPaises();
    Lista<Region*>& regiones = sistema.obtenerListaRegiones();
    Lista<Ciudad*>& ciudadesLista = sistema.obtenerListaCiudades();
    
    std::cout << "Total paises: " << paises.obtenerTam() << std::endl;
    std::cout << "Total regiones: " << regiones.obtenerTam() << std::endl;
    std::cout << "Total ciudades: " << ciudadesLista.obtenerTam() << std::endl;
    
    return 0;
}
