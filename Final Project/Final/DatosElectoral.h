#ifndef DATOS_ELECTORAL_H
#define DATOS_ELECTORAL_H

#include "EstructuraElectoral.h"
#include "UtilidadesAnalisis.h"
#include <fstream>
#include <sstream>
#include <ctime>

#include <string>
#include <vector>

class DatosElectoral {
private:
	
    // Utilidades
    UtilidadesAnalisis parses;
    
    // Listas 
    
    std::vector<Pais*> paises;
    std::vector<Region*> regiones;
    std::vector<Ciudad*> ciudades;
    std::vector<Candidato*> candidatosAlcaldia;
    std::vector<Candidato*> candidatosPresidenciaLista;
    std::vector<Partido*> partidos;
    
    
    // M�todos auxiliares 
    
    unsigned long calcularPesoAscii(const std::string& texto) const;
    std::string pesoHex(const std::string& texto) const;

public:
	
    
    // Constructor y destructor
    DatosElectoral();
    ~DatosElectoral();
    
    // ===== M�TODOS CREACI�N =====
    
    // Crear Pa�s
    Pais* crearPais(std::string& nombre, 
                    std::vector<Candidato*> candidatosPresidencia, 
                    std::vector<Candidato*> candidatosVicepresidencia);
    
    // Crear Regi�n 
    
    Region* crearRegion(std::string nombre, 
                        std::vector<Ciudad*> ciudades, 
                        Pais* padre);
    
    // Crear Ciudad 
    
    Ciudad* crearCiudad(std::string nombre, 
                        Region* regionPadre, 
                        std::vector<Candidato*> candidatos);
    
    // Crear Candidato
    // IM,PORTANTE: Retorna nullptr si el candidato no es valido
	Candidato* crearCandidato(std::string nombre, 
    	                      std::string apellido,
        	                  std::string identificacion,
            	              Sexo sexo,
                	          EstadoCivil estadoCivil,
                    	      std::tm fechaNacimiento,
                        	  Ciudad* ciudadNacimiento,
                	          Ciudad* ciudadResidencia,
                    	      Partido* partido,
                        	  TipoCandidato tipo,                         	  
                              Pais* pais,
                              Ciudad* ciudadAspirante = nullptr,
                              Candidato* vicepresidente = nullptr);
    
    // Crear Partido
    Partido* crearPartido(std::string nombre, 
                          std::string representanteLegal, 
                          bool legal = false);
    
    // ===== M�TODOS AGREGACI�N =====
    
    // Agregar candidato a una ciudad
    
    void agregarCandidatoACiudad(Candidato* candidato, 
                                  Ciudad* ciudad, 
                                  Region* region);
    
    // ===== M�TODOS DE CONSULTA =====
    
    // 1. Ciudades electorales (con censo > 0)
    std::vector<Ciudad*> obtenerCiudadesElectorales();
    
    // 2. Partidos legalmente reconocidos
    std::vector<Partido*> obtenerPartidosLegales();
    
    // 3. Candidatos por ciudad 
    std::vector<std::pair<Ciudad*, std::vector<Candidato*>*>> candidatosPorCiudad();
    
    // 4. Candidatos presidenciales 
    
    std::vector<std::pair<Candidato*, Candidato*>> candidatosPresidenciales();
    
    // 5. Candidatos a alcald�a por partido
    // Retorna tuplas
    std::vector<std::tuple<Ciudad*, Partido*, std::vector<Candidato*>>> candidatosAlcaldiaPorPartido();
    
    // 6. Candidatos presidenciales de un partido espec�fico
    std::pair<Candidato*, Candidato*> candidatosPresidenciaPorPartido(Partido* partido);
    
    // ===== M�TODOS ACCESORES =====
    
    std::vector<Pais*>& obtenerListaPaises();
    std::vector<Region*>& obtenerListaRegiones();
    std::vector<Ciudad*>& obtenerListaCiudades();
    std::vector<Candidato*>& obtenerListaCandidatos();
    std::vector<Partido*>& obtenerListaPartidos();
};
#endif

