// Activitat.hxx
#pragma once
#include "Experiencia.hxx"

#pragma db object
class Activitat : public Experiencia {
public:
    Activitat(std::string n, std::string d, std::string c, int maxP, std::string dA,
        int durada, float preuPer)
        : Experiencia(n, d, c, maxP, dA), durada(durada), preuPersona(preuPer) {
    }

    std::string obteTipus() const override { return "ACTIVITAT"; }
    std::string obteDadesEspecifiques() const override {
        return "Durada: " + std::to_string(durada) + " min, Preu/pers: " + std::to_string(preuPersona) + "€";
    }

private:
    friend class odb::access;
    Activitat() : durada(0), preuPersona(0.0) {}

    int durada;
    float preuPersona;
};