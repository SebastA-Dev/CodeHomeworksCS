#include <iostream>
#include <windows.h>
#include <x86intrin.h>
#include <vector>
#include <fstream>
#include <chrono>
#include <iomanip>
using namespace std;
using namespace chrono;


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

SEBASTIAN AVENDAÑO 20232020101
SERGIO MENDIVELSO 20231020227

*/

// ------------------- FUNCIONES -------------------

 /**
  * FUNCIONES AUXILIARES
  */

  double obtener_velocidad_CPU() {
    std::array<char, 128> buffer;
    std::string result;
    FILE* pipe = _popen("wmic cpu get currentclockspeed", "r");
    if (!pipe) {
        return -1;
    }

    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        result += buffer.data();
    }
    _pclose(pipe);

    std::string::size_type pos = result.find_first_of("0123456789");
    if (pos != std::string::npos) {
        int mhz = std::stoi(result.substr(pos));
        return mhz / 1000.0; 
    }

    return -1; 
}


/**
 * FUNCIONES ALGORITMICAS
 */

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

vector<size_t> codigo4_structs(int N) {
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

/*
 * 
 * FUNCIONES PERFORMANCE
 * 
 */

void resultados()
{
    ofstream salida("analisis_memoria.txt");
    salida << "Codigo; N; Max de bytes usados \n";
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
        vector<size_t> mem = codigo4_structs(i);
        size_t max_mem = 0;
        for (size_t m : mem) if (m > max_mem) max_mem = m;
        salida << "CODIGO4 - Array 2D de structs; " << i << "; " << max_mem << "\n";
    }

    salida.close();
}

void herramienta_tiempo_1(int N) {
    auto start = high_resolution_clock::now();
    vector<size_t> mem = codigo1(N);
    auto end = high_resolution_clock::now();

    auto tiempo_ns = duration_cast<nanoseconds>(end - start).count();

    long long segundos = tiempo_ns / 1'000'000'000;
    long long milisegundos = (tiempo_ns / 1'000'000) % 1000;
    long long nanosegundos = tiempo_ns % 1'000'000;

    cout << "[chrono] "
         << setfill('0') << setw(2) << segundos << ":"
         << setfill('0') << setw(3) << milisegundos << ":"
         << setfill('0') << setw(6) << nanosegundos << endl;
}

void herramienta_tiempo_2(int N) {
    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);

    vector<size_t> mem = codigo1(N);

    QueryPerformanceCounter(&end);

    double elapsed_ns = (end.QuadPart - start.QuadPart) * 1e9 / freq.QuadPart;

    long long segundos = static_cast<long long>(elapsed_ns) / 1'000'000'000;
    long long milisegundos = (static_cast<long long>(elapsed_ns) / 1'000'000) % 1000;
    long long nanosegundos = static_cast<long long>(elapsed_ns) % 1'000'000;

    cout << "[QPC] "
         << setfill('0') << setw(2) << segundos << ":"
         << setfill('0') << setw(3) << milisegundos << ":"
         << setfill('0') << setw(6) << nanosegundos << endl;
}

void herramienta_tiempo_3(int N) {

    unsigned long long start = __rdtsc();
    vector<size_t> mem = codigo1(N);
    unsigned long long end = __rdtsc();

    unsigned long long ciclos = end - start;
    
    double frecuencia_ghz = obtener_velocidad_CPU();
    if (frecuencia_ghz <= 0) {
        cerr << "[RDTSC] Error al obtener frecuencia de CPU\n";
        return;
    }

    double frecuencia_hz = frecuencia_ghz * 1e9;

    // Convertir ciclos a nanosegundos
    double tiempo_ns = ciclos * (1e9 / frecuencia_hz);

    long long segundos = static_cast<long long>(tiempo_ns) / 1'000'000'000;
    long long milisegundos = (static_cast<long long>(tiempo_ns) / 1'000'000) % 1000;
    long long nanosegundos = static_cast<long long>(tiempo_ns) % 1'000'000;

    cout << "[RDTSC] "
         << setfill('0') << setw(2) << segundos << ":"
         << setfill('0') << setw(3) << milisegundos << ":"
         << setfill('0') << setw(6) << nanosegundos << endl;
}

void herramienta_tiempo_4(int N) {
    HANDLE hProcess = GetCurrentProcess();
    FILETIME ftCreation, ftExit, ftKernelStart, ftUserStart, ftKernelEnd, ftUserEnd;

    GetProcessTimes(hProcess, &ftCreation, &ftExit, &ftKernelStart, &ftUserStart);

    vector<size_t> mem = codigo1(N);

    GetProcessTimes(hProcess, &ftCreation, &ftExit, &ftKernelEnd, &ftUserEnd);

    ULARGE_INTEGER kernelStart, userStart, kernelEnd, userEnd;
    kernelStart.LowPart = ftKernelStart.dwLowDateTime;
    kernelStart.HighPart = ftKernelStart.dwHighDateTime;
    userStart.LowPart = ftUserStart.dwLowDateTime;
    userStart.HighPart = ftUserStart.dwHighDateTime;

    kernelEnd.LowPart = ftKernelEnd.dwLowDateTime;
    kernelEnd.HighPart = ftKernelEnd.dwHighDateTime;
    userEnd.LowPart = ftUserEnd.dwLowDateTime;
    userEnd.HighPart = ftUserEnd.dwHighDateTime;

    // Diferencia en 100-ns ticks → convertir a ns
    unsigned long long kernelTime = (kernelEnd.QuadPart - kernelStart.QuadPart) * 100;
    unsigned long long userTime   = (userEnd.QuadPart   - userStart.QuadPart) * 100;

    unsigned long long tiempo_ns = kernelTime + userTime;

    long long segundos = tiempo_ns / 1'000'000'000;
    long long milisegundos = (tiempo_ns / 1'000'000) % 1000;
    long long nanosegundos = tiempo_ns % 1'000'000;

    cout << "[GetProcessTimes] "
         << setfill('0') << setw(2) << segundos << ":"
         << setfill('0') << setw(3) << milisegundos << ":"
         << setfill('0') << setw(6) << nanosegundos << endl;
}

int main() {       
    
    herramienta_tiempo_4(1e7);
    return 0;
}

