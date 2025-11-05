#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include "lista.h"

using namespace std;

// ==================== STRUCT ESTUDIANTE ====================
struct estudiante {
    char nombre[100];
    char carrera[100];
    char actividad[100];
    int edad;
    bool activo;
};

// ==================== STRUCTS DE MULTILISTA - ====================

// Nodo de multilista, 4 apuntadores
struct datos {
    int pos;
    datos *sigNom;      // Siguiente por nombre (A-Z)
    datos *sigCar;      // Siguiente por carrera (orden de llegada)
    datos *sigActiv;    // Siguiente por actividad (al inicio)
    datos *sigEdad;     // Siguiente por edad (descendente)
};

// Cabeceras 

struct cabeza {
    string caracteristica;
    datos *Cab;
};

// ==================== CLASE MULTILISTA ====================

template <class T>
class Multilista {
private:
    cabeza cabeceras[4];        // Array de 4 cabeceras
    string nombreArchivo;
    int numRegistros;
    int tamEstruct;
    
    // Métodos privados
    void insertarPorNombre(datos *nuevo, T est);
    void insertarPorCarrera(datos *nuevo);
    void insertarPorActividad(datos *nuevo);
    void insertarPorEdad(datos *nuevo, T est);
    T leerRegistro(int pos);
    void escribirRegistro(T est, int pos);
    
public:
    // Constructor y Destructor
    Multilista(string archivo, int tamano);
    ~Multilista();
    
    // Inicializar arreglo de cabeceras
    void inicializarCabeceras();
    
    // Cargar datos del archivo
    void cargarDesdeArchivo();
    
    // Funciones principales
    void insertar(T est);
    bool eliminar(string nombre);
    datos* buscar(string nombre);
    bool estaVacia();
    
    // Funciones de obtención de listas por cada atributo
    datos* obtenerCabecera(int indice);
    T obtenerDato(datos *nodo);
};

// ==================== IMPLEMENTACIÓN DE MULTILISTA ====================

template <class T>
Multilista<T>::Multilista(string archivo, int tamano) {
    nombreArchivo = archivo;
    numRegistros = 0;
    tamEstruct = tamano;
    inicializarCabeceras();
    cargarDesdeArchivo();
}

template <class T>
Multilista<T>::~Multilista() {
    datos *actual = cabeceras[0].Cab;
    
    while (actual != nullptr) {
        datos *temp = actual;
        actual = actual->sigNom;
        delete temp;
    }
    
    for (int i = 0; i < 4; i++) {
        cabeceras[i].Cab = nullptr;
    }
}

template <class T>
void Multilista<T>::inicializarCabeceras() {
    cabeceras[0].caracteristica = "Nombre";
    cabeceras[0].Cab = nullptr;
    
    cabeceras[1].caracteristica = "Carrera";
    cabeceras[1].Cab = nullptr;
    
    cabeceras[2].caracteristica = "Actividad";
    cabeceras[2].Cab = nullptr;
    
    cabeceras[3].caracteristica = "Edad";
    cabeceras[3].Cab = nullptr;
}

template <class T>
T Multilista<T>::leerRegistro(int pos) {
    T est;
    ifstream archivo(nombreArchivo.c_str(), ios::binary);
    
    if (archivo) {
        archivo.seekg(pos * tamEstruct, ios::beg);
        archivo.read((char*)&est, tamEstruct);
        archivo.close();
    }
    
    return est;
}

template <class T>
void Multilista<T>::escribirRegistro(T est, int pos) {
    fstream archivo(nombreArchivo.c_str(), ios::in | ios::out | ios::binary);
    
    if (!archivo) {
        archivo.open(nombreArchivo.c_str(), ios::out | ios::binary);
        archivo.close();
        archivo.open(nombreArchivo.c_str(), ios::in | ios::out | ios::binary);
    }
    
    archivo.seekp(pos * tamEstruct, ios::beg);
    archivo.write((char*)&est, tamEstruct);
    archivo.close();
}

