#include "SimulacionElectoral.h"
#include "DatosElectoral.h"
#include "EstructuraElectoral.h"
#include "UtilidadesAnalisis.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>

struct VotoDetallado {
    Candidato* candidato;
    Sexo sexo;
    EstadoCivil estadoCivil;
    int edad;
};

int SimulacionElectoral::generarVotosAleatorios(int censo, int& votosUsados) {
    int votosDisponibles = censo - votosUsados;
    if (votosDisponibles <= 0) return 0;
    
    int votos = rand() % (votosDisponibles + 1);
    votosUsados += votos;
    return votos;
}

int calcularEdad(const std::tm& fechaNacimiento) {
    time_t ahora = time(0);
    struct tm* horaActual = localtime(&ahora);
    
    int edad = horaActual->tm_year - fechaNacimiento.tm_year;
    if (horaActual->tm_mon < fechaNacimiento.tm_mon ||
        (horaActual->tm_mon == fechaNacimiento.tm_mon && horaActual->tm_mday < fechaNacimiento.tm_mday)) {
        edad--;
    }
    return edad;
}

Sexo generarSexoAleatorio() {
    return (rand() % 2 == 0) ? Sexo::Masculino : Sexo::Femenino;
}

EstadoCivil generarEstadoCivilAleatorio() {
    int estado = rand() % 4;
    switch(estado) {
        case 0: return EstadoCivil::Soltero;
        case 1: return EstadoCivil::Casado;
        case 2: return EstadoCivil::Divorciado;
        default: return EstadoCivil::UnionLibre;
    }
}

void SimulacionElectoral::simularAlcaldias(DatosElectoral& sistema, std::vector<ResultadosCiudad>& resultados) {
    auto& ciudades = sistema.obtenerListaCiudades();
    
    for (auto ciudad : ciudades) {
        ResultadosCiudad resultado;
        resultado.ciudad = ciudad;
        int votosUsados = 0;
        
        // Generar votos aleatorios para cada candidato
        for (auto candidato : ciudad->candidatosAlcaldia) {
            int votos = generarVotosAleatorios(ciudad->censoElectoral, votosUsados);
            resultado.votosAlcaldia[candidato] = votos;
        }
        
        // Generar votos especiales (blanco, nulo, abstencion)
        resultado.votosEnBlancoAlcaldia = generarVotosAleatorios(ciudad->censoElectoral, votosUsados);
        resultado.votosNulosAlcaldia = generarVotosAleatorios(ciudad->censoElectoral, votosUsados);
        resultado.abstencionAlcaldia = ciudad->censoElectoral - votosUsados;
        
        // Encontrar ganador
        int votosMax = 0;
        for (auto& par : resultado.votosAlcaldia) {
            if (par.second > votosMax) {
                votosMax = par.second;
                resultado.ganadorAlcaldia = par.first;
            }
        }
        
        resultados.push_back(resultado);
    }
}

void SimulacionElectoral::simularPresidencia(Pais* pais, int totalCenso, ResultadosNacionales& resultados) {
    int votosUsados = 0;
    
    // Generar votos aleatorios para cada formula presidencial
    for (int i = 0; i < pais->candidatosPresidencia.size(); i++) {
        Candidato* pres = pais->candidatosPresidencia[i];
        int votos = generarVotosAleatorios(totalCenso, votosUsados);
        resultados.votosPresidencia[pres] = votos;
    }
    
    // Generar votos especiales
    resultados.votosEnBlancoPresidencia = generarVotosAleatorios(totalCenso, votosUsados);
    resultados.votosNulosPresidencia = generarVotosAleatorios(totalCenso, votosUsados);
    resultados.abstencionPresidencia = totalCenso - votosUsados;
    
    // Encontrar ganador y verificar segunda vuelta
    int votosMax = 0;
    int segundoMayor = 0;
    Candidato* ganador = nullptr;
    
    for (auto& par : resultados.votosPresidencia) {
        if (par.second > votosMax) {
            segundoMayor = votosMax;
            votosMax = par.second;
            ganador = par.first;
        } else if (par.second > segundoMayor) {
            segundoMayor = par.second;
        }
    }
    
    resultados.ganadorPresidencia = ganador;
    
    double porcentajeGanador = totalCenso > 0 ? (votosMax * 100.0) / totalCenso : 0;
    if (porcentajeGanador < 50.0) {
        resultados.requiereSegundaVuelta = true;
    }
}

