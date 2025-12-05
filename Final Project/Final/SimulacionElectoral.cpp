#include "SimulacionElectoral.h"
#include "DatosElectoral.h"
#include "EstructuraElectoral.h"
#include "UtilidadesAnalisis.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <vector>

struct VotoDetallado {
    Candidato* candidato;
    Sexo sexo;
    EstadoCivil estadoCivil;
    int edad;
};

int SimulacionElectoral::generarVotosAleatorios(int totalDisponibles, int& votosUsados, float maxPorcentaje) {
    // Asegurar que no excedamos el límite de votos disponibles
    int votosDisponibles = totalDisponibles - votosUsados;
    if (votosDisponibles <= 0) return 0;
    
    // Calcular el máximo de votos que podemos asignar basado en el porcentaje máximo
    int maxVotosPermitidos = static_cast<int>(votosDisponibles * maxPorcentaje);
    if (maxVotosPermitidos < 0) maxVotosPermitidos = 0;
    
    // Generar votos aleatorios dentro del rango permitido
    int votos = 0;
    if (maxVotosPermitidos > 0) {
        votos = rand() % (maxVotosPermitidos + 1);
    }
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

// Función auxiliar para generar estadísticas de género
EstadisticasGenero calcularEstadisticasGenero(int totalVotos) {
    EstadisticasGenero stats;
    
    if (totalVotos > 0) {
        // Distribución típica: aproximadamente 48-52% femenino, 48-52% masculino
        int baseFemenino = totalVotos * 48 / 100;
        int variacion = totalVotos * 4 / 100;
        
        stats.votosFemeninos = baseFemenino + (rand() % (variacion * 2 + 1)) - variacion;
        stats.votosFemeninos = std::max(0, std::min(totalVotos, stats.votosFemeninos));
        stats.votosMasculinos = totalVotos - stats.votosFemeninos;
        
        stats.porcentajeFemenino = (stats.votosFemeninos * 100.0) / totalVotos;
        stats.porcentajeMasculino = (stats.votosMasculinos * 100.0) / totalVotos;
    }
    
    return stats;
}

void SimulacionElectoral::simularAlcaldias(DatosElectoral& sistema, std::vector<ResultadosCiudad>& resultados) {
    auto& ciudades = sistema.obtenerListaCiudades();
    
    for (auto ciudad : ciudades) {
        ResultadosCiudad resultado;
        resultado.ciudad = ciudad;
        int votosUsados = 0;
        
        // Calcular participación aleatoria entre 80% y 100%
        float participacion = 0.8f + (rand() % 21) / 100.0f; // 80-100%
        int votosTotalesPermitidos = static_cast<int>(ciudad->censoElectoral * participacion);
        
        // Distribuir votos entre candidatos (total debe ser <= votosTotalesPermitidos)
        int votosCandidatosUsados = 0;
        int numCandidatos = ciudad->candidatosAlcaldia.size();
        
        for (auto candidato : ciudad->candidatosAlcaldia) {
            // Para el último candidato, asignar los votos restantes
            if (--numCandidatos == 0) {
                int votos = votosTotalesPermitidos - votosCandidatosUsados;
                resultado.votosAlcaldia[candidato] = std::max(0, votos);
                votosUsados += std::max(0, votos);
            } else {
                // Calcular votos disponibles para este candidato
                int votosDisponibles = votosTotalesPermitidos - votosCandidatosUsados;
                float porcentajeMax = 1.0f / (numCandidatos + 1); // Distribuir equitativamente
                
                int votos = generarVotosAleatorios(votosDisponibles, votosCandidatosUsados, porcentajeMax);
                resultado.votosAlcaldia[candidato] = votos;
                votosUsados += votos;
            }
        }
        
        // Generar votos especiales con los votos restantes hasta votosTotalesPermitidos
        int votosRestantes = votosTotalesPermitidos - votosUsados;
        if (votosRestantes > 0) {
            // Distribuir votos restantes entre blanco, nulo y abstencion
            resultado.votosEnBlancoAlcaldia = generarVotosAleatorios(votosRestantes, votosUsados, 0.4f);
            resultado.votosNulosAlcaldia = generarVotosAleatorios(votosRestantes - resultado.votosEnBlancoAlcaldia, 
                                                                 votosUsados, 0.4f);
        }
        
        // La abstencion es la diferencia entre el censo total y los votos usados
        resultado.abstencionAlcaldia = ciudad->censoElectoral - votosUsados;
        
        // Calcular estadísticas de género para esta ciudad
        int totalVotosCiudad = votosUsados;
        resultado.estadisticasGenero = calcularEstadisticasGenero(totalVotosCiudad);
        
        // Encontrar ganador
        int votosMax = 0;
        resultado.ganadorAlcaldia = nullptr;
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
    
    // Calcular participación aleatoria entre 80% y 100%
    float participacion = 0.8f + (rand() % 21) / 100.0f; // 80-100%
    int votosTotalesPermitidos = static_cast<int>(totalCenso * participacion);
    
    // Distribuir votos entre fórmulas presidenciales
    int votosCandidatosUsados = 0;
    int numCandidatos = pais->candidatosPresidencia.size();
    
    for (int i = 0; i < pais->candidatosPresidencia.size(); i++) {
        Candidato* pres = pais->candidatosPresidencia[i];
        
        if (--numCandidatos == 0) {
            // Último candidato obtiene los votos restantes
            int votos = votosTotalesPermitidos - votosCandidatosUsados;
            resultados.votosPresidencia[pres] = std::max(0, votos);
            votosUsados += std::max(0, votos);
        } else {
            // Distribuir equitativamente entre los candidatos restantes
            int votosDisponibles = votosTotalesPermitidos - votosCandidatosUsados;
            float porcentajeMax = 1.0f / (numCandidatos + 1);
            
            int votos = generarVotosAleatorios(votosDisponibles, votosCandidatosUsados, porcentajeMax);
            resultados.votosPresidencia[pres] = votos;
            votosUsados += votos;
        }
    }
    
    // Distribuir votos especiales con los votos restantes
    int votosRestantes = votosTotalesPermitidos - votosUsados;
    if (votosRestantes > 0) {
        resultados.votosEnBlancoPresidencia = generarVotosAleatorios(votosRestantes, votosUsados, 0.4f);
        resultados.votosNulosPresidencia = generarVotosAleatorios(votosRestantes - resultados.votosEnBlancoPresidencia,
                                                                 votosUsados, 0.4f);
    }
    
    // La abstención es la diferencia total
    resultados.abstencionPresidencia = totalCenso - votosUsados;
    
    // Calcular estadísticas de género para la presidencia
    int totalVotosPresidenciales = votosUsados;
    resultados.estadisticasGenero = calcularEstadisticasGenero(totalVotosPresidenciales);
    
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
    resultados.requiereSegundaVuelta = false;
    
    // Solo verificar segunda vuelta si hay votos suficientes
    if (votosTotalesPermitidos > 0) {
        double porcentajeGanador = (votosMax * 100.0) / votosTotalesPermitidos;
        if (porcentajeGanador < 50.0) {
            resultados.requiereSegundaVuelta = true;
        }
    }
}

void SimulacionElectoral::mostrarResultadosAlcaldias(const std::vector<ResultadosCiudad>& resultados) {
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "         RESULTADOS DE ALCALDIAS" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    
    for (const auto& res : resultados) {
        int totalVotos = res.votosEnBlancoAlcaldia + res.votosNulosAlcaldia;
        for (const auto& par : res.votosAlcaldia) {
            totalVotos += par.second;
        }
        
        double participacion = (totalVotos * 100.0) / res.ciudad->censoElectoral;
        
        std::cout << "\n[CIUDAD: " << res.ciudad->nombre << "]" << std::endl;
        std::cout << "Censo: " << res.ciudad->censoElectoral << std::endl;
        std::cout << "Votantes: " << totalVotos << std::endl;
        std::cout << "Participacion: " << std::fixed << std::setprecision(1) 
                  << participacion << "%" << std::endl;
        std::cout << "Abstencion: " << std::fixed << std::setprecision(1)
                  << (100.0 - participacion) << "%" << std::endl;
        
        // Mostrar estadísticas de género
        std::cout << "\nDistribucion por genero:" << std::endl;
        std::cout << "  Votos masculinos: " << res.estadisticasGenero.votosMasculinos 
                  << " (" << std::fixed << std::setprecision(1) << res.estadisticasGenero.porcentajeMasculino << "%)" << std::endl;
        std::cout << "  Votos femeninos: " << res.estadisticasGenero.votosFemeninos 
                  << " (" << std::fixed << std::setprecision(1) << res.estadisticasGenero.porcentajeFemenino << "%)" << std::endl;
        
        std::cout << "\nCandidatos:" << std::endl;
        for (const auto& par : res.votosAlcaldia) {
            double porcentaje = totalVotos > 0 ? (par.second * 100.0) / totalVotos : 0;
            std::cout << "  " << par.first->nombre << " " << par.first->apellido 
                      << ": " << par.second << " votos (" << porcentaje << "%)" << std::endl;
        }
        
        std::cout << "\nVotos especiales:" << std::endl;
        std::cout << "  Votos en blanco: " << res.votosEnBlancoAlcaldia 
                  << " (" << (totalVotos > 0 ? (res.votosEnBlancoAlcaldia * 100.0) / totalVotos : 0) << "%)" << std::endl;
        std::cout << "  Votos nulos: " << res.votosNulosAlcaldia 
                  << " (" << (totalVotos > 0 ? (res.votosNulosAlcaldia * 100.0) / totalVotos : 0) << "%)" << std::endl;
        
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
    
    int totalVotos = resultados.votosEnBlancoPresidencia + resultados.votosNulosPresidencia;
    for (const auto& par : resultados.votosPresidencia) {
        totalVotos += par.second;
    }
    
    double participacion = (totalVotos * 100.0) / totalCenso;
    
    std::cout << "\nTotal censo: " << totalCenso << std::endl;
    std::cout << "Total votantes: " << totalVotos << std::endl;
    std::cout << "Participacion: " << std::fixed << std::setprecision(1) 
              << participacion << "%" << std::endl;
    std::cout << "Abstencion: " << std::fixed << std::setprecision(1)
              << (100.0 - participacion) << "%" << std::endl;
    
    // Mostrar estadísticas de género
    std::cout << "\nDistribucion por genero:" << std::endl;
    std::cout << "  Votos masculinos: " << resultados.estadisticasGenero.votosMasculinos 
              << " (" << std::fixed << std::setprecision(1) << resultados.estadisticasGenero.porcentajeMasculino << "%)" << std::endl;
    std::cout << "  Votos femeninos: " << resultados.estadisticasGenero.votosFemeninos 
              << " (" << std::fixed << std::setprecision(1) << resultados.estadisticasGenero.porcentajeFemenino << "%)" << std::endl;
    
    std::cout << "\nFormulas presidenciales:" << std::endl;
    for (const auto& par : resultados.votosPresidencia) {
        double porcentaje = totalVotos > 0 ? (par.second * 100.0) / totalVotos : 0;
        std::cout << "  " << par.first->nombre << " " << par.first->apellido 
                  << ": " << par.second << " votos (" << porcentaje << "%)" << std::endl;
    }
    
    std::cout << "\nVotos especiales:" << std::endl;
    std::cout << "  Votos en blanco: " << resultados.votosEnBlancoPresidencia 
              << " (" << (totalVotos > 0 ? (resultados.votosEnBlancoPresidencia * 100.0) / totalVotos : 0) << "%)" << std::endl;
    std::cout << "  Votos nulos: " << resultados.votosNulosPresidencia 
              << " (" << (totalVotos > 0 ? (resultados.votosNulosPresidencia * 100.0) / totalVotos : 0) << "%)" << std::endl;
    
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
        int votosMasculinos = 0;
        int votosFemeninos = 0;
        
        for (const auto& res : resultados) {
            if (res.ciudad->region == region) {
                int votsCiudad = res.votosEnBlancoAlcaldia + res.votosNulosAlcaldia;
                for (const auto& par : res.votosAlcaldia) {
                    votsCiudad += par.second;
                }
                
                totalVotos += votsCiudad;
                totalBlancos += res.votosEnBlancoAlcaldia;
                totalNulos += res.votosNulosAlcaldia;
                totalAbst += res.abstencionAlcaldia;
                totalCenso += res.ciudad->censoElectoral;
                
                // Acumular votos por género
                votosMasculinos += res.estadisticasGenero.votosMasculinos;
                votosFemeninos += res.estadisticasGenero.votosFemeninos;
            }
        }
        
        std::cout << "  Total censo: " << totalCenso << std::endl;
        std::cout << "  Total votantes: " << totalVotos << std::endl;
        std::cout << "  Votos en blanco: " << totalBlancos << std::endl;
        std::cout << "  Votos nulos: " << totalNulos << std::endl;
        std::cout << "  Abstencion: " << totalAbst << std::endl;
        
        // Mostrar estadísticas de género por región
        if (totalVotos > 0) {
            double porcentajeMasculino = (votosMasculinos * 100.0) / totalVotos;
            double porcentajeFemenino = (votosFemeninos * 100.0) / totalVotos;
            
            std::cout << "\n  Distribucion por genero en la region:" << std::endl;
            std::cout << "    Votos masculinos: " << votosMasculinos 
                      << " (" << std::fixed << std::setprecision(1) << porcentajeMasculino << "%)" << std::endl;
            std::cout << "    Votos femeninos: " << votosFemeninos 
                      << " (" << std::fixed << std::setprecision(1) << porcentajeFemenino << "%)" << std::endl;
        }
        
        if (totalCenso > 0) {
            double participacion = (totalVotos * 100.0) / totalCenso;
            std::cout << "\n  Participacion: " << std::fixed << std::setprecision(1) 
                      << participacion << "%" << std::endl;
            std::cout << "  Abstencion regional: " << std::fixed << std::setprecision(1)
                      << (100.0 - participacion) << "%" << std::endl;
        }
    }
}

void SimulacionElectoral::mostrarEstadisticasPresidencia(Pais* pais, const ResultadosNacionales& resultados, int totalCenso) {
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "   ESTADISTICAS PRESIDENCIALES NACIONALES" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    
    int totalVotos = resultados.votosEnBlancoPresidencia + resultados.votosNulosPresidencia;
    for (const auto& par : resultados.votosPresidencia) {
        totalVotos += par.second;
    }
    
    double participacion = (totalVotos * 100.0) / totalCenso;
    
    std::cout << "\nTotal censo: " << totalCenso << std::endl;
    std::cout << "Total votantes: " << totalVotos << std::endl;
    std::cout << "Participacion nacional: " << std::fixed << std::setprecision(2) 
              << participacion << "%" << std::endl;
    std::cout << "Abstencion nacional: " << std::fixed << std::setprecision(2)
              << (100.0 - participacion) << "%" << std::endl;
    
    std::cout << "\nDistribucion de votos por genero a nivel nacional:" << std::endl;
    std::cout << "  Votos masculinos: " << resultados.estadisticasGenero.votosMasculinos 
              << " (" << std::fixed << std::setprecision(2) << resultados.estadisticasGenero.porcentajeMasculino << "% de los votos)" << std::endl;
    std::cout << "  Votos femeninos: " << resultados.estadisticasGenero.votosFemeninos 
              << " (" << std::fixed << std::setprecision(2) << resultados.estadisticasGenero.porcentajeFemenino << "% de los votos)" << std::endl;
    
    std::cout << "\nTasa de votos especiales:" << std::endl;
    std::cout << "  Votos en blanco: " << std::fixed << std::setprecision(2)
              << ((resultados.votosEnBlancoPresidencia * 100.0) / totalCenso) << "% del censo" << std::endl;
    std::cout << "  Votos nulos: " << ((resultados.votosNulosPresidencia * 100.0) / totalCenso) << "% del censo" << std::endl;
    
    // Mostrar distribución por candidato con análisis de género
    std::cout << "\nDistribucion por candidato:" << std::endl;
    for (const auto& par : resultados.votosPresidencia) {
        double porcentajeCenso = (par.second * 100.0) / totalCenso;
        double porcentajeVotos = totalVotos > 0 ? (par.second * 100.0) / totalVotos : 0;
        std::cout << "  " << par.first->nombre << " " << par.first->apellido 
                  << ": " << porcentajeCenso << "% del censo, " 
                  << porcentajeVotos << "% de los votos validos" << std::endl;
    }
    
    // Análisis adicional de género por candidato (simplificado)
    std::cout << "\nAnalisis de genero por candidato (estimado):" << std::endl;
    for (const auto& par : resultados.votosPresidencia) {
        // Estimación simple: distribución similar a la nacional
        int votosMasculinosEstimados = par.second * resultados.estadisticasGenero.porcentajeMasculino / 100;
        int votosFemeninosEstimados = par.second * resultados.estadisticasGenero.porcentajeFemenino / 100;
        
        std::cout << "  " << par.first->nombre << " " << par.first->apellido 
                  << ": " << votosMasculinosEstimados << " votos masculinos, "
                  << votosFemeninosEstimados << " votos femeninos" << std::endl;
    }
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