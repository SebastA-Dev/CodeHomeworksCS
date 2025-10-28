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

    // para comparaci�n
    bool operator<(const Persona& otra) const {
        return codigo < otra.codigo;
    }

    bool operator>(const Persona& otra) const {
        return codigo > otra.codigo;
    }
};

//=============================== Declaraciones (Prototipos) =============================

template<class T>
void imprimirListaAscendente(Lista<T>& lista);

template<class T>
void imprimirListaDescendente(Lista<T>& lista);

bool buscarPorCodigo(Lista<Persona>& lista, int codigo);

bool buscarPorApellido(Lista<Persona>& lista, const string& apellido);

Lista<Persona> leerArchivoPersonas(const string& direccion, bool ord);

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

// Buscar por codigo
bool buscarPorCodigo(Lista<Persona>& lista, int codigo) {
    for (int i = 0; i < lista.obtenerTam(); i++) {
        if (lista[i].codigo == codigo) {
            cout << "\n=== PERSONA ENCONTRADA ===" << endl;
            cout << "Codigo: " << lista[i].codigo << endl;
            cout << "Apellido: " << lista[i].apellido << endl;
            cout << "Nombre: " << lista[i].nombre << endl;
            cout << "Telefono: " << lista[i].telefono << endl;
            cout << "Direccion: " << lista[i].direccion << endl;
            cout << "========================\n" << endl;
            return true;
        }
    }
    return false;
}

// Buscar por apellido
bool buscarPorApellido(Lista<Persona>& lista, const string& apellido) {
    for (int i = 0; i < lista.obtenerTam(); i++) {
        if (lista[i].apellido == apellido) {
            cout << "\n=== PERSONA ENCONTRADA ===" << endl;
            cout << "Codigo: " << lista[i].codigo << endl;
            cout << "Apellido: " << lista[i].apellido << endl;
            cout << "Nombre: " << lista[i].nombre << endl;
            cout << "Telefono: " << lista[i].telefono << endl;
            cout << "Direccion: " << lista[i].direccion << endl;
            cout << "========================\n" << endl;
            return true;
        }
    }
    return false;
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

                // Buscar siguiente n�mero (c�digo)
                size_t posCodigoInicio = linea.find_first_not_of(' ', posTelEnd);
                size_t posCodigoFin = linea.find(' ', posCodigoInicio);
                string codigoStr = linea.substr(posCodigoInicio, posCodigoFin - posCodigoInicio);

                // Validar si realmente es n�mero antes de stoi
                int codigo = 0;
                try { codigo = stoi(codigoStr); } 
                catch (...) { continue; } // si no es n�mero, salta la l�nea

                string direccion = linea.substr(posCodigoFin + 1); // resto de la l�nea
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

    //Apellido (clave = apellido)
    listaDoble.insertarFinal(leerArchivoPersonas("datos_dummy_Lista1.txt", true));

    //Codigo (clave = codigo)
    listaDoble.insertarFinal(leerArchivoPersonas("datos_dummy_Lista2.txt"));
    
    // ASCENDENTE
    cout << "IMPRESION EN ASCENDENTE (APELLIDOS | CODIGO)\n";
    for(int i=0; i<listaDoble.obtenerTam(); i++){
        Lista<Persona> subLista = listaDoble[i];
        imprimirListaAscendente(subLista);
    };

    // DESCENDENTE
    cout << "\n\nIMPRESION EN DESCENDENTE (APELLIDOS | CODIGO)\n";
    for(int i=0; i<listaDoble.obtenerTam(); i++){
        Lista<Persona> subLista = listaDoble[i];
        imprimirListaDescendente(subLista);
    };

    // ELIMINACION LISTA 1 
    
    cout << "\n\nELIMINACION EN LISTA 1\n";
    cout << "Eliminando 'Gómez' de Lista 1...\n";
    bool eliminado1 = listaDoble[0].eliminarElemento<string>("Gómez", [](const Persona& per) { return per.apellido; });
    
    if (eliminado1) {
        cout << "Elemento eliminado exitosamente!\n";
    } else {
        cout << "AVISO: El elemento no se encuentra en la lista.\n";
    }
    
    cout << "\nLista 1 despues de eliminacion (ASCENDENTE):\n";
    Lista<Persona> subLista1 = listaDoble[0];
    imprimirListaAscendente(subLista1);

    // INTENTO ELIMINACION LISTA 2 
    cout << "\n\nELIMINACION EN LISTA 2 (elemento inexistente)\n";
    cout << "Intentando eliminar elemento con codigo 99999\n";
    
    
    bool eliminado2 = listaDoble[1].eliminarElemento<int>(99999, [](const Persona& per) { return per.codigo; });
    
    if (eliminado2) {
        cout << "Elemento eliminado\n";
    } else {
        cout << "AVISO: El elemento con codigo 99999 no se encuentra en la lista.\n";
    }
    
    cout << "\nLista 2 despues de intento (DESCENDENTE):\n";
    Lista<Persona> subLista2 = listaDoble[1];
    imprimirListaDescendente(subLista2);

    // BUSQUEDA APELLIDO
    
    cout << "\n\nBUSQUEDA EN LISTA 1 (clave = apellido)\n";
    
    cout << "Buscando apellido Lopez\n";
    
    if (!buscarPorApellido(listaDoble[0], "López")) {
    
        cout << "AVISO: No se encontro persona con apellido Lopez.\n";
        
    }

    // BUSQUEDA POR CODIGO
    
    
    cout << "\n\nBUSQUEDA EN LISTA 2 (clave = codigo)\n";
    
    cout << "Buscando codigo 105 \n";
    
    if (!buscarPorCodigo(listaDoble[1], 105)) {    	    	
        cout << "AVISO: No se encontro persona con codigo 105.\n";        
    }

    // BUSQUEDA FALLIDA LISTA 1
    
    
    cout << "\n\nBUSQUEDA FALLIDA EN LISTA 1\n";
    
    cout << "Buscando codigo 999 (que no existe)...\n";
    
    if (!buscarPorCodigo(listaDoble[0], 999)) {
    	
        cout << "AVISO: No se encontro persona con codigo 999.\n";
        
    }

    cout << "\n\nPROGRAMA FINALIZADO\n";
    cout << "Lista 1: " << listaDoble[0].obtenerTam() << " elementos\n";
    cout << "Lista 2: " << listaDoble[1].obtenerTam() << " elementos\n";

    return 0;
}

