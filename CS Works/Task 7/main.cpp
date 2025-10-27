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
    string telefono;

    Persona() : codigo(0), nombre(""), telefono("") {}
    Persona(int cod, string nom, string tel)
        : codigo(cod), nombre(nom), telefono(tel) {}

    // psra comparación
    
    bool operator<(const Persona& otra) const {
        return codigo < otra.codigo;
    }

    bool operator>(const Persona& otra) const {
        return codigo > otra.codigo;
    }
};

//=============================== Funciones =============================

// imprimir la lista de personas

template<class T>
void imprimirLista(Lista<T>& lista, string titulo) {
    cout << "\n" << titulo << "\n";
    cout << string(50, '-') << endl;
    for (int i = 0; i < lista.obtenerTam(); i++) {
        cout << "Codigo: " << lista[i].codigo 
             << " - Nombre: " << lista[i].nombre
             << " - Telefono: " << lista[i].telefono << endl;
    }
    cout << string(50, '-') << endl;
}

// imprimir en orden descendente

template<class T>
void imprimirListaDescendente(Lista<T>& lista, string titulo) {
    cout << "\n" << titulo << "\n";
    cout << string(50, '-') << endl;
    for (int i = lista.obtenerTam() - 1; i >= 0; i--) {
        cout << "Codigo: " << lista[i].codigo 
             << " - Nombre: " << lista[i].nombre
             << " - Telefono: " << lista[i].telefono << endl;
    }
    cout << string(50, '-') << endl;
}

// inserción

template<class T>
void ordenarPorCodigo(Lista<T>& origen, Lista<T>& destino) {
    for (int i = 0; i < origen.obtenerTam(); i++) {
        T persona = origen[i];
        
        if (destino.estaVacia()) {
            destino.insertarInicio(persona);
        } else {
            bool insertado = false;
            
            for (int j = 0; j < destino.obtenerTam(); j++) {
                if (persona.codigo < destino[j].codigo) {
                    destino.insertarPosicion(persona, j);
                    insertado = true;
                    break;
                }
            }
            
            if (!insertado) {
                destino.insertarFinal(persona);
            }
        }
    }
}

//  inserción por nombre

template<class T>
void ordenarPorNombre(Lista<T>& origen, Lista<T>& destino) {
    for (int i = 0; i < origen.obtenerTam(); i++) {
        T persona = origen[i];
        
        if (destino.estaVacia()) {
            destino.insertarInicio(persona);
        } else {
            bool insertado = false;
            
            for (int j = 0; j < destino.obtenerTam(); j++) {
                if (persona.nombre < destino[j].nombre) {
                    destino.insertarPosicion(persona, j);
                    insertado = true;
                    break;
                }
            }
            
            if (!insertado) {
                destino.insertarFinal(persona);
            }
        }
    }
}

// Buscar persona por código
template<class T>
int buscarPorCodigo(Lista<T>& lista, int codigo) {
    for (int i = 0; i < lista.obtenerTam(); i++) {
        if (lista[i].codigo == codigo) {
            return i;
        }
    }
    return -1;
}

// Buscar persona por nombre

template<class T>
int buscarPorNombre(Lista<T>& lista, string nombre) {
    for (int i = 0; i < lista.obtenerTam(); i++) {
        if (lista[i].nombre == nombre) {
            return i;
        }
    }
    return -1;
}

// ============= Main =========================

