#pragma once
#include <string>
#include <memory>
#include <odb/core.hxx>

#pragma db object polymorphic
class Experiencia {
public:
    virtual ~Experiencia() {}

    // Getters
    const std::string& getNom() const { return nom; }
    const std::string& getDescripcio() const { return descripcio; }
    const std::string& getCiutat() const { return ciutat; }
    const std::string& getDataAlta() const { return dataAlta; }
    int getMaximPlaces() const { return maximPlaces; }

    // NUEVOS ATRIBUTOS EN LA BASE
    float getPreu() const { return preu; }
    int getNumReserves() const { return numReserves; }

    // Setters necesarios para actualizar reservas
    void setNumReserves(int n) { numReserves = n; }

    // Métodos abstractos
    virtual std::string obteTipus() const = 0;
    virtual std::string obteDadesEspecifiques() const = 0;

protected:
    friend class odb::access;
    Experiencia() : maximPlaces(0), preu(0.0), numReserves(0) {}

    Experiencia(std::string n, std::string d, std::string c, int maxP, float p, std::string dA)
        : nom(n), descripcio(d), ciutat(c), maximPlaces(maxP), preu(p), dataAlta(dA), numReserves(0) {
    }

#pragma db id
    std::string nom;

    std::string descripcio;
    std::string ciutat;
    int maximPlaces;

    // MOVIDO AQUÍ DESDE LAS HIJAS
    float preu;

    std::string dataAlta;

    // NUEVO ATRIBUTO
    int numReserves;
};