#include <iostream>
#include <string>
#include <regex>
#include <algorithm>
#include "Cola1.h"
#include "pila1.h"

using namespace std;

// Limpia el codigo, la idea es quitar cualquier expresion la cual genere ruido al tener
// el palindromo, esto incluye: puntuaciones.
std::string limpiar(std::string texto) {    
     std::transform(texto.begin(), texto.end(), texto.begin(), ::tolower);

    texto = std::regex_replace(texto, std::regex("á"), "a");
    texto = std::regex_replace(texto, std::regex("é"), "e");
    texto = std::regex_replace(texto, std::regex("í"), "i");
    texto = std::regex_replace(texto, std::regex("ó"), "o");
    texto = std::regex_replace(texto, std::regex("ú"), "u");
    texto = std::regex_replace(
        texto,
        std::regex(R"([!¡?¿\s\.,;:\'"()\[\]{}…/\\_@#$%&=+\*\^<>|~\-])"),
        ""
    );

    texto = std::regex_replace(texto, std::regex(R"(\s{2,})"), " ");
    texto = std::regex_replace(texto, std::regex(R"(^\s+|\s+$)"), "");
    return texto;
}

// Segun lo mencionado en Moodle, se genera el string como parametro y es limpio
// Cree un nuevo metodo el cual me carga el string, luego de eso
// genero el equivalente al metodo ToString() de java y comparo
// como se el push basicamente invierte, entonces al ser palindromo deberia de ser igual

// Complejidad es O(n)
bool isPalindrome(string s){        
    pila pl;
    pl.CargarDesdeString(s);
    string s_final = pl.ToString();
    bool result = (s_final == s);
    return result;
}

int main(int argc, char** argv) {

    // Prueba de Palindromo
    string s = "¿Anita lava la tina!12312";
    string limpio = limpiar(s);    
    if(isPalindrome(limpio))
        cout << "hola";

    
    return 0;
}

