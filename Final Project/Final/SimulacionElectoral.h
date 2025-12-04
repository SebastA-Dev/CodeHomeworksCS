#ifndef SIMULACION_ELECTORAL_H
#define SIMULACION_ELECTORAL_H

#include <map>
#include <vector>

class DatosElectoral;
class Pais;
class Ciudad;
class Candidato;

struct ResultadosCiudad {
    Ciudad* ciudad;
    std::map<Candidato*, int> votosAlcaldia;
    int votosEnBlancoAlcaldia = 0;
    int votosNulosAlcaldia = 0;
    int abstencionAlcaldia = 0;
    Candidato* ganadorAlcaldia = nullptr;
};

struct ResultadosNacionales {
    std::map<Candidato*, int> votosPresidencia;
    int votosEnBlancoPresidencia = 0;
    int votosNulosPresidencia = 0;
    int abstencionPresidencia = 0;
    Candidato* ganadorPresidencia = nullptr;
    bool requiereSegundaVuelta = false;
};

class SimulacionElectoral {
public:
    static void simularElecciones(DatosElectoral& sistema, Pais* pais);
    
private:
    static int generarVotosAleatorios(int censo, int& votosUsados);
    static void simularAlcaldias(DatosElectoral& sistema, std::vector<ResultadosCiudad>& resultados);
    static void simularPresidencia(Pais* pais, int totalCenso, ResultadosNacionales& resultados);
    static void mostrarResultadosAlcaldias(const std::vector<ResultadosCiudad>& resultados);
    static void mostrarResultadosPresidencia(const ResultadosNacionales& resultados, int totalCenso);
    static void mostrarEstadisticasAlcaldias(DatosElectoral& sistema, const std::vector<ResultadosCiudad>& resultados);
    static void mostrarEstadisticasPresidencia(Pais* pais, const ResultadosNacionales& resultados, int totalCenso);
};

#endif
