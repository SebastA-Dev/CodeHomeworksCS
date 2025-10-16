#include <iostream>
#include <vector>
#include <cstdio>
#include "../include/SortingAlgorithm.h"
#include <array>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <string>
#include <windows.h>
#include <x86intrin.h>

// Formatea nanosegundos a "ss:mmm:nnnnnn"
static inline std::string format_ns(unsigned long long ns) {
    unsigned long long s = ns / 1000000000ULL;
    unsigned long long ms = (ns / 1000000ULL) % 1000ULL;
    unsigned long long rem_ns = ns % 1000000ULL;

    std::ostringstream ss;
    ss << std::setfill('0') << std::setw(2) << s << ":"
       << std::setfill('0') << std::setw(3) << ms << ":"
       << std::setfill('0') << std::setw(6) << rem_ns;
    return ss.str();
}

// Obtiene la frecuencia de la CPU en GHz
static inline double get_cpu_freq_ghz() {
    std::array<char, 128> buffer;
    std::string result;
    FILE* pipe = _popen("wmic cpu get currentclockspeed", "r");
    if (!pipe) return -1.0;
    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) result += buffer.data();
    _pclose(pipe);
    std::string::size_type pos = result.find_first_of("0123456789");
    if (pos != std::string::npos) {
        int mhz = std::stoi(result.substr(pos));
        return mhz / 1000.0;
    }
    return -1.0;
}

template<typename Obj, typename M>
static inline unsigned long long time_chrono(M m, Obj& obj, std::vector<int>& data) {
    using namespace std::chrono;
    auto start = high_resolution_clock::now();
    (obj.*m)(data);
    auto end = high_resolution_clock::now();
    return static_cast<unsigned long long>(duration_cast<nanoseconds>(end - start).count());
}

template<typename Obj, typename M>
static inline unsigned long long time_qpc(M m, Obj& obj, std::vector<int>& data) {
    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
    (obj.*m)(data);
    QueryPerformanceCounter(&end);
    double ns = (end.QuadPart - start.QuadPart) * 1e9 / freq.QuadPart;
    return static_cast<unsigned long long>(ns);
}

template<typename Obj, typename M>
static inline unsigned long long time_rdtsc(M m, Obj& obj, std::vector<int>& data) {
    unsigned long long start = __rdtsc();
    (obj.*m)(data);
    unsigned long long end = __rdtsc();
    unsigned long long cycles = end - start;
    double ghz = get_cpu_freq_ghz();
    if (ghz <= 0) return 0ULL;
    double hz = ghz * 1e9;
    double ns = cycles * (1e9 / hz);
    return static_cast<unsigned long long>(ns);
}

template<typename Obj, typename M>
static inline unsigned long long time_precise(M m, Obj& obj, std::vector<int>& data) {
    using GetSystemTimePreciseAsFileTime_t = VOID (WINAPI*)(LPFILETIME);
    HMODULE hKernel = GetModuleHandleW(L"kernel32.dll");
    GetSystemTimePreciseAsFileTime_t pPrecise = nullptr;
    if (hKernel) pPrecise = (GetSystemTimePreciseAsFileTime_t)GetProcAddress(hKernel, "GetSystemTimePreciseAsFileTime");
    if (!pPrecise) return time_chrono(m, obj, data);
    FILETIME ftStart, ftEnd;
    pPrecise(&ftStart);
    (obj.*m)(data);
    pPrecise(&ftEnd);
    ULARGE_INTEGER uStart, uEnd;
    uStart.LowPart = ftStart.dwLowDateTime; uStart.HighPart = ftStart.dwHighDateTime;
    uEnd.LowPart = ftEnd.dwLowDateTime; uEnd.HighPart = ftEnd.dwHighDateTime;
    unsigned long long diff100ns = (uEnd.QuadPart - uStart.QuadPart);
    return diff100ns * 100ULL; // convertir a ns
}

// Genera un vector aleatorio de N enteros
std::vector<int> generateRandomVector(int N) {
    std::vector<int> arr(N);
    for (int& x : arr) {
        x = rand() % 1000;
    }
    return arr;
}

int main() {
    int N = 1000;     
    std::vector<int> arr = generateRandomVector(N);
    SortingAlgorithm sorter;

    // Alias para puntero a método de SortingAlgorithm
    using MethodPtr = void (SortingAlgorithm::*)(std::vector<int>&);

    struct AlgoInfo { MethodPtr ptr; const char* name; } algos[] = {
        { &SortingAlgorithm::InsertionAlgorithm, "Insertion" },
        { &SortingAlgorithm::SelectionAlgorithm, "Selection" },
        { &SortingAlgorithm::BubbleAlgorithm,    "Bubble"    },
        { &SortingAlgorithm::ShellAlgorithm,     "Shell"     }
    };

    for (auto &a : algos) {
        std::cout << "=== " << a.name << " Algorithm ===" << std::endl;

        auto copy = arr;
        unsigned long long t1 = time_chrono(a.ptr, sorter, copy);
        std::cout << "  Tool 1 (chrono):  " << format_ns(t1) << std::endl;

        copy = arr;
        unsigned long long t2 = time_qpc(a.ptr, sorter, copy);
        std::cout << "  Tool 2 (QPC):     " << format_ns(t2) << std::endl;

        copy = arr;
        unsigned long long t3 = time_rdtsc(a.ptr, sorter, copy);
        std::cout << "  Tool 3 (RDTSC):   " << format_ns(t3) << std::endl;

        copy = arr;
        unsigned long long t4 = time_precise(a.ptr, sorter, copy);
        std::cout << "  Tool 4 (Precise): " << format_ns(t4) << std::endl;
    }

    return 0;
}

