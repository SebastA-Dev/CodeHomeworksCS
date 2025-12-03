#include "DatosElectoral.h"
#include "Lista.h"

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

    Partido* conservador = sistema.crearPartido("Conservador", persona2, true);
    Partido* liberal = sistema.crearPartido("Liberal", persona1, true);    
    Partido* as = sistema.crearPartido("4", persona3, true);
    Partido* ea = sistema.crearPartido("32", persona4, true);
    Partido* afd = sistema.crearPartido("1", persona5, true);

    // ============================================================================
    // MOSTRANDO PARTIDOS EN LA LISTA PÚBLICA DEL SISTEMA
    // ============================================================================
    std::cout << "\n=== PARTIDOS EN sistema.partidos ===" << std::endl;
    std::cout << "Total de partidos en la lista: " << sistema.partidos.obtenerTam() << std::endl;
    
    for (int i = 0; i < sistema.partidos.obtenerTam(); ++i) {
        Partido* p = sistema.partidos.obtenerInfo(i);
        std::cout << "\nPartido " << (i + 1) << ":" << std::endl;
        std::cout << "  Nombre: " << p->nombre << std::endl;
        std::cout << "  Codigo: " << p->codigo << std::endl;
        std::cout << "  Legal: " << (p->legal ? "Si" : "No") << std::endl;
        if (p->representanteLegal) {
            std::cout << "  Representante Legal: " << p->representanteLegal->nombre 
                      << " " << p->representanteLegal->apellido << std::endl;
        }
    }

    return 0;
}
