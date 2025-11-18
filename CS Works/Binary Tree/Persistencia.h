#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include <string>
#include <iostream>
#include <fstream>

struct Persistencia {
    int id;
    std::string nombre;
    std::string descripcion;
    bool marcadoComoBorrado;

    Persistencia() 
        : id(-1), nombre(""), descripcion(""), marcadoComoBorrado(false) {}

    Persistencia(int i, const std::string& n, const std::string& d)
        : id(i), nombre(n), descripcion(d), marcadoComoBorrado(false) {}

    // Guardar en archivo
    void guardarEnArchivo(std::ofstream& archivo) const {
        size_t lenNombre = nombre.length();
        size_t lenDesc = descripcion.length();

        archivo.write(reinterpret_cast<const char*>(&id), sizeof(int));
        archivo.write(reinterpret_cast<const char*>(&lenNombre), sizeof(size_t));
        archivo.write(nombre.c_str(), lenNombre);
        archivo.write(reinterpret_cast<const char*>(&lenDesc), sizeof(size_t));
        archivo.write(descripcion.c_str(), lenDesc);
        archivo.write(reinterpret_cast<const char*>(&marcadoComoBorrado), sizeof(bool));
    }

    // cargar
    
    void cargarDesdeArchivo(std::ifstream& archivo) {
        size_t lenNombre = 0;
        size_t lenDesc = 0;
        char buffer[1024];

        archivo.read(reinterpret_cast<char*>(&id), sizeof(int));
        
        archivo.read(reinterpret_cast<char*>(&lenNombre), sizeof(size_t));
        archivo.read(buffer, lenNombre);
        nombre = std::string(buffer, lenNombre);

        archivo.read(reinterpret_cast<char*>(&lenDesc), sizeof(size_t));
        archivo.read(buffer, lenDesc);
        descripcion = std::string(buffer, lenDesc);

        archivo.read(reinterpret_cast<char*>(&marcadoComoBorrado), sizeof(bool));
    }

    void imprimir() const {
        if (marcadoComoBorrado) {
            std::cout << "[BORRADO] ";
        }
        std::cout << "ID: " << id << " | Nombre: " << nombre 
                  << " | Descripcion: " << descripcion << std::endl;
    }
};

class GestorInfo {
private:
    std::string archivoBase;
    int proximoId;

public:
    GestorInfo(const std::string& nombreArchivoBase) 
        : archivoBase(nombreArchivoBase), proximoId(0) {
        cargarProximoId();
    }

    // Generar nombre de archivo
    
    std::string generarNombreArchivo(int id) const {
        return archivoBase + "_" + std::to_string(id) + ".bin";
    }

    // Guardar registro
    
    bool guardarRegistro(const Persistencia& registro) {
        std::string nombreArchivo = generarNombreArchivo(registro.id);
        std::ofstream archivo(nombreArchivo, std::ios::binary);

        if (!archivo.is_open()) {
            return false;
        }

        registro.guardarEnArchivo(archivo);
        archivo.close();
        return true;
    }

    // Cargar registro
    
    bool cargarRegistro(int id, Persistencia& registro) const {
        std::string nombreArchivo = generarNombreArchivo(id);
        std::ifstream archivo(nombreArchivo, std::ios::binary);

        if (!archivo.is_open()) {
            return false;
        }

        registro.cargarDesdeArchivo(archivo);
        archivo.close();
        return true;
    }

    // Obtener ID para nuevo algun nuevo registro
    int obtenerNuevoId() {
        return proximoId++;
    }

    // Guardar próximo ID
    void guardarProximoId() {
        std::ofstream archivo(archivoBase + "_ids.bin", std::ios::binary);
        if (archivo.is_open()) {
            archivo.write(reinterpret_cast<const char*>(&proximoId), sizeof(int));
            archivo.close();
        }
    }

    // Cargar próximo ID
    void cargarProximoId() {
        std::ifstream archivo(archivoBase + "_ids.bin", std::ios::binary);
        if (archivo.is_open()) {
            archivo.read(reinterpret_cast<char*>(&proximoId), sizeof(int));
            archivo.close();
        } else {
            proximoId = 0;
        }
    }
};

#endif
