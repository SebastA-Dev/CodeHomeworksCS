#include "UtilidadesAnalisis.h"
#include "EstructuraElectoral.h"
#include <sstream>
#include <iomanip>
#include <functional>
#include <vector>
#include <iostream>

std::string UtilidadesAnalisis::stringToHex(const std::string &input) {
    std::ostringstream oss;
    for (unsigned char c : input) {        
        oss << std::hex << std::setw(2) << std::setfill('0')
            << static_cast<int>(c);
    }
    return oss.str();
}

int UtilidadesAnalisis::calcularEdad(const std::tm& fechaNacimiento) {
    time_t ahora = std::time(nullptr);
    std::tm* fechaActual = std::localtime(&ahora);
    
    int edad = fechaActual->tm_year - fechaNacimiento.tm_year;
    
    if (fechaActual->tm_mon < fechaNacimiento.tm_mon ||
        (fechaActual->tm_mon == fechaNacimiento.tm_mon && 
         fechaActual->tm_mday < fechaNacimiento.tm_mday)) {
        edad--;
    }
    
    return edad;
}

unsigned long long UtilidadesAnalisis::hashToULL(const std::string &input) {
    std::hash<std::string> hasher;
    auto h = hasher(input);
    return static_cast<unsigned long long>(h);
}

std::string UtilidadesAnalisis::hashToHex(const std::string &input) {
    unsigned long long v = hashToULL(input);
    std::ostringstream oss;
    oss << std::hex << std::uppercase << v;
    return oss.str();
}

// ============================================================================
// FUNCIONES DE IMPRESIÓN
// ============================================================================

void UtilidadesAnalisis::imprimirTarjetonAlcaldia(Ciudad* ciudad) {
    if (!ciudad) {
        std::cerr << "Ciudad no válida" << std::endl;
        return;
    }
    
    std::cout << "\n";
    std::cout << "+--------------------------------------------+" << std::endl;
    std::cout << "¦       TARJETÓN DE ALCALDÍA - " 
              << std::setw(20) << std::left << ciudad->nombre << "¦" << std::endl;
    std::cout << "¦--------------------------------------------¦" << std::endl;
    std::cout << "¦  0. VOTO EN BLANCO                         ¦" << std::endl;
    
    int opcion = 1;
    for (auto candidato : ciudad->candidatosAlcaldia) {
        std::string nombreCompleto = candidato->nombre + " " + candidato->apellido;
        std::cout << "¦  " << opcion << ". " 
                  << std::setw(37) << std::left << nombreCompleto << "¦" << std::endl;
        opcion++;
    }
    
    std::cout << "+--------------------------------------------+" << std::endl;
}

void UtilidadesAnalisis::imprimirTarjetonPresidencia(Pais* pais) {
    if (!pais) {
        std::cerr << "País no válido" << std::endl;
        return;
    }
    
    std::cout << "\n";
    std::cout << "+------------------------------------------------------------+" << std::endl;
    std::cout << "¦       TARJETÓN DE PRESIDENCIA - " 
              << std::setw(25) << std::left << pais->nombre << "¦" << std::endl;
    std::cout << "¦------------------------------------------------------------¦" << std::endl;
    std::cout << "¦  0. VOTO EN BLANCO                                         ¦" << std::endl;
    
    int opcion = 1;
    int minSize = (pais->candidatosPresidencia.size() < pais->candidatosVicepresidencia.size())
                  ? pais->candidatosPresidencia.size()
                  : pais->candidatosVicepresidencia.size();
    
    for (int i = 0; i < minSize; i++) {
        Candidato* pres = pais->candidatosPresidencia[i];
        Candidato* vice = pais->candidatosVicepresidencia[i];
        
        std::string linea = pres->partido->nombre + " - " + pres->nombre + " - " + vice->nombre;
        
        std::cout << "¦  " << opcion << ". " 
                  << std::setw(56) << std::left << linea << "¦" << std::endl;
        opcion++;
    }
    
    std::cout << "+------------------------------------------------------------+" << std::endl;
}

