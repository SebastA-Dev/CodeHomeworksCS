#include <iostream>
#include <windows.h>
#include <x86intrin.h>
#include <vector>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <array>
#include <functional>

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

//VECTORES DE APUNTADORES A FUNCIONES

typedef vector<size_t> (*Codigo1Func)(int);
typedef vector<size_t> (*Codigo2Func)(int, int);
typedef vector<size_t> (*Codigo3Func)(int);
typedef vector<size_t> (*Codigo4Func)(int);


// Punteros a funciones de tiempo modificadas

double medir_tiempo_chrono(Codigo2Func f, int N) {
    auto start = std::chrono::high_resolution_clock::now();
    vector<size_t> mem = f(N, N);
    auto end = std::chrono::high_resolution_clock::now();
    double tiempo_ns = std::chrono::duration<double, std::nano>(end - start).count();
    return tiempo_ns;
}
double medir_tiempo_qpc(Codigo2Func f, int N) {
    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
    vector<size_t> mem = f(N, N);
    QueryPerformanceCounter(&end);
    double elapsed_ns = (end.QuadPart - start.QuadPart) * 1e9 / freq.QuadPart;
    return elapsed_ns;
}
double medir_tiempo_rdtsc(Codigo2Func f, int N) {
    unsigned long long start = __rdtsc();
    vector<size_t> mem = f(N, N);
    unsigned long long end = __rdtsc();
    double frecuencia_ghz = obtener_velocidad_CPU();
    double frecuencia_hz = frecuencia_ghz * 1e9;
    double tiempo_ns = (end - start) * (1e9 / frecuencia_hz);
    return tiempo_ns;
}
double medir_tiempo_preciso(Codigo2Func f, int N) {
    using GetSystemTimePreciseAsFileTime_t = VOID (WINAPI*)(LPFILETIME);
    HMODULE hKernel = GetModuleHandleW(L"kernel32.dll");
    GetSystemTimePreciseAsFileTime_t pPrecise = nullptr;
    if (hKernel) {
        pPrecise = (GetSystemTimePreciseAsFileTime_t)GetProcAddress(hKernel, "GetSystemTimePreciseAsFileTime");
    }
    if (!pPrecise) return 0;
    FILETIME ftStart, ftEnd;
    pPrecise(&ftStart);
    vector<size_t> mem = f(N, N);
    pPrecise(&ftEnd);
    ULARGE_INTEGER uStart, uEnd;
    uStart.LowPart  = ftStart.dwLowDateTime;
    uStart.HighPart = ftStart.dwHighDateTime;
    uEnd.LowPart    = ftEnd.dwLowDateTime;
    uEnd.HighPart   = ftEnd.dwHighDateTime;
    unsigned long long diff100ns = (uEnd.QuadPart - uStart.QuadPart);
    unsigned long long tiempo_ns  = diff100ns * 100ULL;
    return static_cast<double>(tiempo_ns);
}

// Wrapper para adaptar c�digos al tipo 2

vector<size_t> codigo1_wrapper(int N, int unused) { return codigo1(N); }
vector<size_t> codigo3_wrapper(int N, int unused) { return codigo3_listas(N); }
vector<size_t> codigo4_wrapper(int N, int unused) { return codigo4_structs(N); }

// Funcion para medir todos los tiempos para un c�digo con un formato especfico

	void medir_y_guardar(ofstream &salida,
    	const string& codigo_nombre,
    	int codigo_num,
    	Codigo2Func codigo_func,
    	int N_min,
    	int N_max,
    	int N_step) 
{
    

    for (int i = N_min; i <= N_max; i += N_step) {
        double t1 = medir_tiempo_chrono(codigo_func, i);
        double t2 = medir_tiempo_qpc(codigo_func, i);
        double t3 = medir_tiempo_rdtsc(codigo_func, i);
        double t4 = medir_tiempo_preciso(codigo_func, i);

        // Guardar tiempos
		auto formatear = [](double ns) {
    		long long tiempo_ns = static_cast<long long>(ns);
    		char buffer[64];
    	snprintf(buffer, sizeof(buffer), "%lld", tiempo_ns);
    	return string(buffer);
		};		

        salida << codigo_num << " - " << codigo_nombre << ";" << i << ";" 
               << formatear(t1) << ";" << formatear(t2) << ";" << formatear(t3) << ";" << formatear(t4) 
               << "\n";
    }
}


int main() {
    ofstream salida("analisis_memoria.txt");
    
    //medir_y_guardar(salida, "Lista enlazada", 1, codigo1_wrapper, 10, 500, 10);
    //medir_y_guardar(salida, "Array de listas", 2, codigo2, 10, 500, 10);
    medir_y_guardar(salida, "Array 3D de listas", 3, codigo3_wrapper, 410, 500, 10);
    medir_y_guardar(salida, "Array 2D de structs", 4, codigo4_wrapper, 10, 500, 10);

    salida.close();
    return 0;
}
