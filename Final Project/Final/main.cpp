#include "DatosElectoral.h"

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

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
    
    // ============================================================================
    // CREANDO 5 PERSONAS
    // ============================================================================
    
    std::cout << "\n=== Creando Personas ===" << std::endl;
    
    // Persona 1
    Persona* persona1 = new Persona();
    persona1->nombre = "Juan";
    persona1->apellido = "Perez";
    persona1->identificacion = "123456789";
    persona1->sexo = Sexo::Masculino;
    persona1->estadoCivil = EstadoCivil::Soltero;
    std::tm fecha1 = {};
    fecha1.tm_year = 90; // 1990
    fecha1.tm_mon = 4;   // Mayo
    fecha1.tm_mday = 15;
    persona1->fechaNacimiento = fecha1;
    std::cout << "Persona 1 creada: " << persona1->nombre << " " << persona1->apellido << std::endl;
    
    // Persona 2
    Persona* persona2 = new Persona();
    persona2->nombre = "Maria";
    persona2->apellido = "Garcia";
    persona2->identificacion = "987654321";
    persona2->sexo = Sexo::Femenino;
    persona2->estadoCivil = EstadoCivil::Casado;
    std::tm fecha2 = {};
    fecha2.tm_year = 92; // 1992
    fecha2.tm_mon = 7;   // Agosto
    fecha2.tm_mday = 22;
    persona2->fechaNacimiento = fecha2;
    std::cout << "Persona 2 creada: " << persona2->nombre << " " << persona2->apellido << std::endl;
    
    // Persona 3
    Persona* persona3 = new Persona();
    persona3->nombre = "Pedro";
    persona3->apellido = "Lopez";
    persona3->identificacion = "555555555";
    persona3->sexo = Sexo::Masculino;
    persona3->estadoCivil = EstadoCivil::Divorciado;
    std::tm fecha3 = {};
    fecha3.tm_year = 88; // 1988
    fecha3.tm_mon = 2;   // Marzo
    fecha3.tm_mday = 10;
    persona3->fechaNacimiento = fecha3;
    std::cout << "Persona 3 creada: " << persona3->nombre << " " << persona3->apellido << std::endl;
    
    // Persona 4
    Persona* persona4 = new Persona();
    persona4->nombre = "Ana";
    persona4->apellido = "Martinez";
    persona4->identificacion = "444444444";
    persona4->sexo = Sexo::Femenino;
    persona4->estadoCivil = EstadoCivil::Soltero;
    std::tm fecha4 = {};
    fecha4.tm_year = 95; // 1995 
    fecha4.tm_mon = 11;  // Diciembre
    fecha4.tm_mday = 5;
    persona4->fechaNacimiento = fecha4;
    std::cout << "Persona 4 creada: " << persona4->nombre << " " << persona4->apellido << std::endl;
    
    // Persona 5
    Persona* persona5 = new Persona();
    persona5->nombre = "Carlos";
    persona5->apellido = "Sanchez";
    persona5->identificacion = "666666666";
    persona5->sexo = Sexo::Masculino;
    persona5->estadoCivil = EstadoCivil::Viudo;
    std::tm fecha5 = {};
    fecha5.tm_year = 85; // 1985
    fecha5.tm_mon = 0;   // Enero
    fecha5.tm_mday = 20;
    persona5->fechaNacimiento = fecha5;
    std::cout << "Persona 5 creada: " << persona5->nombre << " " << persona5->apellido << std::endl;
    
    Partido* liberal = sistema.crearPartido("Liberal", persona1, true);    
    Partido* as = sistema.crearPartido("4", persona3, true);
    Partido* ea = sistema.crearPartido("32", persona4, true);
    Partido* afd = sistema.crearPartido("1", persona5, true);
    Partido* conservador = sistema.crearPartido("Conservador", persona2, true);

    // ============================================================================
    // MOSTRANDO PARTIDOS EN LA LISTA PÚBLICA DEL SISTEMA
    // ============================================================================
    std::cout << "\n=== PARTIDOS EN sistema.partidos ===" << std::endl;
    std::cout << "Total de partidos en la lista: " << sistema.partidos.size() << std::endl;
    
    for (size_t i = 0; i < sistema.partidos.size(); ++i) {
        Partido* p = sistema.partidos[i];
        std::cout << "\nPartido " << (i + 1) << ":" << std::endl;
        std::cout << "  Nombre: " << p->nombre << std::endl;
        std::cout << "  Codigo: " << p->codigo << std::endl;
        std::cout << "  Legal: " << (p->legal ? "Si" : "No") << std::endl;
        if (p->representanteLegal) {
            std::cout << "  Representante Legal: " << p->representanteLegal->nombre 
                      << " " << p->representanteLegal->apellido << std::endl;
        }
    }

    // ============================================================================
    // BUSCAR POSICION DE "Liberal" USANDO LA BÚSQUEDA BINARIA (por codigo hex)
    // ============================================================================
    std::string hexLiberal = UtilidadesAnalisis::hashToHex(std::string("Liberal"));
    int posicion = UtilidadesAnalisis::busquedaBinariaCodigo(sistema.partidos, hexLiberal);
    if (posicion >= 0) {
        Partido* encontrado = sistema.partidos[posicion];
        std::cout << "\nBusqueda: 'Liberal' -> codigo " << hexLiberal << " encontrado en posicion " << posicion << "." << std::endl;
        if (encontrado) std::cout << "  Partido en esa posicion: " << encontrado->nombre << std::endl;
    } else {
        std::cout << "\nBusqueda: 'Liberal' -> codigo " << hexLiberal << " NO encontrado en la lista." << std::endl;
    }

    // ============================================================================
    // TEST: Crear partidos adicionales para alcaldes y candidatos regionales
    // ============================================================================
    std::cout << "\n=== Creando Partidos Adicionales ===" << std::endl;
    Partido* socialista = sistema.crearPartido("Socialista", persona3, true);
    Partido* verde = sistema.crearPartido("Verde", persona4, true);
    Partido* centro = sistema.crearPartido("Centro", persona5, true);
    std::cout << "Partidos adicionales creados: Socialista, Verde, Centro" << std::endl;

    // ============================================================================
    // TEST: Crear 1 pais con 5 regiones, 2 ciudades por region y candidatos
    // ============================================================================
    std::cout << "\n=== TEST: Crear Pais/Regiones/Ciudades con Candidatos ===" << std::endl;
    std::vector<Candidato*> emptyCandidates;
    std::string paisNombre = "RepublicaTest";
    Pais* pais = sistema.crearPais(paisNombre, emptyCandidates, emptyCandidates);
    std::cout << "Pais creado: " << pais->nombre << std::endl;

    // Contador para crear personas y candidatos unicos
    int personaId = 100;
    
    // Crear 5 regiones y 2 ciudades por region, con candidatos para alcaldias
    for (int ri = 1; ri <= 5; ++ri) {
        std::string nombreRegion = "Region " + std::to_string(ri);
        Region* region = sistema.crearRegion(nombreRegion, std::vector<Ciudad*>(), pais);
        region->pais = pais; // Vincular region al pais
        std::cout << "\n  Creada " << nombreRegion << std::endl;
        
        for (int ci = 1; ci <= 2; ++ci) {
            std::string nombreCiudad = nombreRegion + " - Ciudad " + std::to_string(ci);
            
            // Crear 2 candidatos para la alcaldia de esta ciudad
            std::vector<Candidato*> candidatosCiudad;
            for (int candi = 1; candi <= 2; ++candi) {
                // Crear persona para el candidato
                Persona* personaCandidato = new Persona();
                personaCandidato->nombre = "Candidato" + std::to_string(personaId);
                personaCandidato->apellido = nombreCiudad;
                personaCandidato->identificacion = std::to_string(personaId) + "00000";
                personaCandidato->sexo = (candi % 2 == 0) ? Sexo::Femenino : Sexo::Masculino;
                personaCandidato->estadoCivil = EstadoCivil::Soltero;
                std::tm fechaC = {};
                fechaC.tm_year = 90 + candi;
                fechaC.tm_mon = (candi % 12);
                fechaC.tm_mday = 15 + candi;
                personaCandidato->fechaNacimiento = fechaC;
                
                // Crear candidato con partido correspondiente
                Partido* partidoCandidato = (candi == 1) ? liberal : socialista;
                Candidato* candidato = sistema.crearCandidato(personaCandidato, partidoCandidato);
                candidato->tipo = Candidato::TipoCandidato::ALCALDIA;
                candidatosCiudad.push_back(candidato);
                personaId++;
            }
            
            // Crear ciudad con sus candidatos
            Ciudad* ciudad = sistema.crearCiudad(nombreCiudad, region, candidatosCiudad);
            ciudad->censoElectoral = 50000 + (ri * 1000) + (ci * 100);
            region->ciudades.push_back(ciudad);
            
            // Agregar candidatos a la region
            for (auto &cand : candidatosCiudad) {
                region->candidatosAlcaldias.push_back(cand);
            }
            
            std::cout << "    Creada " << nombreCiudad << " con " << candidatosCiudad.size() 
                      << " candidatos (censo: " << ciudad->censoElectoral << ")" << std::endl;
        }
        
        region->censoElectoral = 100000 + (ri * 2000);
    }
    
    std::cout << "\nPais " << pais->nombre << " completamente poblado." << std::endl;

    // Iterar sobre paises/regiones/ciudades usando los accesores
    auto &paises = sistema.obtenerListaPaises();
    std::cout << "\n=== ITERATING OVER PAISES ===" << std::endl;
    std::cout << "Total paises: " << paises.size() << std::endl;
    for (size_t pi = 0; pi < paises.size(); ++pi) {
        Pais* p = paises[pi];
        std::cout << "\nPais " << pi << ": " << p->nombre << " (codigo=" << p->codigo << ")" << std::endl;
        std::cout << "  Candidatos Presidencia: " << p->candidatosPresidencia.size() << std::endl;
        std::cout << "  Candidatos Vicepresidencia: " << p->candidatosVicepresidencia.size() << std::endl;
    }

    auto &regiones = sistema.obtenerListaRegiones();
    std::cout << "\n=== ITERATING OVER REGIONES ===" << std::endl;
    std::cout << "Total regiones: " << regiones.size() << std::endl;
    for (size_t ri = 0; ri < regiones.size(); ++ri) {
        Region* r = regiones[ri];
        std::cout << "\nRegion " << ri << ": " << r->nombre << " (codigo=" << r->codigo << ")" << std::endl;
        std::cout << "  Censo electoral: " << r->censoElectoral << std::endl;
        std::cout << "  Ciudades en la region: " << r->ciudades.size() << std::endl;
        std::cout << "  Candidatos alcaldias: " << r->candidatosAlcaldias.size() << std::endl;
        for (size_t cii = 0; cii < r->ciudades.size(); ++cii) {
            Ciudad* cc = r->ciudades[cii];
            std::cout << "    Ciudad " << cii << ": " << cc->nombre 
                      << " (codigo=" << cc->codigo << ", censo=" << cc->censoElectoral 
                      << ", candidatos=" << cc->candidatosAlcaldia.size() << ")" << std::endl;
            for (size_t candi = 0; candi < cc->candidatosAlcaldia.size(); ++candi) {
                Candidato* c = cc->candidatosAlcaldia[candi];
                std::cout << "      Candidato " << candi << ": " 
                          << c->inf->nombre << " " << c->inf->apellido 
                          << " (partido: " << c->partido->nombre << ")" << std::endl;
            }
        }
    }

    auto &ciudades = sistema.obtenerListaCiudades();
    std::cout << "\n=== ITERATING OVER CIUDADES (GLOBAL) ===" << std::endl;
    std::cout << "Total ciudades globales: " << ciudades.size() << std::endl;
    for (size_t ci = 0; ci < ciudades.size(); ++ci) {
        Ciudad* c = ciudades[ci];
        std::cout << "Ciudad global " << ci << ": " << c->nombre 
                  << " (codigo=" << c->codigo << ", censo=" << c->censoElectoral 
                  << ", candidatos=" << c->candidatosAlcaldia.size() << ")" << std::endl;
    }

    // Elegir una ciudad aleatoria (o intermedia) y buscarla con busqueda binaria
    std::cout << "\n=== BINARY SEARCH TEST: Buscar ciudad ===" << std::endl;
    if (!ciudades.empty()) {
        size_t targetIndex = ciudades.size() / 2; // elegir la del medio
        std::string targetCodigo = ciudades[targetIndex]->codigo;
        std::cout << "Buscando ciudad en posicion " << targetIndex << ": " << ciudades[targetIndex]->nombre 
                  << " con codigo " << targetCodigo << std::endl;
        int found = UtilidadesAnalisis::busquedaBinariaCodigo(ciudades, targetCodigo);
        std::cout << "Resultado de busqueda binaria: index = " << found << std::endl;
        if (found >= 0) {
            std::cout << "  Ciudad encontrada: " << ciudades[found]->nombre << std::endl;
        } else {
            std::cout << "  Ciudad NO encontrada." << std::endl;
        }
    }

    return 0;
}
