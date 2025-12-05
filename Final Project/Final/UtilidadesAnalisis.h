#ifndef UTILIDADES_ANALISIS_H
#define UTILIDADES_ANALISIS_H

#include <string>
#include <ctime>
#include <vector>
#include <type_traits>
#include "EstructuraElectoral.h"

// Forward declarations
struct Ciudad;
struct Partido;
struct Pais;
struct Candidato;
struct Region;

class UtilidadesAnalisis {
public:
    // Funciones existentes
    static std::string stringToHex(const std::string &input);
    static EstadoCivil parsearEstadoCivil(int opcion);
    static EstadoCivil parsearEstadoCivil(std::string opcion);
    static std::tm parsearFecha(const std::string& fechaStr);
    static Sexo parsearSexo(char opcion);
    static TipoCandidato parsearTipoCandidato(int opc);
    static TipoCandidato parsearTipoCandidato(std::string opc);

    static int calcularEdad(const std::tm& fechaNacimiento);
    static unsigned long long hashToULL(const std::string &input);
    static std::string hashToHex(const std::string &input);

    //Impresion
    static void imprimirTarjetonAlcaldia(Ciudad* ciudad);
    static void imprimirTarjetonPresidencia(Pais* pais);
    static void imprimirCensoElectoral(const std::vector<Ciudad*>& ciudades);
    static void imprimirCandidatosPartidoRegion(const std::vector<Candidato*>& candidatos, Partido* partido, Region* region);
    static void imprimirCandidatosPartidoTodasCiudades(const std::vector<std::tuple<Ciudad*, std::string, Sexo, int>>& candidatos, Partido* partido);
    static void imprimirCandidatosPorCiudadPorPartido(const std::vector<std::pair<Partido*, Candidato*>>& candidatos, Ciudad* ciudad);

    //Validaciones
    static char leerSexo();
    static int leerEstadoCivil();
    static std::tm leerFechaNacimiento();
    static int leerTipoCandidato();

    //Busqueda binaria
    template<typename T>
    static int busquedaBinariaCodigo(const std::vector<T>& elementos, const std::string& buscado) {
        auto obtenerCodigo = [&](const T& item) -> std::string {
            return obtenerCodigoImpl(item, std::is_pointer<T>());
        };
        auto hexToULL = [](const std::string &s) -> unsigned long long {
            if (s.empty()) return 0ULL;
            try {
                return std::stoull(s, nullptr, 16);
            } catch (...) {
                return 0ULL;
            }
        };

        unsigned long long target = hexToULL(buscado);

        int izquierda = 0;
        int derecha = static_cast<int>(elementos.size()) - 1;

        while (izquierda <= derecha) {
            int medio = izquierda + (derecha - izquierda) / 2;
            const T& elem = elementos[medio];
            std::string codigoMedioStr = obtenerCodigo(elem);
            unsigned long long codigoMedio = hexToULL(codigoMedioStr);

            if (codigoMedio == target) return medio;
            if (codigoMedio < target) izquierda = medio + 1;
            else derecha = medio - 1;
        }
        return -1;
    }

private:
    template<typename U>
    static std::string obtenerCodigoImpl(const U& v, std::false_type) {
        return v.codigo;
    }

    template<typename U>
    static std::string obtenerCodigoImpl(const U& v, std::true_type) {
        if (!v) return std::string();
        return v->codigo;
    }
};

#endif
