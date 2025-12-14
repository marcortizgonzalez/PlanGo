#include "CapaDePresentacio.hxx"
#include "CapaDeDomini.hxx"
#include <iostream>
#include <string>
#include <vector>
#include <iomanip> // Para formatear la salida (opcional)

// Implementación de las operaciones definidas en el .hxx

void CapaDePresentacio::consultaNovetats() {
    try {
        // 1. Llamamos a la Capa de Dominio para obtener los datos (DTOs)
        // El dominio ya se encarga de ordenar y limitar a 10 resultados
        CapaDeDomini& domini = CapaDeDomini::getInstance();
        std::vector<DTOExperiencia> novetats = domini.consultarNovetats();

        // 2. Mostramos los resultados por pantalla
        std::cout << "\n===== NOVETATS (TOP 10) =====" << std::endl;

        if (novetats.empty()) {
            std::cout << "No hi ha experiencies disponibles." << std::endl;
        }
        else {
            for (const auto& dto : novetats) {
                std::cout << "------------------------------------------------" << std::endl;
                // Usamos los getters del DTO
                std::cout << "[" << dto.obteTipus() << "] " << dto.obteNom() << std::endl;
                std::cout << "Ciutat: " << dto.obteCiutat() << std::endl;
                std::cout << "Descripcio: " << dto.obteDescripcio() << std::endl;
                std::cout << "Detalls: " << dto.obteDetalls() << std::endl;

                // Mostrar categorías (si las incluiste en el DTO)
                std::cout << "Categories: ";
                for (const auto& cat : dto.obteCategories()) {
                    std::cout << cat << " ";
                }
                std::cout << std::endl;
            }
            std::cout << "------------------------------------------------" << std::endl;
        }

        // Pausa para que el usuario pueda leer
        std::cout << "\nPrem Enter per tornar al menu...";
        std::cin.ignore();
        std::cin.get();

    }
    catch (const std::exception& e) {
        // Capturamos cualquier error que venga de capas inferiores
        std::cout << "Error al consultar novetats: " << e.what() << std::endl;
    }
}

void CapaDePresentacio::esborrarUsuari() {
    std::string password;
    std::string confirmacio;

    std::cout << "\n===== ESBORRAR USUARI =====" << std::endl;
    std::cout << "ATENCIO: Aquesta accio eliminara el teu compte i totes les teves reserves." << std::endl;

    // 1. Pedimos la contraseña (como especificaba tu diseño)
    std::cout << "Introdueix la teva contrasenya per confirmar: ";
    std::cin >> password;

    try {
        CapaDeDomini& domini = CapaDeDomini::getInstance();

        // Verificamos contraseña y pedimos confirmación final
        // (Nota: Podrías llamar a un método 'verificarPassword' de dominio aquí, 
        // pero según tu diseño anterior, la operación 'esborrarUsuari' de dominio lo hace todo)

        std::cout << "Estas segur que vols continuar? (S/N): ";
        std::cin >> confirmacio;

        if (confirmacio == "S" || confirmacio == "s") {
            // Llamada al dominio para ejecutar la acción
            domini.esborrarUsuari(password);
            std::cout << "El teu compte ha estat esborrat correctament. Adeu!" << std::endl;

            // Aquí podrías forzar la salida del programa o volver al menú principal (que debería detectar que no hay usuario)
            exit(0);
        }
        else {
            std::cout << "Operacio cancel.lada." << std::endl;
        }

    }
    catch (const std::exception& e) {
        // Aquí capturamos la excepción "Contrasenya incorrecta" que lanza el Dominio
        std::cout << "Error: " << e.what() << std::endl;
    }
}