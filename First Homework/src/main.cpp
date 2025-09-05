#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

struct nodo {
    int x,y;
    nodo *sig;
};

struct nodo2 {
    int a;
    float b;
};

/*

INTEGRANTES

SEBASTIAN AVENDAÃ‘O 20232020101
SERGIO MENDIVELSO 20231020227

*/

// ------------------- FUNCIONES -------------------

vector<size_t> codigo1(int N){    
    vector<size_t> memoria_usada;
    nodo *cab = NULL, *Aux;
    
    size_t mem_inicial = sizeof(cab) + sizeof(Aux);
    memoria_usada.push_back(mem_inicial);
    
    for(int i = 0; i < N; ++i) {
        Aux = new nodo;
        Aux->x = i;
        Aux->y = N-i;
        Aux->sig = cab;
        cab = Aux;
        memoria_usada.push_back(mem_inicial + sizeof(nodo) * (i+1));
    }
    
    Aux = cab;
    while (Aux != NULL) Aux = Aux->sig; 
    
    for (Aux=cab->sig; Aux!=NULL; Aux=Aux->sig) {
        delete cab;
        cab = Aux;
    }
    delete cab;    
    return memoria_usada;
}

vector<size_t> codigo2(int M, int tam) {
    vector<size_t> memoria_usada;
    nodo **arr, *Aux;
    arr = new nodo *[M];
    size_t mem_array = sizeof(nodo*) * M;
    memoria_usada.push_back(mem_array);

    for (int i = 0; i < M; i++) arr[i] = NULL;

    int total_nodos = 0;
    for (int i = 0; i < M; i++) {
        for (int j = 1; j <= tam; j++) {
            Aux = new nodo;
            Aux->x = j;
            Aux->y = M + j * i;
            Aux->sig = arr[i];
            arr[i] = Aux;
            total_nodos++;
            memoria_usada.push_back(mem_array + sizeof(nodo) * total_nodos);
        }
    }

    for (int i = 0; i < M; i++) {
        for (Aux = arr[i]->sig; Aux != NULL; Aux = Aux->sig) {
            delete arr[i];
            arr[i] = Aux;
        }
        delete arr[i];        
    }
    delete[] arr;    
    return memoria_usada;
}

vector<size_t> codigo3_listas(int N) {
    vector<size_t> memoria_usada;
    nodo ***a, *Aux;
    a = new nodo **[N];
    size_t mem_array3d = sizeof(nodo**) * N;
    size_t mem_arrays2d = 0;
    for (int i = 0; i < N; i++) {
        a[i] = new nodo *[N];
        mem_arrays2d += sizeof(nodo*) * N;
        for (int j = 0; j < N; j++) a[i][j] = NULL;
    }
    size_t mem_estructuras = mem_array3d + mem_arrays2d;
    memoria_usada.push_back(mem_estructuras);

    int total_nodos = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                Aux = new nodo;
                Aux->x = j*k;
                Aux->y = i+k;
                Aux->sig = a[i][j];
                a[i][j] = Aux;
                total_nodos++;
                memoria_usada.push_back(mem_estructuras + sizeof(nodo) * total_nodos);
            }
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (Aux = a[i][j]->sig; Aux != NULL; Aux = Aux->sig) {
                delete a[i][j];
                a[i][j] = Aux;
            }
            delete a[i][j];
        }
        delete a[i];
    }
    delete[] a;
    memoria_usada.push_back(0);
    return memoria_usada;
}

vector<size_t> codigo3_structs(int N) {
    vector<size_t> memoria_usada;
    nodo2 **a;
    a = new nodo2 *[N];
    size_t mem_array_ptrs = sizeof(nodo2*) * static_cast<size_t>(N);
    memoria_usada.push_back(mem_array_ptrs);

    size_t mem_total_structs = 0;
    for (int i = 0; i < N; i++) {
        a[i] = new nodo2[N];
        mem_total_structs += sizeof(nodo2) * static_cast<size_t>(N);

        for (int j = 0; j < N; j++) {
            a[i][j].a = j*i;
            a[i][j].b = static_cast<float>(j+i);
            
            memoria_usada.push_back(mem_array_ptrs + mem_total_structs);
        }
    }

    for (int i = 0; i < N; i++) delete[] a[i];
    delete[] a;
    return memoria_usada;
}