template <class T>
void Multilista<T>::cargarDesdeArchivo() {
    ifstream archivo(nombreArchivo.c_str(), ios::binary);
    
    if (!archivo) {
        numRegistros = 0;
        return;
    }
    
    T est;
    int pos = 0;
    
    while (archivo.read((char*)&est, tamEstruct)) {
        char *ptr = (char*)&est;
        bool activo = *(bool*)(ptr + tamEstruct - sizeof(bool));
        
        if (activo) {
            datos *nuevo = new datos;
            nuevo->pos = pos;
            nuevo->sigNom = nullptr;
            nuevo->sigCar = nullptr;
            nuevo->sigActiv = nullptr;
            nuevo->sigEdad = nullptr;
            
            insertarPorNombre(nuevo, est);
            insertarPorCarrera(nuevo);
            insertarPorActividad(nuevo);
            insertarPorEdad(nuevo, est);
        }
        pos++;
    }
    
    numRegistros = pos;
    archivo.close();
}

template <class T>
void Multilista<T>::insertarPorNombre(datos *nuevo, T est) {
    if (cabeceras[0].Cab == nullptr) {
        cabeceras[0].Cab = nuevo;
        return;
    }
    
    T estCabeza = leerRegistro(cabeceras[0].Cab->pos);
    
    if (string(est.nombre) < string(estCabeza.nombre)) {
        nuevo->sigNom = cabeceras[0].Cab;
        cabeceras[0].Cab = nuevo;
        return;
    }
    
    datos *actual = cabeceras[0].Cab;
    
    while (actual->sigNom != nullptr) {
        T estSiguiente = leerRegistro(actual->sigNom->pos);
        
        if (string(est.nombre) < string(estSiguiente.nombre)) {
            break;
        }
        
        actual = actual->sigNom;
    }
    
    nuevo->sigNom = actual->sigNom;
    actual->sigNom = nuevo;
}

template <class T>
void Multilista<T>::insertarPorCarrera(datos *nuevo) {
    if (cabeceras[1].Cab == nullptr) {
        cabeceras[1].Cab = nuevo;
        return;
    }
    
    datos *actual = cabeceras[1].Cab;
    
    while (actual->sigCar != nullptr) {
        actual = actual->sigCar;
    }
    
    actual->sigCar = nuevo;
}

template <class T>
void Multilista<T>::insertarPorActividad(datos *nuevo) {
    nuevo->sigActiv = cabeceras[2].Cab;
    cabeceras[2].Cab = nuevo;
}

template <class T>
void Multilista<T>::insertarPorEdad(datos *nuevo, T est) {
    if (cabeceras[3].Cab == nullptr) {
        cabeceras[3].Cab = nuevo;
        return;
    }
    
    T estCabeza = leerRegistro(cabeceras[3].Cab->pos);
    
    if (est.edad > estCabeza.edad) {
        nuevo->sigEdad = cabeceras[3].Cab;
        cabeceras[3].Cab = nuevo;
        return;
    }
    
    datos *actual = cabeceras[3].Cab;
    
    while (actual->sigEdad != nullptr) {
        T estSiguiente = leerRegistro(actual->sigEdad->pos);
        
        if (est.edad > estSiguiente.edad) {
            break;
        }
        
        actual = actual->sigEdad;
    }
    
    nuevo->sigEdad = actual->sigEdad;
    actual->sigEdad = nuevo;
}

template <class T>
void Multilista<T>::insertar(T est) {
    est.activo = true;
    escribirRegistro(est, numRegistros);
    
    datos *nuevo = new datos;
    nuevo->pos = numRegistros;
    nuevo->sigNom = nullptr;
    nuevo->sigCar = nullptr;
    nuevo->sigActiv = nullptr;
    nuevo->sigEdad = nullptr;
    
    insertarPorNombre(nuevo, est);
    insertarPorCarrera(nuevo);
    insertarPorActividad(nuevo);
    insertarPorEdad(nuevo, est);
    
    numRegistros++;
}

template <class T>
datos* Multilista<T>::buscar(string nombre) {
    datos *actual = cabeceras[0].Cab;
    
    while (actual != nullptr) {
        T est = leerRegistro(actual->pos);
        
        if (string(est.nombre) == nombre) {
            return actual;
        }
        
        actual = actual->sigNom;
    }
    
    return nullptr;
}

