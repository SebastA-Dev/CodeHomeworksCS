#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "Listas.h"

using namespace std;

// ===================== Persona ====================================

struct Persona {
    int codigo;
    string nombre;
    string apellido;
    string direccion;
    string telefono;

    Persona() : codigo(0), nombre(""), apellido(""), direccion("") ,telefono("") {}
    Persona(int cod, string nom, string ape,string tel, string dir)
        : codigo(cod), nombre(nom), apellido(ape), direccion(dir),telefono(tel) {}

    // para comparación
    bool operator<(const Persona& otra) const {
        return codigo < otra.codigo;
    }

    bool operator>(const Persona& otra) const {
        return codigo > otra.codigo;
    }
};

//=============================== Funciones =============================

// Imprimir lista ascendente
template<class T>
void imprimirListaAscendente(Lista<T>& lista) {    
    cout << string(50, '-') << endl;
    for (int i = 0; i < lista.obtenerTam(); i++) {
        cout << "Codigo: " << lista[i].codigo
             << " - Apellido: " << lista[i].apellido
             << " - Nombre: " << lista[i].nombre
             << " - Telefono: " << lista[i].telefono 
             << " - Direccion: " << lista[i].direccion << endl;
    }
    cout << string(50, '-') << endl;
}

// Imprimir lista descendente
template<class T>
void imprimirListaDescendente(Lista<T>& lista) {    
    cout << string(50, '-') << endl;
    for (int i = lista.obtenerTam() - 1; i >= 0; i--) {
        cout << "Codigo: " << lista[i].codigo
             << " - Apellido: " << lista[i].apellido
             << " - Nombre: " << lista[i].nombre
             << " - Telefono: " << lista[i].telefono 
             << " - Direccion: " << lista[i].direccion << endl;
    }
    cout << string(50, '-') << endl;
}

Lista<Persona> leerArchivoPersonas(const string& direccion, bool ord = false) {
    ifstream archivo(direccion);
    string linea;
    Lista<Persona> listaOriginal;

    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo." << endl;
        return listaOriginal;
    }

    while (getline(archivo, linea)) {        
        if (!linea.empty()) {           
            size_t posPlus = linea.find("+57");
            if (posPlus != string::npos) {
                size_t coma = linea.find(',');
                string apellido = coma != string::npos ? linea.substr(0, coma) : "";
                string nombre = coma != string::npos ? linea.substr(coma + 1, posPlus - coma - 1) : linea.substr(0, posPlus);

                size_t posTelEnd = linea.find(' ', posPlus);
                string telefono = linea.substr(posPlus, posTelEnd - posPlus);

                // Buscar siguiente número (código)
                size_t posCodigoInicio = linea.find_first_not_of(' ', posTelEnd);
                size_t posCodigoFin = linea.find(' ', posCodigoInicio);
                string codigoStr = linea.substr(posCodigoInicio, posCodigoFin - posCodigoInicio);

                // Validar si realmente es número antes de stoi
                int codigo = 0;
                try { codigo = stoi(codigoStr); } 
                catch (...) { continue; } // si no es número, salta la línea

                string direccion = linea.substr(posCodigoFin + 1); // resto de la línea
                Persona p(codigo, nombre, apellido, telefono, direccion);
            
                if(!ord)
                    listaOriginal.insertarOrdenStruct<int>(p, [](const Persona& per){ return per.codigo; }, true);
                else
                    listaOriginal.insertarOrdenStruct<string>(p, [](const Persona& per){ return per.apellido; }, true);
            }
        }
    }


    archivo.close();
    return listaOriginal;
}


// ============= Main =========================

int main() {

    
    Lista<Lista<Persona>> listaDoble;

    //Appellido
    listaDoble.insertarFinal(leerArchivoPersonas("datos_dummy_Lista1.txt", true));

    //Codigo
    listaDoble.insertarFinal(leerArchivoPersonas("datos_dummy_Lista2.txt"));
    
    // ASCENDENTE
    cout << "IMPRESION EN ASCENDENTE (APELLIDOS | CODIGO)\n";
    for(int i=0; i<listaDoble.obtenerTam(); i++){
        Lista<Persona> subLista = listaDoble[i];
        imprimirListaAscendente(subLista);
    };

    // DESCENDENTE
    cout << "\n\nIMPRESION EN DESCENTENDE (APELLIDOS | CODIGO)\n";
    for(int i=0; i<listaDoble.obtenerTam(); i++){
        Lista<Persona> subLista = listaDoble[i];
        imprimirListaDescendente(subLista);
    };

    // ESTO DE ACA ELIMINA, ENTONCES, PIDE EL PARAMETRO, EL VALOR, LUEGO EL DATO ABSTRACTO Y LA COLUMNA DE LA CUAL TOMARA LA ELIMINACION
    // INSERCION VA IGUAL
    listaDoble[0].eliminarElemento<string>("Gómez", [](const Persona& per) { return per.apellido; });
    listaDoble[0].eliminarElemento<string>("ASFASFDAS", [](const Persona& per) { return per.apellido; });

    for(int i=0; i<listaDoble.obtenerTam(); i++){
        Lista<Persona> subLista = listaDoble[i];
        cout << subLista.obtenerTam()<<"\n\n";
    };

    return 0;
}
