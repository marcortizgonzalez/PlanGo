#pragma once
#include "Experiencia.hxx"

// Clase que representa una actividad turística (hereda de Experiencia)
#pragma db object
class Activitat : public Experiencia {
public:
    // Constructor: Inicializa los datos comunes en la clase base y guarda la duración específica
    Activitat(std::string n, std::string d, std::string c, int maxP, float preuPer, std::string dA,
        int durada)
        : Experiencia(n, d, c, maxP, preuPer, dA), durada(durada) {
    }

    // Devuelve el identificador del tipo de experiencia
    std::string obteTipus() const override { return "ACTIVITAT"; }

    // Retorna una cadena con los detalles específicos (Duración)
    std::string obteDadesEspecifiques() const override {
        return "Durada: " + std::to_string(durada) + " min";
    }

private:
    friend class odb::access;
    Activitat() : durada(0) {} // Constructor vacío para ODB

    int durada;
};