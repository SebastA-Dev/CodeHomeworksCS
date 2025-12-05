#ifndef SIMULACIONELECTORAL_H
#define SIMULACIONELECTORAL_H

#include "DatosElectoral.h"
#include "EstructuraElectoral.h"
#include <vector>
#include <map>

// Estructura para estadísticas de género
struct EstadisticasGenero {
    int votosMasculinos = 0;
    int votosFemeninos = 0;
    double porcentajeMasculino = 0.0;
    double porcentajeFemenino = 0.0;
};

struct ResultadosCiudad {
    Ciudad* ciudad;
    std::map<Candidato*, int> votosAlcaldia;
    int votosEnBlancoAlcaldia = 0;
    int votosNulosAlcaldia = 0;
    int abstencionAlcaldia = 0;
    Candidato* ganadorAlcaldia = nullptr;
    EstadisticasGenero estadisticasGenero;  // Nueva: estadísticas por género
};

struct ResultadosNacionales {
    std::map<Candidato*, int> votosPresidencia;
    int votosEnBlancoPresidencia = 0;
    int votosNulosPresidencia = 0;
    int abstencionPresidencia = 0;
    Candidato* ganadorPresidencia = nullptr;
    bool requiereSegundaVuelta = false;
    EstadisticasGenero estadisticasGenero;  // Nueva: estadísticas por género
};

class SimulacionElectoral {
public:
    static int generarVotosAleatorios(int totalDisponibles, int& votosUsados, float maxPorcentaje = 1.0f);
    
    static void simularAlcaldias(DatosElectoral& sistema, std::vector<ResultadosCiudad>& resultados);
    static void simularPresidencia(Pais* pais, int totalCenso, ResultadosNacionales& resultados);
    
    static void mostrarResultadosAlcaldias(const std::vector<ResultadosCiudad>& resultados);
    static void mostrarResultadosPresidencia(const ResultadosNacionales& resultados, int totalCenso);
    
    static void mostrarEstadisticasAlcaldias(DatosElectoral& sistema, const std::vector<ResultadosCiudad>& resultados);
    static void mostrarEstadisticasPresidencia(Pais* pais, const ResultadosNacionales& resultados, int totalCenso);
    
    static void simularElecciones(DatosElectoral& sistema, Pais* pais);
};

#endif // SIMULACIONELECTORAL_H