void UtilidadesAnalisis::imprimirCensoElectoral(const std::vector<Ciudad*>& ciudades) {
    std::cout << "\n";
    std::cout << "+--------------------------------------------+" << std::endl;
    std::cout << "¦       CENSO ELECTORAL POR CIUDAD           ¦" << std::endl;
    std::cout << "¦--------------------------------------------¦" << std::endl;
    
    long totalNacional = 0;
    
    for (auto ciudad : ciudades) {
        std::cout << "¦ " << std::setw(26) << std::left << ciudad->nombre 
                  << "¦ " << std::setw(11) << std::right << ciudad->censoElectoral << " ¦" << std::endl;
        totalNacional += ciudad->censoElectoral;
    }
    
    std::cout << "¦-----------------------------+--------------¦" << std::endl;
    std::cout << "¦ " << std::setw(26) << std::left << "TOTAL NACIONAL" 
              << "¦ " << std::setw(11) << std::right << totalNacional << " ¦" << std::endl;
    std::cout << "+--------------------------------------------+" << std::endl;
}

void UtilidadesAnalisis::imprimirCandidatosPartidoRegion(
    const std::vector<Candidato*>& candidatos, Partido* partido, Region* region) {
    
    std::cout << "\n=== CANDIDATOS A ALCALDÍA - " << partido->nombre 
              << " EN " << region->nombre << " ===" << std::endl;
    
    if (candidatos.empty()) {
        std::cout << "No hay candidatos de este partido en esta región." << std::endl;
    } else {
        for (auto c : candidatos) {
            int edad = calcularEdad(c->fechaNacimiento);
            std::string sexo = (c->sexo == Sexo::Masculino) ? "M" : "F";
            std::cout << "- " << c->nombre << " " << c->apellido 
                      << " (Edad: " << edad << ", Sexo: " << sexo 
                      << ", Ciudad: " << c->ciudadAspirante->nombre << ")" << std::endl;
        }
    }
}

void UtilidadesAnalisis::imprimirCandidatosPartidoTodasCiudades(
    const std::vector<std::tuple<Ciudad*, std::string, Sexo, int>>& candidatos, Partido* partido) {
    
    std::cout << "\n=== CANDIDATOS A ALCALDÍA - " << partido->nombre 
              << " EN TODAS LAS CIUDADES ===" << std::endl;
    
	if (candidatos.empty()) {
    	std::cout << "No hay candidatos de este partido." << std::endl;
	} else {
    	for (const auto& item : candidatos) {
        	auto ciudad = std::get<0>(item);
        	auto nombre = std::get<1>(item);
        	auto sexo = std::get<2>(item);
        	auto edad = std::get<3>(item);
        
        	std::string sexoStr = (sexo == Sexo::Masculino) ? "M" : "F";
        	std::cout << "- " << ciudad->nombre << ": " << nombre 
            	      << " (Edad: " << edad << ", Sexo: " << sexoStr << ")" << std::endl;
    	}
	}
}
void UtilidadesAnalisis::imprimirCandidatosPorCiudadPorPartido(
    const std::vector<std::pair<Partido*, Candidato*>>& candidatos, Ciudad* ciudad) {
    
    std::cout << "\n=== CANDIDATOS A ALCALDÍA POR PARTIDO EN " << ciudad->nombre << " ===" << std::endl;
    
    if (candidatos.empty()) {
        std::cout << "No hay candidatos registrados en esta ciudad." << std::endl;
	} else {
    	for (const auto& item : candidatos) {
        	auto partido = item.first;
        	auto candidato = item.second;
        
        	int edad = calcularEdad(candidato->fechaNacimiento);
        	std::string sexo = (candidato->sexo == Sexo::Masculino) ? "M" : "F";
        	std::cout << "- " << partido->nombre << ": " << candidato->nombre << " " 
            	      << candidato->apellido << " (Edad: " << edad << ", Sexo: " << sexo << ")" << std::endl;
    	}
	}
}


