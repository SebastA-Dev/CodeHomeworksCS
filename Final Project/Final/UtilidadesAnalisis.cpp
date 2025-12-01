// UtilidadesAnalisis.cpp
#include "UtilidadesAnalisis.h"
#include <sstream>
#include <iomanip>

std::string UtilidadesAnalisis::stringToHex(const std::string &input) {
    std::ostringstream oss;
    for (unsigned char c : input) {        
        oss << std::hex << std::setw(2) << std::setfill('0')
            << static_cast<int>(c);
    }
    return oss.str();
}

