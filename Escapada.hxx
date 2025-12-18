#pragma once
#include "Experiencia.hxx"

#pragma db object
class Escapada : public Experiencia {
public:
    // Constructor ajustado: Pasa el precio al padre
    Escapada(std::string n, std::string d, std::string c, int maxP, float preu, std::string dA,
        std::string hotel, int nits)
        : Experiencia(n, d, c, maxP, preu, dA), hotel(hotel), numNits(nits) {
    }

    std::string obteTipus() const override { return "EXPERIÈNCIA"; } // O "ESCAPADA"
    std::string obteDadesEspecifiques() const override {
        return "Hotel: " + hotel + ", Nits: " + std::to_string(numNits);
    }

private:
    friend class odb::access;
    Escapada() : numNits(0) {}

    std::string hotel;
    int numNits;
    // 'preu' eliminado de aquí, ahora usa el de Experiencia
};