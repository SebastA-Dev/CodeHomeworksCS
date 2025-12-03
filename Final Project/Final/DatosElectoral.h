#ifndef DATOS_ELECTORAL_H
#define DATOS_ELECTORAL_H

#include "EstructuraElectoral.h"
#include "UtilidadesAnalisis.h"

#include <string>
#include <vector>

class DatosElectoral {
private:
    UtilidadesAnalisis parses;

    std::vector<Pais*> paises;
    std::vector<Region*> regiones;
    std::vector<Ciudad*> ciudades;
    std::vector<Candidato*> candidatosPresidenciaLista;
    std::vector<Candidato*> candidatosRegionales;

    std::vector<Candidato*> candidatosAlcaldia;

    unsigned long calcularPesoAscii(const std::string& texto) const;
    std::string pesoHex(const std::string& texto) const;

public:
    std::vector<Partido*> partidos; 
    DatosElectoral();
    ~DatosElectoral();
    
    Pais* crearPais(std::string& nombre, std::vector<Candidato*> candidatosPresidencia, std::vector<Candidato*> candidatosViicepresidencia); // Funciona
    Region* crearRegion(std::string nombre, std::vector<Ciudad*> ciudades, Pais* padre); // Funciona
    Ciudad* crearCiudad(std::string nombre, Region* regionPadre, std::vector<Candidato*> candidatos); // Funciona
    Candidato* crearCandidato(Persona* persona, Partido* partido, Candidato* presidencia = nullptr); 
    Partido* crearPartido(std::string nombre, Persona* persona, bool legal = false); // Funciona

    void agregarCandidatoACiudad(Candidato* candidato, Ciudad* ciudad, Region* region);

    // Consultas solicitadas:
    // 1. Ciudades para las cuales se realizar� el proceso electoral.
    std::vector<Ciudad*> obtenerCiudadesElectorales();

    // 2. Partidos legalmente reconocidos.
    std::vector<Partido*> obtenerPartidosLegales();

    // 3. Todos los candidatos a la alcald�a de cada una de las ciudades.
    //    Devuelve vector de pares (Ciudad*, Lista<Candidato*>&) -> aqu� uso vector de tuplas simples.
    std::vector<std::pair<Ciudad*, std::vector<Candidato*>*>> candidatosPorCiudad();

    // 4. Todos los candidatos a la presidencia y vicepresidencia
    std::vector<std::pair<Candidato*, Candidato*>> candidatosPresidenciales(); // par: (presidente, vicepresidente)

    // 5. Candidatos a cada una de las alcald�as, por partido.
    //    Retorna un vector donde cada item es (Ciudad*, Partido*, vector<Candidato*>)
    std::vector<std::tuple<Ciudad*, Partido*, std::vector<Candidato*>>> candidatosAlcaldiaPorPartido();

    // 6. Candidato a la presidencia y vicepresidencia por un partido dado.
    std::pair<Candidato*, Candidato*> candidatosPresidenciaPorPartido(Partido* partido);

    // Accesores directos (si los necesitas)
    std::vector<Pais*>& obtenerListaPaises();
    std::vector<Region*>& obtenerListaRegiones();
    std::vector<Ciudad*>& obtenerListaCiudades();
};

#endif

