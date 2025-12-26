#pragma once
#include "Experiencia.hxx"

// Clase que representa una experiencia de tipo Escapada (Hotel + Noches)
#pragma db object
class Escapada : public Experiencia {
public:
    // Constructor: Inicializa la experiencia base y añade los detalles del hotel y noches
    Escapada(std::string n, std::string d, std::string c, int maxP, float preu, std::string dA,
        std::string hotel, int nits)
        : Experiencia(n, d, c, maxP, preu, dA), hotel(hotel), numNits(nits) {
    }

    // Devuelve el identificador del tipo de experiencia
    std::string obteTipus() const override { return "ESCAPADA"; }

    // Retorna una cadena con los detalles específicos (Hotel y Noches)
    std::string obteDadesEspecifiques() const override {
        return "Hotel: " + hotel + ", Nits: " + std::to_string(numNits);
    }

private:
    friend class odb::access;
    Escapada() : numNits(0) {} // Constructor vacío para ODB

    std::string hotel;
    int numNits;
};