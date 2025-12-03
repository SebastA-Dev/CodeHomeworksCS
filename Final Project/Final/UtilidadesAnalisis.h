#ifndef UTILIDADES_ANALISIS_H
#define UTILIDADES_ANALISIS_H

#include <string>
#include <ctime>
#include <vector>
#include <type_traits>

class UtilidadesAnalisis {
public:
    static std::string stringToHex(const std::string &input);
    static int calcularEdad(const std::tm& fechaNacimiento);
    static unsigned long long hashToULL(const std::string &input);
    static std::string hashToHex(const std::string &input);

    template<typename T>
    static int busquedaBinariaCodigo(const std::vector<T>& elementos, const std::string& buscado) {
        auto obtenerCodigo = [&](const T& item) -> std::string {
            return obtenerCodigoImpl(item, std::is_pointer<T>());
        };
        // Compare numeric values of hex codes to ensure ascending numeric order
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
    // helpers for pointer/non-pointer extraction
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

#endif // UTILIDADES_ANALISIS_H

