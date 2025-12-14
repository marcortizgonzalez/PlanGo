#include <iostream>
#include "CapaDeDomini.hxx"


void mostrarMenu() {
    std::cout << "1. Consultar Novetats" << std::endl;
    std::cout << "2. Esborrar Usuari" << std::endl;
    std::cout << "0. Sortir" << std::endl;
}

int main() {
    CapaDeDomini::getInstance().iniciarSessio("marcortiz");

    int opcio = -1;
    while (opcio != 0) {
        mostrarMenu();
        std::cin >> opcio;

        if (opcio == 1) {
            auto novetats = CapaDeDomini::getInstance().consultarNovetats();
            std::cout << "--- NOVETATS ---" << std::endl;
            for (const auto& dto : novetats) {
                std::cout << dto.tipus << ": " << dto.nom << " (" << dto.detalls << ")" << std::endl;
            }
        }
        else if (opcio == 2) {
            std::string pwd;
            std::cout << "Password: ";
            std::cin >> pwd;
            try {
                CapaDeDomini::getInstance().esborrarUsuari(pwd);
                std::cout << "Esborrat!" << std::endl;
            }
            catch (std::exception& e) {
                std::cout << "Error: " << e.what() << std::endl;
            }
        }
    }
    return 0;
}