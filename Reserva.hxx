#pragma once
#include <string>
#include <memory>
#include <odb/core.hxx>

// Forward declarations para romper dependencias circulares
class Usuari;
class Experiencia;

#pragma db object
class Reserva {
public:
    // Getters y Setters...

private:
    friend class odb::access;
    Reserva() {}

#pragma db id auto
    unsigned long id; // ID autogenerado para la reserva

    int numPlaces;
    float preuPagat;
    std::string dataReserva;

    // Relación N:1 -> Una reserva pertenece a un Usuario
#pragma db not_null
    std::shared_ptr<Usuari> usuari;

    // Relación N:1 -> Una reserva es de una Experiencia
#pragma db not_null
    std::shared_ptr<Experiencia> experiencia;
};