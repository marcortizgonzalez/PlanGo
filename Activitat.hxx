#pragma once
#include "Experiencia.hxx"

#pragma db object
class Activitat : public Experiencia {
public:
    // Constructor ajustado: 'preuPer' se pasa al padre como 'preu'
    Activitat(std::string n, std::string d, std::string c, int maxP, float preuPer, std::string dA,
        int durada)
        : Experiencia(n, d, c, maxP, preuPer, dA), durada(durada) {
    }

    std::string obteTipus() const override { return "ACTIVITAT"; }
    std::string obteDadesEspecifiques() const override {
        return "Durada: " + std::to_string(durada) + " min";
    }

private:
    friend class odb::access;
    Activitat() : durada(0) {}

    int durada;
    // 'preuPersona' eliminado, usamos 'preu' de Experiencia
};