template <class T>
bool Multilista<T>::eliminar(string nombre) {
    datos *nodo = buscar(nombre);
    
    if (nodo == nullptr) {
        return false;
    }
    
    T est = leerRegistro(nodo->pos);
    est.activo = false;
    escribirRegistro(est, nodo->pos);
    
    // Eliminar de lista por nombre
    if (cabeceras[0].Cab == nodo) {
        cabeceras[0].Cab = nodo->sigNom;
    } else {
        datos *actual = cabeceras[0].Cab;
        while (actual->sigNom != nodo) {
            actual = actual->sigNom;
        }
        actual->sigNom = nodo->sigNom;
    }
    
    // Eliminar de lista por carrera
    if (cabeceras[1].Cab == nodo) {
        cabeceras[1].Cab = nodo->sigCar;
    } else {
        datos *actual = cabeceras[1].Cab;
        while (actual->sigCar != nodo) {
            actual = actual->sigCar;
        }
        actual->sigCar = nodo->sigCar;
    }
    
    // Eliminar de lista por actividad
    if (cabeceras[2].Cab == nodo) {
        cabeceras[2].Cab = nodo->sigActiv;
    } else {
        datos *actual = cabeceras[2].Cab;
        while (actual->sigActiv != nodo) {
            actual = actual->sigActiv;
        }
        actual->sigActiv = nodo->sigActiv;
    }
    
    // Eliminar de lista por edad
    if (cabeceras[3].Cab == nodo) {
        cabeceras[3].Cab = nodo->sigEdad;
    } else {
        datos *actual = cabeceras[3].Cab;
        while (actual->sigEdad != nodo) {
            actual = actual->sigEdad;
        }
        actual->sigEdad = nodo->sigEdad;
    }
    
    delete nodo;
    return true;
}

template <class T>
bool Multilista<T>::estaVacia() {
    return cabeceras[0].Cab == nullptr;
}

template <class T>
datos* Multilista<T>::obtenerCabecera(int indice) {
    if (indice >= 0 && indice < 4) {
        return cabeceras[indice].Cab;
    }
    return nullptr;
}

template <class T>
T Multilista<T>::obtenerDato(datos *nodo) {
    return leerRegistro(nodo->pos);
}

// ==================== INTERFAZ  ====================

void mostrarMenu() {
	
	cout << "\n BIENVENIDOOOOOOOO!! " << endl;
    cout << "\n========================================" << endl;
    cout << "       LISTA ESTUDIANTES" << endl;
    cout << "========================================" << endl;
    cout << "1. Agregar estudiante" << endl;
    cout << "2. Eliminar estudiante" << endl;
    cout << "3. Imprimir lista por nombre " << endl;
    cout << "4. Imprimir estudiantes por carrera" << endl;
    cout << "5. Imprimir estudiantes por actividad" << endl;
    cout << "6. Imprimir lista por mayor de edad a menor" << endl;
    cout << "7. Salir" << endl;
    cout << "========================================" << endl;
    cout << "Seleccione una opcion: ";
}

void agregarEstudiante(Multilista<estudiante> &ml) {
    estudiante est;
    est.activo = true;
    
    cout << "\n--- AGREGAR NUEVO ESTUDIANTE ---" << endl;
    cout << "Nombre: ";
    cin.ignore();
    cin.getline(est.nombre, 100);
    
    cout << "Carrera: ";
    cin.getline(est.carrera, 100);
    
    cout << "Actividad: ";
    cin.getline(est.actividad, 100);
    
    cout << "Edad: ";
    cin >> est.edad;
    
    ml.insertar(est);
    cout << "\n>>> Estudiante agregado exitosamente!" << endl;
}

void eliminarEstudiante(Multilista<estudiante> &ml) {
    char nombre[100];
    
    cout << "\n--- ELIMINAR ESTUDIANTE ---" << endl;
    cout << "Nombre del estudiante a eliminar: ";
    cin.ignore();
    cin.getline(nombre, 100);
    
    if (ml.eliminar(string(nombre))) {
        cout << "\n>>> Estudiante eliminado exitosamente!" << endl;
    } else {
        cout << "\n>>> Estudiante no encontrado." << endl;
    }
}

