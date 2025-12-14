#pragma once
#include "CapaDeDomini.hxx"

class CapaDePresentacio {
public:
    // Patrón Singleton
    static CapaDePresentacio& getInstance() {
        static CapaDePresentacio instance;
        return instance;
    }

    // Métodos que procesan las opciones del menú
    void consultaNovetats();
    void esborrarUsuari();

private:
    CapaDePresentacio() = default;
    // Evitar copias
    CapaDePresentacio(const CapaDePresentacio&) = delete;
    void operator=(const CapaDePresentacio&) = delete;
};