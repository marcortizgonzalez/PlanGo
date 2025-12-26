#pragma once
#include <string>
#include <memory>
#include <odb/core.hxx>

// Clase base abstracta que representa una experiencia genérica (polimórfica para ODB)
#pragma db object polymorphic
class Experiencia {
public:
    // Destructor virtual para asegurar la correcta destrucción de objetos derivados
    virtual ~Experiencia() {}

    // --- MÉTODOS DE ACCESO (Getters) ---
    const std::string& getNom() const { return nom; }           // Devuelve el nombre (Clave primaria)
    const std::string& getDescripcio() const { return descripcio; } // Devuelve la descripción
    const std::string& getCiutat() const { return ciutat; }     // Devuelve la ciudad
    const std::string& getDataAlta() const { return dataAlta; } // Devuelve la fecha de creación
    int getMaximPlaces() const { return maximPlaces; }          // Devuelve el aforo total

    // Atributos de gestión de reservas y precios
    float getPreu() const { return preu; }                      // Devuelve el precio base
    int getNumReserves() const { return numReserves; }          // Devuelve el número de plazas ocupadas

    // --- MÉTODOS DE MODIFICACIÓN (Setters) ---

    // Actualiza el contador de reservas (usado al confirmar una reserva)
    void setNumReserves(int n) { numReserves = n; }

    // --- MÉTODOS ABSTRACTOS (A implementar por las hijas) ---

    // Devuelve el tipo de experiencia ("ESCAPADA" o "ACTIVITAT")
    virtual std::string obteTipus() const = 0;

    // Devuelve una cadena con los detalles específicos (Duración o Hotel/Noches)
    virtual std::string obteDadesEspecifiques() const = 0;

protected:
    friend class odb::access;

    // Constructor vacío requerido por ODB
    Experiencia() : maximPlaces(0), preu(0.0), numReserves(0) {}

    // Constructor protegido para uso de las clases derivadas (Escapada/Activitat)
    Experiencia(std::string n, std::string d, std::string c, int maxP, float p, std::string dA)
        : nom(n), descripcio(d), ciutat(c), maximPlaces(maxP), preu(p), dataAlta(dA), numReserves(0) {
    }

    // --- ATRIBUTOS PERSISTENTES ---

#pragma db id
    std::string nom;          // Clave primaria

    std::string descripcio;
    std::string ciutat;
    std::string dataAlta;
    int maximPlaces;
    float preu;
    int numReserves;
};