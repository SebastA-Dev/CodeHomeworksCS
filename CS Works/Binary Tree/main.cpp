#include <iostream>
#include <string>
#include <limits>

#include "Lista.h"
#include "Cola.h"
#include "Pila.h"
#include "NodoArbol.h"
#include "Arbol.h"
#include "Persistencia.h"

using namespace std;

// Limpiar entrada

void limpiarBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Menú Principal

void mostrarMenuPrincipal() {
    cout << "\n======= MENU PRINCIPAL =======" << endl;
    cout << "1. Insertar nuevo elemento" << endl;
    cout << "2. Ver recorridos del árbol" << endl;
    cout << "3. Buscar elemento" << endl;
    cout << "4. Modificar elemento" << endl;
    cout << "5. Eliminar elemento" << endl;
    cout << "6. Guardar y salir" << endl;
    cout << "==============================" << endl;
    cout << "Seleccione opcion: ";
}

// Menpu recorridos

void mostrarMenuRecorridos() {
    cout << "\n======= RECORRIDOS =======" << endl;
    cout << "1. Recorrido Inorden" << endl;
    cout << "2. Recorrido Preorden" << endl;
    cout << "3. Recorrido Postorden" << endl;
    cout << "4. Recorrido por Niveles" << endl;
    cout << "5. Ver todos los recorridos" << endl;
    cout << "6. Volver al menú principal" << endl;
    cout << "==========================" << endl;
    cout << "Seleccione opcion: ";
}

// Imprimir cola

void imprimirCola(const Cola<pair<int, int>>& cola, const GestorInfo& gestor) {
    if (cola.estaVacia()) {
        cout << "Cola vacía" << endl;
        return;
    }

    Cola<pair<int, int>> colaCopia(cola);
    int pos = 0;

    while (!colaCopia.estaVacia()) {
        pair<int, int> elemento = colaCopia.desencolar();
        int clave = elemento.first;
        int idInfo = elemento.second;

        cout << "[" << pos++ << "] Clave: " << clave;

        Persistencia registro;
        
        if (gestor.cargarRegistro(idInfo, registro)) {
            cout << " | ";
            registro.imprimir();
        } else {
            cout << " | Informacion no disponible" << endl;
        }
    }
}

// Insertar Elementos

void insertarElementos(Arbol<int>& arbol, GestorInfo& gestor) {
    cout << "\n--- INSERCION DE ELEMENTOS ---" << endl;
    cout << "Ingrese claves. Escriba -999 para finalizar." << endl;

    int clave;
    while (true) {
        cout << "\nIngrese clave (int): ";
        cin >> clave;

        if (clave == -999) {
            cout << "Finalizando inserción..." << endl;
            break;
        }

        if (arbol.estaLleno()) {
            cout << "ERROR: Árbol lleno" << endl;
            break;
        }

        cout << "Ingrese nombre: ";
        limpiarBuffer();
        string nombre;
        getline(cin, nombre);

        cout << "Ingrese descripcion: ";
        string descripcion;
        getline(cin, descripcion);

        // Crear registro
        int nuevoId = gestor.obtenerNuevoId();
        Persistencia registro(nuevoId, nombre, descripcion);
        gestor.guardarRegistro(registro);

        // Insertar en el arbol
        if (arbol.insertar(clave, nuevoId)) {
            cout << "Elemento insertado exitosamente" << endl;
        } else {
            cout << "ERROR: No se pudo insertar (clave duplicada o árbol lleno)" << endl;
        }
    }

    // Mostrar recorridos después de insertar
    
    cout << "\n--- MOSTRANDO RECORRIDOS POST-INSERCION ---" << endl;

    cout << "\nINORDEN (ordenado):" << endl;
    Cola<pair<int, int>> inorden = arbol.recorridoInorden();
    imprimirCola(inorden, gestor);

    cout << "\nPREORDEN:" << endl;
    Cola<pair<int, int>> preorden = arbol.recorridoPreorden();
    imprimirCola(preorden, gestor);

    cout << "\nPOSTORDEN:" << endl;
    Cola<pair<int, int>> postorden = arbol.recorridoPostorden();
    imprimirCola(postorden, gestor);

    cout << "\nPOR NIVELES:" << endl;
    Cola<pair<int, int>> porNiveles = arbol.recorridoPorNiveles();
    imprimirCola(porNiveles, gestor);
}

// ver los recorridos

void verRecorridos(const Arbol<int>& arbol, const GestorInfo& gestor) {
    int opcion;

    while (true) {
        mostrarMenuRecorridos();
        cin >> opcion;

        if (opcion == 6) {
            cout << "Volviendo al menú principal..." << endl;
            break;
        }

        switch (opcion) {
            case 1: {
                cout << "\n--- RECORRIDO INORDEN ---" << endl;
                Cola<pair<int, int>> recorrido = arbol.recorridoInorden();
                imprimirCola(recorrido, gestor);
                break;
            }
            case 2: {
                cout << "\n--- RECORRIDO PREORDEN ---" << endl;
                Cola<pair<int, int>> recorrido = arbol.recorridoPreorden();
                imprimirCola(recorrido, gestor);
                break;
            }
            case 3: {
                cout << "\n--- RECORRIDO POSTORDEN ---" << endl;
                Cola<pair<int, int>> recorrido = arbol.recorridoPostorden();
                imprimirCola(recorrido, gestor);
                break;
            }
            case 4: {
                cout << "\n--- RECORRIDO POR NIVELES ---" << endl;
                Cola<pair<int, int>> recorrido = arbol.recorridoPorNiveles();
                imprimirCola(recorrido, gestor);
                break;
            }
            case 5: {
                cout << "\n--- TODOS LOS RECORRIDOS ---" << endl;

                cout << "\nINORDEN:" << endl;
                Cola<pair<int, int>> inorden = arbol.recorridoInorden();
                imprimirCola(inorden, gestor);

                cout << "\nPREORDEN:" << endl;
                Cola<pair<int, int>> preorden = arbol.recorridoPreorden();
                imprimirCola(preorden, gestor);

                cout << "\nPOSTORDEN:" << endl;
                Cola<pair<int, int>> postorden = arbol.recorridoPostorden();
                imprimirCola(postorden, gestor);

                cout << "\nPOR NIVELES:" << endl;
                Cola<pair<int, int>> porNiveles = arbol.recorridoPorNiveles();
                imprimirCola(porNiveles, gestor);
                break;
            }
            default:
                cout << "Opción inválida" << endl;
        }
    }
}

