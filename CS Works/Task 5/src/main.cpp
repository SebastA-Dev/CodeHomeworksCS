
#include <iostream>     
#include <vector>       
#include <iomanip>      
#include <string>
#include <sstream>

#include "../include/RandomVector.h"
#include "../include/Radix.h"
#include "../include/Bases.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>


void print_vec(const std::vector<int>& v, const char* title) {
	std::cout << title;
	for (std::size_t i = 0; i < v.size(); ++i) {
		if (i) std::cout << ", ";
		std::cout << v[i];
		if (i >= 19) { std::cout << ", ..."; break; }
	}
	std::cout << std::endl;
}

int main(int argc, char** argv) {
		
	std::vector<std::string> radix_times;
	std::vector<std::string> bases_times;
	std::vector<size_t> Ns;

	for (size_t i = 0; i < 10000; i+=50)
	{
		Ns.push_back(i);
		std::vector<int> array_normal = make_random_vector(i);

		{
			LARGE_INTEGER freq, start, end;
			QueryPerformanceFrequency(&freq);
			QueryPerformanceCounter(&start);
			radixSort(array_normal);
			QueryPerformanceCounter(&end);
			unsigned long long ticks = static_cast<unsigned long long>(end.QuadPart - start.QuadPart);
			unsigned long long ns = static_cast<unsigned long long>(ticks * 1000000000ULL / static_cast<unsigned long long>(freq.QuadPart));
			unsigned long long minutes = ns / 60000000000ULL;
			unsigned long long seconds = (ns / 1000000000ULL) % 60ULL;
			unsigned long long msecs = (ns / 1000000ULL) % 1000ULL;
			unsigned long long nsrem = ns % 1000000ULL;
			unsigned long long usecs = nsrem / 1000ULL;
			unsigned long long nsecs = nsrem % 1000ULL;
			std::ostringstream oss;
			oss << std::setfill('0') << std::setw(2) << minutes << ":"
				<< std::setw(2) << seconds << ":"
				<< std::setw(3) << msecs << ":"
				<< std::setw(3) << usecs << ":"
				<< std::setw(3) << nsecs;
			radix_times.push_back(oss.str());
		}
	}

	for (size_t i = 0; i < 10000; i+=50)
	{
		std::vector<int> array_normal = make_random_vector(i);

		{
			LARGE_INTEGER freq, start, end;
			QueryPerformanceFrequency(&freq);
			QueryPerformanceCounter(&start);
			basesSort(array_normal);
			QueryPerformanceCounter(&end);
			unsigned long long ticks = static_cast<unsigned long long>(end.QuadPart - start.QuadPart);
			unsigned long long ns = static_cast<unsigned long long>(ticks * 1000000000ULL / static_cast<unsigned long long>(freq.QuadPart));
			unsigned long long minutes = ns / 60000000000ULL;
			unsigned long long seconds = (ns / 1000000000ULL) % 60ULL;
			unsigned long long msecs = (ns / 1000000ULL) % 1000ULL;
			unsigned long long nsrem = ns % 1000000ULL;
			unsigned long long usecs = nsrem / 1000ULL;
			unsigned long long nsecs = nsrem % 1000ULL;
			std::ostringstream oss;
			oss << std::setfill('0') << std::setw(2) << minutes << ":"
				<< std::setw(2) << seconds << ":"
				<< std::setw(3) << msecs << ":"
				<< std::setw(3) << usecs << ":"
				<< std::setw(3) << nsecs;
			bases_times.push_back(oss.str());
		}
	}

	std::cout << "\nRadixSort (Bytes): Tiempos\n";
	for (size_t idx = 0; idx < radix_times.size() && idx < Ns.size(); ++idx) {
		std::cout << "N=" << Ns[idx] << " -> " << radix_times[idx] << " ns\n";
	}

	std::cout << "\nBasesSort (Bases): Tiempos\n";
	for (size_t idx = 0; idx < bases_times.size() && idx < Ns.size(); ++idx) {
		std::cout << "N=" << Ns[idx] << " -> " << bases_times[idx] << " ns\n";
	}

	return 0;
}
