#include <fstream>
#include <sstream>
#include <vector>
#include "BlackRedTree.h"

int leerClaveValida(const std::string& mensaje, RedBlackTree& T, bool debeExistir) {
    int clave;
    std::string input;

    while (true) {
        std::cout << mensaje;
        std::getline(std::cin, input);

        std::stringstream ss(input);
        if (!(ss >> clave) || !(ss.eof())) {
            std::cout << "Error: Debes ingresar un numero entero.\n";
            continue;
        }

        if (clave <= 0) {
            std::cout << "Error: La clave debe ser positiva.\n";
            continue;
        }

        if (debeExistir && T.SEARCH(T.root, clave) == T.nil) {
            std::cout << "Error: La clave no existe en el árbol.\n";
            continue;
        }

        if (!debeExistir && T.SEARCH(T.root, clave) != T.nil) {
            std::cout << "Error: La clave ya existe en el árbol.\n";
            continue;
        }

        break;
    }

    return clave;
}

int main() {
    RedBlackTree T;

    ifstream file("names_extended.txt");

    if (!file.is_open()) {
        cout << "No se pudo abrir el archivo.\n";
        return 1;
    }

	string line;
	
	while (getline(file, line)) {
	
	    while (!line.empty() && line[0] == ' ') line.erase(0, 1);
	    while (!line.empty() && line[line.size()-1] == ' ') line.pop_back();
	
	    if (line.empty()) continue;
	
	    size_t pos = line.find('-');
	    if (pos == string::npos) continue;
	
	    int clave = stoi(line.substr(0, pos));
	
	    string name = line.substr(pos + 1);
	
	    while (!name.empty() && name[0] == ' ') name.erase(0, 1);
	    while (!name.empty() && name[name.size()-1] == ' ') name.pop_back();
	    
	    T.RB_INSERT(clave, name);
	
	    cout << "[OK] Insertado: " << clave << " - " << name << "\n";
	}
	
	file.close();
    
    
    int opcion;
    int clave;
    string informacion;

do {
    std::cout << "\n========== MENU ARBOL RED-BLACK ==========\n";
    std::cout << "1. Insertar clave\n";
    std::cout << "2. Eliminar clave\n";
    std::cout << "3. Buscar clave\n";
    std::cout << "4. Modificar informacion de una clave\n";
    std::cout << "5. Mostrar recorridos (Inorder / Preorder / Postorder / Niveles)\n";
    std::cout << "0. Salir\n";
    std::cout << "Seleccione una opcion: ";
    std::cin >> opcion;
    std::cin.ignore();

    switch(opcion) {
        case 1: {
            int clave = leerClaveValida("Ingresa la nueva clave (entero positivo): ", T, false);

            std::string informacion;
            while (true) {
                std::cout << "Ingresa el nuevo nombre: ";
                std::getline(std::cin, informacion);
                if (!informacion.empty()) break;
                std::cout << "Error: El nombre no puede estar vacio.\n";
            }

            T.RB_INSERT(clave, informacion);
            std::cout << "[OK] Nodo insertado: " << clave << " - " << informacion << "\n";
            break;
        }

        case 2: {
            int clave = leerClaveValida("Ingresa la clave a eliminar: ", T, true);

            T.RB_DELETE(clave);
            std::cout << "[OK] Nodo eliminado: " << clave << "\n";

            std::cout << "Arbol Inorder actual: ";
            T.INORDER(T.root);
            std::cout << "\n";
            break;
        }

        case 3: {
            int clave = leerClaveValida("Ingresa la clave a buscar: ", T, true);

            Node* nodo = T.SEARCH(T.root, clave);
            std::cout << "Clave encontrada: " << clave << " - Información: " << nodo->info << "\n";
            break;
        }

        case 4: {
            int clave = leerClaveValida("Ingresa la clave a modificar: ", T, true);

            std::string nuevaInfo;
            std::cout << "Ingresa la nueva información: ";
            std::getline(std::cin, nuevaInfo);

            if (T.MODIFY(clave, nuevaInfo)) {
                std::cout << "[OK] Información de la clave " << clave << " modificada a: " << nuevaInfo << "\n";
            } else {
                std::cout << "Error: No se pudo modificar la clave.\n";
            }

            break;
        }


        case 5: {
            int opcionRecorrido;

            do {
                std::cout << "\n=== MENU DE RECORRIDOS ===\n";
                std::cout << "1. Inorden\n";
                std::cout << "2. Preorden\n";
                std::cout << "3. Postorden\n";
                std::cout << "4. Regresar al menu principal\n";
                std::cout << "Selecciona una opcion: ";
                std::cin >> opcionRecorrido;
                std::cin.ignore();

                switch (opcionRecorrido) {
                    case 1:
                        std::cout << "Recorrido Inorden: ";
                        T.INORDER(T.root);
                        std::cout << "\n";
                        break;
                    case 2:
                        std::cout << "Recorrido Preorden: ";
                        T.PREORDER(T.root);
                        std::cout << "\n";
                        break;
                    case 3:
                        std::cout << "Recorrido Postorden: ";
                        T.POSTORDER(T.root);
                        std::cout << "\n";
                        break;
                    case 4:
                        std::cout << "Regresando al menu principal...\n";
                        break;
                    default:
                        std::cout << "Opcion invalida. Intenta de nuevo.\n";
                }
            } while (opcionRecorrido != 4);

            break;
        }

        case 0:
            std::cout << "Saliendo...\n";
            break;
        default:
            std::cout << "Opcion invalida, intente nuevamente.\n";
    }

} while(opcion != 0);

}
