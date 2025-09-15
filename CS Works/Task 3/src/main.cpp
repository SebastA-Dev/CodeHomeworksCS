#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <cstdio>
#include <algorithm>
#include "../include/SearchingAlgorithms.h"
// Timing helpers (inlined per request)
#include <array>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <string>
#include <windows.h>
#include <x86intrin.h>

static inline std::string format_ns(unsigned long long ns) {
    unsigned long long s = ns / 1'000'000'000ULL;
    unsigned long long ms = (ns / 1'000'000ULL) % 1000ULL;
    unsigned long long rem_ns = ns % 1'000'000ULL;
    std::ostringstream ss;
    ss << std::setfill('0') << std::setw(2) << s << ":"
       << std::setfill('0') << std::setw(3) << ms << ":"
       << std::setfill('0') << std::setw(6) << rem_ns;
    return ss.str();
}

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
static inline unsigned long long time_chrono(M m, Obj& obj, std::vector<size_t>& data, int target = NULL) {
    using namespace std::chrono;
    auto start = high_resolution_clock::now();
    (obj.*m)(data, target);
    auto end = high_resolution_clock::now();
    return static_cast<unsigned long long>(duration_cast<nanoseconds>(end - start).count());
}

template<typename Obj, typename M>
static inline unsigned long long time_qpc(M m, Obj& obj, std::vector<size_t>& data, int target = NULL) {
    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
    (obj.*m)(data, target);
    QueryPerformanceCounter(&end);
    double ns = (end.QuadPart - start.QuadPart) * 1e9 / freq.QuadPart;
    return static_cast<unsigned long long>(ns);
}

template<typename Obj, typename M>
static inline unsigned long long time_rdtsc(M m, Obj& obj, std::vector<size_t>& data, int target = NULL) {
    unsigned long long start = __rdtsc();
    (obj.*m)(data, target);
    unsigned long long end = __rdtsc();
    unsigned long long cycles = end - start;
    double ghz = get_cpu_freq_ghz();
    if (ghz <= 0) return 0ULL;
    double hz = ghz * 1e9;
    double ns = cycles * (1e9 / hz);
    return static_cast<unsigned long long>(ns);
}

template<typename Obj, typename M>
static inline unsigned long long time_precise(M m, Obj& obj, std::vector<size_t>& data, int target = NULL) {
    using GetSystemTimePreciseAsFileTime_t = VOID (WINAPI*)(LPFILETIME);
    HMODULE hKernel = GetModuleHandleW(L"kernel32.dll");
    GetSystemTimePreciseAsFileTime_t pPrecise = nullptr;
    if (hKernel) pPrecise = (GetSystemTimePreciseAsFileTime_t)GetProcAddress(hKernel, "GetSystemTimePreciseAsFileTime");
    if (!pPrecise) return time_chrono(m, obj, data, target);
    FILETIME ftStart, ftEnd;
    pPrecise(&ftStart);
    (obj.*m)(data, target);
    pPrecise(&ftEnd);
    ULARGE_INTEGER uStart, uEnd;
    uStart.LowPart = ftStart.dwLowDateTime; uStart.HighPart = ftStart.dwHighDateTime;
    uEnd.LowPart = ftEnd.dwLowDateTime; uEnd.HighPart = ftEnd.dwHighDateTime;
    unsigned long long diff100ns = (uEnd.QuadPart - uStart.QuadPart);
    return diff100ns * 100ULL; // to ns
}


// Generates a vector of N random integers
std::vector<size_t> generateSteppedVector(int N) {
    std::vector<size_t> arr(N);
    for (int i = 0; i < N; ++i) {
        arr[i] = i * 10;
    }
    return arr;
}

int main() {
	int maxN = 1000;
	std::vector<size_t> baseArray = generateSteppedVector(maxN);
    std::random_device rd;
    std::mt19937 gen(rd());
    
    // std::uniform_int_distribution<> distrib(0, arr.size() - 1);
    
    SearchingAlgorithms search;
    
    // Helper typedef for member function pointer: methods return int and take (const vector<int>&, int)
    
    using MethodPtr = int (SearchingAlgorithms::*)(std::vector<size_t>&, int);
    
/*
    // // --- Run tests and print grouped output ----------------------
    struct AlgoInfo { MethodPtr ptr; const char* name; } algos[] = {
        { &SearchingAlgorithms::LinearSearch1, "LinearSearch1" },
        { &SearchingAlgorithms::LinearSearch2, "LinearSearch2" },
        { &SearchingAlgorithms::BinarySearch, "BinarySearch" },
        { &SearchingAlgorithms::ShellAlgorithm, "ShellAlgorithm" }
    };

    for (auto &a : algos) {
        int indice = distrib(gen);
        int target = arr[indice];
        std::cout << "=== " << a.name << std::endl;
        auto copy = arr;
        if(a.name == "BinarySearch"){
            std::sort(arr.begin(), arr.end());
            copy = arr;
        }         
        unsigned long long t1 = time_chrono(a.ptr, search, copy, target);
        std::cout << "  Tool 1 (chrono): " << format_ns(t1) << std::endl;

        unsigned long long t2 = time_qpc(a.ptr, search, copy, target);
        std::cout << "  Tool 2 (QPC):    " << format_ns(t2) << std::endl;
        
        unsigned long long t3 = time_rdtsc(a.ptr, search, copy, target);
        std::cout << "  Tool 3 (RDTSC):  " << format_ns(t3) << std::endl;

        unsigned long long t4 = time_precise(a.ptr, search, copy, target);
        std::cout << "  Tool 4 (Precise):" << format_ns(t4) << std::endl;        
    }

    return 0;
    
    */
    
    struct AlgoInfo { MethodPtr ptr; const char* name; } algos[] = {
        { &SearchingAlgorithms::BinarySearch, "BinarySearch" },
        { &SearchingAlgorithms::ShellAlgorithm, "ShellAlgorithm" }
    };

    std::ofstream outFile("resultados.txt");

    for (int N = 10; N <= maxN; N += 10) {
        std::vector<size_t> subArray(baseArray.begin(), baseArray.begin() + N);
        int target = -1; // worst binary search case

        outFile << "N=" << N << std::endl;

        for (auto &a : algos) {
            std::vector<size_t> arrCopy = subArray;

            if (a.name == std::string("ShellAlgorithm")) {
                // worst shell case
                std::shuffle(arrCopy.begin(), arrCopy.end(), gen);
                target = arrCopy[gen() % N]; 
            } else if (a.name == std::string("BinarySearch")) {
                
                std::sort(arrCopy.begin(), arrCopy.end());
                target = -1; 
            }

            unsigned long long t = time_qpc(a.ptr, search, arrCopy, target);
            outFile << a.name << "\t" << format_ns(t) << std::endl;
        }
    }

    outFile.close();
    return 0;
}



