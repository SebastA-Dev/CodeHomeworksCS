#ifndef DATOS_ELECTORAL_H
#define DATOS_ELECTORAL_H

#include "Lista.h"
#include "EstructuraElectoral.h"
#include "UtilidadesAnalisis.h"

#include <string>
#include <vector>

class DatosElectoral {
private:
    UtilidadesAnalisis parses;

    Lista<Pais*> paises;
    Lista<Region*> regiones;  
    Lista<Ciudad*> ciudades;      
    Lista<Candidato*> candidatosPresidenciaLista;
    Lista<Candidato*> candidatosRegionales;


    Lista<Candidato*> candidatosAlcaldia;    

    unsigned long calcularPesoAscii(const std::string& texto) const;
    std::string pesoHex(const std::string& texto) const;

public:
    Lista<Partido*> partidos;  
    DatosElectoral();
    ~DatosElectoral();
    
    Pais* crearPais(std::string& nombre, Lista<Candidato*> candidatosPresidencia, Lista<Candidato*> candidatosViicepresidencia);
    Region* crearRegion(std::string nombre, Lista<Ciudad*> ciudades, Pais* padre);
    Ciudad* crearCiudad(std::string nombre, Region* regionPadre, Lista<Candidato*> candidatos);
    Candidato* crearCandidato(Persona* persona, Partido* partido, Candidato* presidencia = nullptr);
    Partido* crearPartido(std::string nombre, Persona* persona, bool legal = false);

    void agregarCandidatoACiudad(Candidato* candidato, Ciudad* ciudad, Region* region);

    // Consultas solicitadas:
    // 1. Ciudades para las cuales se realizar� el proceso electoral.
    std::vector<Ciudad*> obtenerCiudadesElectorales();

    // 2. Partidos legalmente reconocidos.
    std::vector<Partido*> obtenerPartidosLegales();

    // 3. Todos los candidatos a la alcald�a de cada una de las ciudades.
    //    Devuelve vector de pares (Ciudad*, Lista<Candidato*>&) -> aqu� uso vector de tuplas simples.
    std::vector<std::pair<Ciudad*, Lista<Candidato*>*>> candidatosPorCiudad();

    // 4. Todos los candidatos a la presidencia y vicepresidencia
    std::vector<std::pair<Candidato*, Candidato*>> candidatosPresidenciales(); // par: (presidente, vicepresidente)

    // 5. Candidatos a cada una de las alcald�as, por partido.
    //    Retorna un vector donde cada item es (Ciudad*, Partido*, vector<Candidato*>)
    std::vector<std::tuple<Ciudad*, Partido*, std::vector<Candidato*>>> candidatosAlcaldiaPorPartido();

    // 6. Candidato a la presidencia y vicepresidencia por un partido dado.
    std::pair<Candidato*, Candidato*> candidatosPresidenciaPorPartido(Partido* partido);

    // Accesores directos (si los necesitas)
    Lista<Pais*>& obtenerListaPaises();
    Lista<Region*>& obtenerListaRegiones();
    Lista<Ciudad*>& obtenerListaCiudades();
};

#endif

