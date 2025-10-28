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
    string telefono;

    Persona() : codigo(0), nombre(""), apellido("") ,telefono("") {}
    Persona(int cod, string nom, string ape,string tel)
        : codigo(cod), nombre(nom), apellido(ape),telefono(tel) {}

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
             << " - Telefono: " << lista[i].telefono << endl;
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
             << " - Telefono: " << lista[i].telefono << endl;
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
                string apellido = "";
                string nombre = "";
                if (coma != string::npos) {
                    apellido = linea.substr(0, coma);
                    nombre = linea.substr(coma + 1, posPlus - coma - 1);
                } else {
                    nombre = linea.substr(0, posPlus);
                }

                size_t ultimoEspacio = linea.rfind(' ');
                string telefono = linea.substr(posPlus, ultimoEspacio - posPlus);
                string codigoStr = linea.substr(ultimoEspacio + 1);
                int codigo = stoi(codigoStr);
                Persona p(codigo, nombre, apellido, telefono);
            
                if(!ord){
                    listaOriginal.insertarOrdenStruct<int>(p, [](const Persona& per) { return per.codigo; },true);  
                }else{
                    listaOriginal.insertarOrdenStruct<string>(p, [](const Persona& per) { return per.apellido; },true);
                }                
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

    listaDoble[0].eliminarElemento<string>("Gómez", [](const Persona& per) { return per.apellido; });

    // DESCENDENTE
    cout << "\n\nIMPRESION EN DESCENTENDE (APELLIDOS | CODIGO)\n";
    for(int i=0; i<listaDoble.obtenerTam(); i++){
        Lista<Persona> subLista = listaDoble[i];
        cout << subLista.obtenerTam()<<"\n\n";
    };

    return 0;
}