int main() {
    
    
    cout << "\n========== LEYENDO ARCHIVO datos_dummy.txt ==========\n";
    
    ifstream archivo("./datos_dummy.txt");
    string linea;

    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo." << endl;
        return 1;
    }

    // Leer archivo 
    
    cout << "\nContenido del archivo:\n";
    cout << string(50, '-') << endl;
    
    Lista<Lista<Persona>> listaDeListas;
    Lista<Persona> listaInterna;
    int contadorPersonas = 0;

    // Leer línea por línea
    while (getline(archivo, linea)) {
        cout << linea << endl;
        
        if (!linea.empty()) {
            
            
            // 1. Encontrar posición del teléfono (+57)
            size_t posPlus = linea.find("+57");
            if (posPlus != string::npos) {
                
                // 2. Extraer nombre (desde inicio hasta +57)
                string nombre = linea.substr(0, posPlus);
                while (!nombre.empty() && nombre.back() == ' ') {
                    nombre.pop_back();
                }
                
                // 3. Encontrar último espacio para obtener código
                size_t ultimoEspacio = linea.rfind(' ');
                
                // 4. Extraer teléfono (desde +57 hasta último espacio)
                string telefono = linea.substr(posPlus, ultimoEspacio - posPlus);
                while (!telefono.empty() && telefono.back() == ' ') {
                    telefono.pop_back();
                }
                
                // 5. Extraer código (último campo)
                string codigoStr = linea.substr(ultimoEspacio + 1);
                int codigo = stoi(codigoStr);
                
                // 6. Crear persona y agregar a lista
                listaInterna.insertarFinal(Persona(codigo, nombre, telefono));
                contadorPersonas++;
                
                // Crear grupos de 5 personas
                if (contadorPersonas % 5 == 0) {
                    if (!listaInterna.estaVacia()) {
                        listaDeListas.insertarFinal(listaInterna);
                        listaInterna = Lista<Persona>();
                    }
                }
            }
        }
    }
    
    cout << string(50, '-') << endl;
    
    
    if (!listaInterna.estaVacia()) {
        listaDeListas.insertarFinal(listaInterna);
    }
    
    archivo.close();
    
    // ============  Crear Lista<Lista<Persona>> con los datos =============
    
    cout << "\n========== CARGANDO DATOS EN LISTA<LISTA<PERSONA>> ==========\n";
    
	cout << "\nDatos cargados en Lista<Lista<Persona>>:" << endl;
	for (int i = 0; i < listaDeListas.obtenerTam(); i++) {
    	cout << "\nGrupo " << (i + 1) << ":" << endl;
    	for (int j = 0; j < listaDeListas[i].obtenerTam(); j++) {
       		Persona p = listaDeListas[i][j];  
        	cout << "  Codigo: " << p.codigo 
            	 << " - Nombre: " << p.nombre
            	 << " - Telefono: " << p.telefono << endl;
    	}
	}

    
    // Crear todas cosas las personas
    
    Lista<Persona> listaOriginal;
    for (int i = 0; i < listaDeListas.obtenerTam(); i++) {
        for (int j = 0; j < listaDeListas[i].obtenerTam(); j++) {
            listaOriginal.insertarFinal(listaDeListas[i][j]);
        }
    }
    
    // ============= PASO 2:  listas ordenadas =============
    
    
    Lista<Persona> listaPorCodigo;
    Lista<Persona> listaPorNombre;
    
    ordenarPorCodigo(listaOriginal, listaPorCodigo);
    ordenarPorNombre(listaOriginal, listaPorNombre);
    
    
    // ============ PASO 3: Imprimir en ascendente y descendente ===============
    
    
    cout << "\n\n========== LISTAS ORDENADAS (ASCENDENTE Y DESCENDENTE) ==========\n";
    
    imprimirLista(listaPorCodigo, "LISTA ORDENADA POR CODIGO (ASCENDENTE)");
    imprimirListaDescendente(listaPorCodigo, "LISTA ORDENADA POR CODIGO (DESCENDENTE)");
    
    imprimirLista(listaPorNombre, "LISTA ORDENADA POR NOMBRE (ASCENDENTE)");
    imprimirListaDescendente(listaPorNombre, "LISTA ORDENADA POR NOMBRE (DESCENDENTE)");
    
    
    // =========== PASO 4: Eliminar un elemento de la lista por código ===============
    
    cout << "\n\n========== ELIMINACION DE ELEMENTO (LISTA POR CODIGO) ==========\n";
    
    int codigoEliminar = 10000015; // Herrera, Mateo
    int posEliminar = buscarPorCodigo(listaPorCodigo, codigoEliminar);
    
    if (posEliminar != -1) {
        cout << "\nEliminando a: " << listaPorCodigo[posEliminar].nombre 
             << " (Codigo: " << listaPorCodigo[posEliminar].codigo << ")" << endl;
        listaPorCodigo.eliminarPosicion(posEliminar);
    }
    
    // ========== PASO 5: Imprimir lista en ascendente después de eliminación ==========
    
    
    imprimirLista(listaPorCodigo, "LISTA POR CODIGO DESPUES DE ELIMINAR (ASCENDENTE)");
    
    
    // ========== PASO 6: Eliminar elemento que NO esté en la otra lista ==========
    
    
    cout << "\n\n========== ELIMINACION DE ELEMENTO QUE NO ESTE EN OTRA LISTA ==========\n";
    
    
    // PENDIENTE
    
    
    // ========== PASO 7: Imprimir en descendente ==========
    
    
    imprimirListaDescendente(listaPorNombre, "LISTA POR NOMBRE FINAL (DESCENDENTE)");
    imprimirListaDescendente(listaPorCodigo, "LISTA POR CODIGO FINAL (DESCENDENTE)");
    
    cout << "\n\n========== PROGRAMA FINALIZADO ==========\n\n";
    
    return 0;
    
}