void imprimirPorNombre(Multilista<estudiante> &ml) {
    cout << "\n========== LISTA ORDENADA POR NOMBRE  ==========" << endl;
    
    if (ml.estaVacia()) {
        cout << "La multilista esta vacia." << endl;
        return;
    }
    
    datos *actual = ml.obtenerCabecera(0);
    int contador = 1;
    
    while (actual != nullptr) {
        estudiante est = ml.obtenerDato(actual);
        cout << contador << ". " << est.nombre 
             << " | Carrera: " << est.carrera 
             << " | Actividad: " << est.actividad 
             << " | Edad: " << est.edad << endl;
        actual = actual->sigNom;
        contador++;
    }
}

void imprimirPorCarrera(Multilista<estudiante> &ml) {
    char carrera[100];
    
    cout << "\nIngrese el nombre de la carrera: ";
    cin.ignore();
    cin.getline(carrera, 100);
    
    cout << "\n========== ESTUDIANTES DE " << carrera << " ==========" << endl;
    
    if (ml.estaVacia()) {
        cout << "La multilista esta vacia." << endl;
        return;
    }
    
    datos *actual = ml.obtenerCabecera(1);
    int contador = 1;
    bool encontrado = false;
    
    while (actual != nullptr) {
        estudiante est = ml.obtenerDato(actual);
        
        if (strcmp(est.carrera, carrera) == 0) {
            cout << contador << ". " << est.nombre 
                 << " | Actividad: " << est.actividad 
                 << " | Edad: " << est.edad << endl;
            encontrado = true;
            contador++;
        }
        
        actual = actual->sigCar;
    }
    
    if (!encontrado) {
        cout << "No hay estudiantes que crusen esta carrera." << endl;
    }
}

void imprimirPorActividad(Multilista<estudiante> &ml) {
    char actividad[100];
    
    cout << "\nIngrese el nombre de la actividad: ";
    cin.ignore();
    cin.getline(actividad, 100);
    
    cout << "\n========== ESTUDIANTES QUE PRACTICAN " << actividad << " ==========" << endl;
    
    if (ml.estaVacia()) {
        cout << "Esta vacia." << endl;
        return;
    }
    
    datos *actual = ml.obtenerCabecera(2);
    int contador = 1;
    bool encontrado = false;
    
    while (actual != nullptr) {
        estudiante est = ml.obtenerDato(actual);
        
        if (strcmp(est.actividad, actividad) == 0) {
            cout << contador << ". " << est.nombre 
                 << " | Carrera: " << est.carrera 
                 << " | Edad: " << est.edad << endl;
            encontrado = true;
            contador++;
        }
        
        actual = actual->sigActiv;
    }
    
    if (!encontrado) {
        cout << "No hay estudiantes con esta actividad " << endl;
    }
}

void imprimirPorEdad(Multilista<estudiante> &ml) {
    cout << "\n========== LISTA POR EDAD  ==========" << endl;
    
    if (ml.estaVacia()) {
        cout << "La multilista esta vacia." << endl;
        return;
    }
    
    datos *actual = ml.obtenerCabecera(3);
    
    cout << "Nombre" << " | " << "Edad" << endl;
    cout << "------|--------" << endl;
    
    while (actual != nullptr) {
        estudiante est = ml.obtenerDato(actual);
        cout << est.nombre << " | " << est.edad << endl;
        actual = actual->sigEdad;
    }
}

// ==================== MAIN ====================
int main() {
    Multilista<estudiante> ml("estudiantes.dat", sizeof(estudiante));
    
    int opcion;
    
    do {
        mostrarMenu();
        cin >> opcion;
        
        switch(opcion) {
            case 1:
                agregarEstudiante(ml);
                break;
                
            case 2:
                eliminarEstudiante(ml);
                break;
                
            case 3:
                imprimirPorNombre(ml);
                break;
                
            case 4:
                imprimirPorCarrera(ml);
                break;
                
            case 5:
                imprimirPorActividad(ml);
                break;
                
            case 6:
                imprimirPorEdad(ml);
                break;
                
            case 7:
                cout << "\n>>> Hasta luego :)))" << endl;
                break;
                
            default:
                cout << "\n>>> Opcion invalida. Intente de nuevo." << endl;
        }
        
        if (opcion != 7) {
            cout << "\nPresione Enter para continuar...";
            cin.ignore();
            cin.get();
        }
        
    } while (opcion != 7);
    
    return 0;
}
