#include <iostream>
#include <exception>
#include <locale>
#include "CapaDePresentacio.hxx"

// Función principal: Punto de entrada de la aplicación
int main() {
    // Configuración regional para admitir caracteres especiales (acentos, etc.)
    try {
        std::locale::global(std::locale(""));
    }
    catch (...) {
        // Si falla la configuración local, continuamos con la estándar
    }

    try {
        // --- INICIO DEL PROGRAMA ---
        // Delegamos el control a la Capa de Presentación para iniciar la interfaz
        CapaDePresentacio::getInstance().inici();
    }
    catch (const std::exception& e) {
        // Captura de errores fatales no controlados en capas inferiores
        std::cerr << "\nERROR FATAL: " << e.what() << std::endl;
        system("pause");
        return 1;
    }
    catch (...) {
        std::cerr << "\nError desconegut inesperat." << std::endl;
        system("pause");
        return 1;
    }

    // Pausa final para visualizar la salida antes de cerrar la ventana de consola
    system("pause");
    return 0;
}