void SimulacionElectoral::mostrarResultadosAlcaldias(const std::vector<ResultadosCiudad>& resultados) {
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "         RESULTADOS DE ALCALDIAS" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    
    for (const auto& res : resultados) {
        int totalVotos = res.votosEnBlancoAlcaldia + res.votosNulosAlcaldia + res.abstencionAlcaldia;
        for (const auto& par : res.votosAlcaldia) {
            totalVotos += par.second;
        }
        
        std::cout << "\n[CIUDAD: " << res.ciudad->nombre << "]" << std::endl;
        std::cout << "Total votantes: " << totalVotos << " / Censo: " << res.ciudad->censoElectoral << std::endl;
        std::cout << "Participacion: " << std::fixed << std::setprecision(1) 
                  << ((totalVotos * 100.0) / res.ciudad->censoElectoral) << "%" << std::endl;
        
        std::cout << "\nCandidatos:" << std::endl;
        for (const auto& par : res.votosAlcaldia) {
            double porcentaje = totalVotos > 0 ? (par.second * 100.0) / totalVotos : 0;
            std::cout << "  " << par.first->nombre << " " << par.first->apellido 
                      << ": " << par.second << " votos (" << porcentaje << "%)" << std::endl;
        }
        
        std::cout << "\nVotos especiales:" << std::endl;
        std::cout << "  Votos en blanco: " << res.votosEnBlancoAlcaldia << std::endl;
        std::cout << "  Votos nulos: " << res.votosNulosAlcaldia << std::endl;
        std::cout << "  Abstencion: " << res.abstencionAlcaldia << std::endl;
        
        if (res.ganadorAlcaldia) {
            int votosGanador = res.votosAlcaldia.at(res.ganadorAlcaldia);
            double porcGanador = totalVotos > 0 ? (votosGanador * 100.0) / totalVotos : 0;
            std::cout << "\n*** GANADOR: " << res.ganadorAlcaldia->nombre << " " 
                      << res.ganadorAlcaldia->apellido << " (" << votosGanador << " votos - " 
                      << porcGanador << "%) ***" << std::endl;
        }
    }
}

void SimulacionElectoral::mostrarResultadosPresidencia(const ResultadosNacionales& resultados, int totalCenso) {
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "       RESULTADOS PRESIDENCIALES" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    
    int totalVotos = resultados.votosEnBlancoPresidencia + resultados.votosNulosPresidencia + 
                     resultados.abstencionPresidencia;
    for (const auto& par : resultados.votosPresidencia) {
        totalVotos += par.second;
    }
    
    std::cout << "\nTotal votantes: " << totalVotos << " / Censo: " << totalCenso << std::endl;
    std::cout << "Participacion: " << std::fixed << std::setprecision(1) 
              << ((totalVotos * 100.0) / totalCenso) << "%" << std::endl;
    
    std::cout << "\nFormulas presidenciales:" << std::endl;
    for (const auto& par : resultados.votosPresidencia) {
        double porcentaje = totalVotos > 0 ? (par.second * 100.0) / totalVotos : 0;
        std::cout << "  " << par.first->nombre << " " << par.first->apellido 
                  << ": " << par.second << " votos (" << porcentaje << "%)" << std::endl;
    }
    
    std::cout << "\nVotos especiales:" << std::endl;
    std::cout << "  Votos en blanco: " << resultados.votosEnBlancoPresidencia << std::endl;
    std::cout << "  Votos nulos: " << resultados.votosNulosPresidencia << std::endl;
    std::cout << "  Abstencion: " << resultados.abstencionPresidencia << std::endl;
    
    if (resultados.ganadorPresidencia) {
        int votosGanador = resultados.votosPresidencia.at(resultados.ganadorPresidencia);
        double porcGanador = totalVotos > 0 ? (votosGanador * 100.0) / totalVotos : 0;
        std::cout << "\n*** GANADOR: " << resultados.ganadorPresidencia->nombre << " " 
                  << resultados.ganadorPresidencia->apellido << " (" << votosGanador 
                  << " votos - " << porcGanador << "%) ***" << std::endl;
    }
    
    if (resultados.requiereSegundaVuelta) {
        std::cout << "\n!!! SEGUNDA VUELTA REQUERIDA - Ningun candidato alcanzo 50% !!!" << std::endl;
    }
}