// ------------------- MAIN -------------------

//NOTA: MAIN ANTERIOR QUE MUESTRA TODAS LAS ITERACIONES DE CADA VALOR DE N POSIBLE


/*
void guardar_resultados(ofstream &f, const vector<size_t>& datos, const string& nombre, int N) {
    f << "\n=== " << nombre << ", N = " << N << " ===\n";
    f << "Paso\tMemoria (bytes)\n";
    for (size_t i = 0; i < datos.size(); i++) {
        f << i << "\t" << datos[i] << "\n";
    }
    size_t maximo = 0;
    for (size_t mem : datos) if (mem > maximo) maximo = mem;
    f << "Máximo uso de memoria: " << maximo << " bytes\n";
}

int main() {
    ofstream salida("analisis_memoria.txt");
    salida << "Análisis de complejidad espacial usando sizeof\n";
    salida << "sizeof(nodo): " << sizeof(nodo) << " bytes\n";
    salida << "sizeof(nodo2): " << sizeof(nodo2) << " bytes\n";
    salida << "sizeof(nodo*): " << sizeof(nodo*) << " bytes\n";

    for (int i = 10; i <= 200; i += 10) {
        vector<size_t> mem1 = codigo1(i);
        guardar_resultados(salida, mem1, "CODIGO1 - Lista enlazada", i);

        vector<size_t> mem2 = codigo2(i, i);
        guardar_resultados(salida, mem2, "CODIGO2 - Array de listas", i);

        vector<size_t> mem3 = codigo3_listas(i);
        guardar_resultados(salida, mem3, "CODIGO3 - Array 3D de listas", i);

        vector<size_t> mem4 = codigo3_structs(i);
        guardar_resultados(salida, mem4, "CODIGO3 - Array 2D de structs", i);
    }

    salida.close();
    return 0;
}
*/
int main() {
    ofstream salida("analisis_memoria.txt");
    salida << "Codigo; N; Max de bytes usados \n";
    

// NOTA: EN CASO DE NO ENSEÑAR TODAS LAS ITERACIONES, HACERLAS POR SECCIONES REDUCIENDO LOS VALORES DE i O EJECUTANDO CADA FOR POR SEPARADO

	for (int i = 10; i <= 500; i += 10) {
        vector<size_t> mem = codigo1(i);
        size_t max_mem = 0;
        for (size_t m : mem) if (m > max_mem) max_mem = m;
        salida << "CODIGO1 - Lista enlazada; " << i << "; " << max_mem << "\n";
    }

   	for (int i = 10; i <= 500; i += 10) {
        vector<size_t> mem = codigo2(i, i);
        size_t max_mem = 0;
        for (size_t m : mem) if (m > max_mem) max_mem = m;
        salida << "CODIGO2 - Array de listas; " << i << "; " << max_mem << "\n";
    }

	
    for (int i = 10; i <= 500; i += 10) {
        vector<size_t> mem = codigo3_listas(i);
        size_t max_mem = 0;
        for (size_t m : mem) if (m > max_mem) max_mem = m;
        salida << "CODIGO3 - Array 3D de listas; " << i << "; " << max_mem << "\n";
    }

    for (int i = 10; i <= 500; i += 10) {
        vector<size_t> mem = codigo3_structs(i);
        size_t max_mem = 0;
        for (size_t m : mem) if (m > max_mem) max_mem = m;
        salida << "CODIGO4 - Array 2D de structs; " << i << "; " << max_mem << "\n";
    }
*/
    salida.close();
    return 0;
}

