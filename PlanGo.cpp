#include <iostream>
#include <exception>
#include <locale> // Opcional, para acentos si se configura
#include "CapaDePresentacio.hxx"

int main() {
    // Configuración opcional para mostrar acentos en la consola (según Lab 1)
    try {
        std::locale::global(std::locale(""));
    }
    catch (...) {
        // Si falla la configuración regional, continuamos sin ella
    }

    try {
        // --- PUNTO DE ENTRADA ---
        // Delegamos el control a la Capa de Presentación.
        // El método inici() contiene el bucle del menú principal (No Loggejat).
        CapaDePresentacio::getInstance().inici();
    }
    catch (const std::exception& e) {
        // Capturamos cualquier error fatal que no haya sido controlado antes
        std::cerr << "\nERROR FATAL: " << e.what() << std::endl;
        system("pause"); // Pausa para leer el error antes de salir
        return 1;
    }
    catch (...) {
        std::cerr << "\nError desconegut inesperat." << std::endl;
        system("pause");
        return 1;
    }

    // Pausa final para que la ventana no se cierre de golpe al salir ordenadamente
    // (Útil si ejecutas fuera de Visual Studio o si VS no pausa automáticamente)
    system("pause");
    return 0;
}