void SimulacionElectoral::mostrarEstadisticasAlcaldias(DatosElectoral& sistema, const std::vector<ResultadosCiudad>& resultados) {
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "     ESTADISTICAS POR REGION" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    
    auto& regiones = sistema.obtenerListaRegiones();
    
    for (auto region : regiones) {
        std::cout << "\n[REGION: " << region->nombre << "]" << std::endl;
        
        int totalVotos = 0;
        int totalBlancos = 0;
        int totalNulos = 0;
        int totalAbst = 0;
        int totalCenso = 0;
        
        for (const auto& res : resultados) {
            if (res.ciudad->region == region) {
                int votsCiudad = res.votosEnBlancoAlcaldia + res.votosNulosAlcaldia + res.abstencionAlcaldia;
                for (const auto& par : res.votosAlcaldia) {
                    votsCiudad += par.second;
                }
                
                totalVotos += votsCiudad;
                totalBlancos += res.votosEnBlancoAlcaldia;
                totalNulos += res.votosNulosAlcaldia;
                totalAbst += res.abstencionAlcaldia;
                totalCenso += res.ciudad->censoElectoral;
            }
        }
        
        std::cout << "  Total votos: " << totalVotos << " / Censo: " << totalCenso << std::endl;
        std::cout << "  Votos en blanco: " << totalBlancos << std::endl;
        std::cout << "  Votos nulos: " << totalNulos << std::endl;
        std::cout << "  Abstencion: " << totalAbst << std::endl;
        
        if (totalCenso > 0) {
            std::cout << "  Participacion: " << std::fixed << std::setprecision(1) 
                      << ((totalVotos * 100.0) / totalCenso) << "%" << std::endl;
        }
    }
}

void SimulacionElectoral::mostrarEstadisticasPresidencia(Pais* pais, const ResultadosNacionales& resultados, int totalCenso) {
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "   ESTADISTICAS PRESIDENCIALES NACIONALES" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    
    int totalVotos = resultados.votosEnBlancoPresidencia + resultados.votosNulosPresidencia + 
                     resultados.abstencionPresidencia;
    for (const auto& par : resultados.votosPresidencia) {
        totalVotos += par.second;
    }
    
    std::cout << "\nTotal votantes: " << totalVotos << std::endl;
    std::cout << "Total censo: " << totalCenso << std::endl;
    std::cout << "Participacion nacional: " << std::fixed << std::setprecision(2) 
              << ((totalVotos * 100.0) / totalCenso) << "%" << std::endl;
    
    std::cout << "\nTasa de votos especiales:" << std::endl;
    std::cout << "  Votos en blanco: " << std::fixed << std::setprecision(2)
              << ((resultados.votosEnBlancoPresidencia * 100.0) / totalCenso) << "%" << std::endl;
    std::cout << "  Votos nulos: " << ((resultados.votosNulosPresidencia * 100.0) / totalCenso) << "%" << std::endl;
    std::cout << "  Abstencion: " << ((resultados.abstencionPresidencia * 100.0) / totalCenso) << "%" << std::endl;
}

void SimulacionElectoral::simularElecciones(DatosElectoral& sistema, Pais* pais) {
    srand(time(0));
    
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "  INICIANDO SIMULACION DE ELECCIONES" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    
    // Calcular censo total
    int totalCenso = 0;
    auto& ciudades = sistema.obtenerListaCiudades();
    for (auto ciudad : ciudades) {
        totalCenso += ciudad->censoElectoral;
    }
    
    std::cout << "\nCenso electoral total: " << totalCenso << " votantes" << std::endl;
    std::cout << "Ciudades participantes: " << ciudades.size() << std::endl;
    std::cout << "Regiones participantes: " << sistema.obtenerListaRegiones().size() << std::endl;
    
    // Simular elecciones
    std::vector<ResultadosCiudad> resultadosAlcaldias;
    simularAlcaldias(sistema, resultadosAlcaldias);
    
    ResultadosNacionales resultadosPresidencia;
    simularPresidencia(pais, totalCenso, resultadosPresidencia);
    
    // Mostrar resultados
    mostrarResultadosAlcaldias(resultadosAlcaldias);
    mostrarResultadosPresidencia(resultadosPresidencia, totalCenso);
    
    // Mostrar estadisticas
    mostrarEstadisticasAlcaldias(sistema, resultadosAlcaldias);
    mostrarEstadisticasPresidencia(pais, resultadosPresidencia, totalCenso);
    
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "         SIMULACION COMPLETADA" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    std::cout << "\nProceso electoral finalizado exitosamente." << std::endl;
}
