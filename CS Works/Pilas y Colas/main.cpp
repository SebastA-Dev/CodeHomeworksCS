#include <iostream>
#include <string>
#include <regex>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <limits>
#include "Cola1.h" 
#include "pila1.h"

using namespace std;

struct PersonaBanco {
    string nombre;
    int indiceTransaccion;

    PersonaBanco(const string& n, int idx)
        : nombre(n), indiceTransaccion(idx) {}

    std::string to_string() const {
        return "{" + nombre + ", idx=" + std::to_string(indiceTransaccion) + "}";
    }
};

inline std::ostream& operator<<(std::ostream& os, const PersonaBanco& p) {
    return os << p.to_string();
}

// Limpia el string para prueba de palíndromo
string limpiar(string texto) {
    transform(texto.begin(), texto.end(), texto.begin(), ::tolower);
    texto = regex_replace(texto, std::regex("á"), "a");
    texto = regex_replace(texto, std::regex("é"), "e");
    texto = regex_replace(texto, std::regex("í"), "i");
    texto = regex_replace(texto, std::regex("ó"), "o");
    texto = regex_replace(texto, std::regex("ú"), "u");
    texto = regex_replace(
        texto,
        regex(R"([!¡?¿\s\.,;:'"()\[\]{}…/\\_@#$%&=+\*\^<>|~\-])"),
        ""
    );
    texto = regex_replace(texto, std::regex(R"(\s{2,})"), " ");
    texto = regex_replace(texto, std::regex(R"(^\s+|\s+$)"), "");
    return texto;
}

bool isPalindrome(string s){
    pila pl;
    pl.CargarDesdeString(s);
    string s_final = pl.ToString();
    bool result = (s_final == s);
    return result;
}

// Recorre una copia, y luego obtiene el tiempo para devolver
long long TiempoEsperaHastaPos(const cola<PersonaBanco*>& q, int pos1, const vector<double>& tiempos) {
    if (pos1 <= 0) {
        throw runtime_error("Posicion invalida");
    }
    cola<PersonaBanco*> copia = q.Clone();

    long long total = 0;
    int idxPos = pos1 - 1; 
    for (int i = 0; i < idxPos; ++i) {
        if (copia.ColaVacia()) {
            throw runtime_error("Posicion fuera del tamano de la cola");
        }
        PersonaBanco* frente = copia.AtenderCola(); 
        int idx = frente->indiceTransaccion;
        if (idx < 0 || idx >= static_cast<int>(tiempos.size())) {
            throw runtime_error("Indice de transaccion fuera de rango");
        }
        total += static_cast<long long>(tiempos[idx]);
    }
    // Validación adicional: debe existir la persona en pos1
    if (copia.ColaVacia()) {
        throw runtime_error("Posicion fuera del tamano de la cola");
    }
    return total;
}

int main(int argc, char** argv) {

    /*
    PALINDROMO
    */

    while (true) {
        cout << "Menu\n";
        cout << "1. Ingresar frase\n";
        cout << "2. Salir\n";
        cout << "Opcion: ";

        int opcion;
        if (!(cin >> opcion)) {                      
            cin.clear();                             
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            cout << "Ingresa un valor correcto\n";
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (opcion == 1) {
            string s;
            cout << "Ingrese la frase: ";
            getline(cin, s);

            string limpio = limpiar(s);
            if (isPalindrome(limpio))
                cout << "La frase ingresada es un Palindromo\n";
            else
                cout << "no es palindromo\n";
        } else if (opcion == 2) {
            break;
        } else {
            cout << "Ingresa un valor correcto\n";
        }
    }


    /*
    COLA BANCO
    */

    vector<string> operaciones = {"Retiro", "Deposito", "Consulta", "Actualizacion", "Pagos"};
    vector<double> tiempos = {4.0, 2.0, 3.5, 5.0, 2.0};

    cola<PersonaBanco*> q;

    // Semilla de aleatorio
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    while (true) {
        cout << "\nMenu:\n";
        cout << "1. Ingresar persona\n";
        cout << "2. Imprimir cola\n";
        cout << "3. Identificar persona (tiempo restante)\n";
        cout << "4. Salir\n";
        cout << "Opcion: ";

        int op;
        if (!(cin >> op)) {
            cout << "Entrada invalida.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (op) {
            case 1: {
                cout << "Ingresa el nombre de la persona:\n\t";
                string nombre;

                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getline(cin, nombre);
                
                int indice = std::rand() % operaciones.size();
                PersonaBanco* p = new PersonaBanco(nombre, indice);
                q.InsCola(p);
                cout << "Persona agregada a la cola.\n";
            }
            case 2: {
                q.ImprimirCola();
                break;
            }
            case 3: {                
                q.ImprimirCola();

                cout << "Ingresa la posicion (1 es el primero en la cola): ";
                int pos1;
                if (!(cin >> pos1)) {
                    cout << "Posicion invalida.\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }
                try {
                    long long minutos = TiempoEsperaHastaPos(q, pos1, tiempos);
                    cout << "Tiempo restante de espera para la persona en posicion "
                         << pos1 << ": " << minutos << " minutos.\n";
                } catch (const exception& e) {
                    cout << "Error: " << e.what() << "\n";
                }
                break;
            }
            case 4: {
                cout << "Saliendo...\n";                
                try {
                    while (!q.ColaVacia()) {
                        PersonaBanco* p = q.AtenderCola();
                        delete p;
                    }
                } catch (...) {
                }
                return 0;
            }
            default:
                cout << "Opcion no valida.\n";
        }
    }

    return 0;
}
