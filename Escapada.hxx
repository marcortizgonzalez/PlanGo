// Escapada.hxx
#pragma once
#include "Experiencia.hxx"

#pragma db object
class Escapada : public Experiencia {
public:
    Escapada(std::string n, std::string d, std::string c, int maxP, std::string dA,
        std::string hotel, int nits, float preu)
        : Experiencia(n, d, c, maxP, dA), hotel(hotel), numNits(nits), preu(preu) {
    }

    // Implementación del polimorfismo
    std::string obteTipus() const override { return "EXPERIÈNCIA"; } // Según enunciado
    std::string obteDadesEspecifiques() const override {
        return "Hotel: " + hotel + ", Nits: " + std::to_string(numNits) + ", Preu: " + std::to_string(preu) + "€";
    }

private:
    friend class odb::access;
    Escapada() : numNits(0), preu(0.0) {}

    std::string hotel;
    int numNits;
    float preu;
};