// buscar

void buscarElemento(const Arbol<int>& arbol, const GestorInfo& gestor) {
    cout << "\n--- BUSQUEDA DE DATO ---" << endl;
    int clave;
    cout << "Ingrese clave a buscar: ";
    cin >> clave;

    int idInfo;
    if (arbol.buscar(clave, idInfo)) {
        cout << "Dato encontrado!" << endl;
        cout << "Clave: " << clave << endl;

        Persistencia registro;
        if (gestor.cargarRegistro(idInfo, registro)) {
            registro.imprimir();
        }
    } else {
        cout << "Elemento no encontrado" << endl;
    }
}

// modificar

void modificarElemento(Arbol<int>& arbol, GestorInfo& gestor) {
    cout << "\n--- MODIFICACION DE DATOS ---" << endl;
    int clave;
    cout << "Ingrese clave a modificar: ";
    cin >> clave;

    int idInfo;
    if (arbol.buscar(clave, idInfo)) {
        cout << "Dato encontrado!" << endl;

        Persistencia registro;
        if (gestor.cargarRegistro(idInfo, registro)) {
            cout << "Información actual:" << endl;
            registro.imprimir();

            limpiarBuffer();
            cout << "\nIngrese nuevo nombre: ";
            string nuevoNombre;
            getline(cin, nuevoNombre);

            cout << "Ingrese nueva descripcion: ";
            string nuevaDesc;
            getline(cin, nuevaDesc);

            registro.nombre = nuevoNombre;
            registro.descripcion = nuevaDesc;

            if (gestor.guardarRegistro(registro)) {
                cout << "El dato ha sido modificado" << endl;
            } else {
                cout << "ERROR al guardar cambios" << endl;
            }
        }
    } else {
        cout << "Elemento no encontrado" << endl;
    }
}

// Eliminar 

void eliminarElemento(Arbol<int>& arbol, GestorInfo& gestor) {
    cout << "\n--- ELIMINACION DE ELEMENTO ---" << endl;
    int clave;
    cout << "Ingrese clave a eliminar: ";
    cin >> clave;

    int idInfo;
    if (arbol.buscar(clave, idInfo)) {
        Persistencia registro;
        if (gestor.cargarRegistro(idInfo, registro)) {
            cout << "Información a eliminar:" << endl;
            registro.imprimir();

            if (arbol.eliminar(clave)) {
               
                registro.marcadoComoBorrado = true;
                gestor.guardarRegistro(registro);

                cout << "Elemento eliminado" << endl;

                // Mostrar recorridos luego de que se elimino
                
                cout << "\n--- MOSTRANDO RECORRIDOS LUEGO DE LA ELIMINACIÓN ---" << endl;

                cout << "\nINORDEN (ordenado):" << endl;
                Cola<pair<int, int>> inorden = arbol.recorridoInorden();
                imprimirCola(inorden, gestor);

                cout << "\nPREORDEN:" << endl;
                Cola<pair<int, int>> preorden = arbol.recorridoPreorden();
                imprimirCola(preorden, gestor);

                cout << "\nPOSTORDEN:" << endl;
                Cola<pair<int, int>> postorden = arbol.recorridoPostorden();
                imprimirCola(postorden, gestor);

                cout << "\nPOR NIVELES:" << endl;
                Cola<pair<int, int>> porNiveles = arbol.recorridoPorNiveles();
                imprimirCola(porNiveles, gestor);
            }
        }
    } else {
        cout << "Elemento no encontrado" << endl;
    }
}

// Función main
int main() {
    cout << "====== ARBOL BINARIO DE BUSQUEDA ======" << endl;
    cout << "Sistema de gestion de informacion" << endl;
    cout << "=======================================" << endl;

    // Crear árbol con capacidad de 100 elementos
    Arbol<int> arbol(100, "arbol_datos");
    GestorInfo gestor("registros");

    // Intentar cargar datos previos
    if (arbol.cargarDesdeArchivo()) {
        cout << "\nDatos cargados desde el archivo" << endl;
        cout << "Cantidad de datos: " << arbol.obtenerCantidad() << endl;
    } else {
        cout << "\nEl nuevo árbol ha sido creado" << endl;
        insertarElementos(arbol, gestor);
    }

    int opcion;
    while (true) {
        mostrarMenuPrincipal();
        cin >> opcion;

        switch (opcion) {
            case 1:
                insertarElementos(arbol, gestor);
                break;

            case 2:
                verRecorridos(arbol, gestor);
                break;

            case 3:
                buscarElemento(arbol, gestor);
                break;

            case 4:
                modificarElemento(arbol, gestor);
                break;

            case 5:
                eliminarElemento(arbol, gestor);
                break;

            case 6: {
                cout << "\nGuardando datos..." << endl;
                arbol.guardarEnArchivo();
                gestor.guardarProximoId();
                cout << "Los datos hasn sido guardados correctamente" << endl;
;
                return 0;
            }

            default:
                cout << "Opción inválida. Escoja una opción." << endl;
        }
    }

    return 0;
}
