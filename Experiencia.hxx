#pragma once
#include <string>
#include <vector>
#include <memory> // Para std::shared_ptr
#include <odb/core.hxx>
#include "Categoria.hxx"

// Importante: 'polymorphic' habilita la herencia en la BD
#pragma db object polymorphic
class Experiencia {
public:
    virtual ~Experiencia() {}

    // Getters básicos
    const std::string& getNom() const { return nom; }
    const std::string& getDescripcio() const { return descripcio; }
    const std::string& getCiutat() const { return ciutat; }
    const std::string& getDataAlta() const { return dataAlta; } // Simplificado como string (YYYY-MM-DD)
    int getMaximPlaces() const { return maximPlaces; }

    // Acceso a categorías (Relación Muchos a Muchos o 1 a N según diseño)
    const std::vector<std::shared_ptr<Categoria>>& getCategories() const { return categories; }

    // Métodos abstractos del Diseño (Polimorfismo puro)
    virtual std::string obteTipus() const = 0;
    virtual std::string obteDadesEspecifiques() const = 0;

protected:
    friend class odb::access;
    // Inicializamos numéricos a 0
    Experiencia() : maximPlaces(0) {}

    // Constructor para las hijas
    Experiencia(std::string n, std::string d, std::string c, int maxP, std::string dA)
        : nom(n), descripcio(d), ciutat(c), maximPlaces(maxP), dataAlta(dA) {
    }

#pragma db id
    std::string nom;

    std::string descripcio;
    std::string ciutat;
    int maximPlaces;
    std::string dataAlta;

    // Relación con Categoria
    std::vector<std::shared_ptr<Categoria>> categories;
};