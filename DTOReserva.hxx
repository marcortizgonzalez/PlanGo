#pragma once
#include <string>
#include <memory>
#include "DTOExperiencia.hxx"

// Clase DTO para transferir datos de reservas a la capa de presentación
class DTOReserva {
public:
    // Constructor: Inicializa el DTO con los detalles de la reserva y la experiencia asociada
    DTOReserva(std::string data, int places, float preu, DTOExperiencia exp)
        : dataReserva(data), numPlaces(places), preuPagat(preu), experiencia(exp) {
    }

    // --- MÉTODOS DE ACCESO (Getters) ---

    std::string obteData() const { return dataReserva; }          // Devuelve la fecha de la reserva
    int obteNumPlaces() const { return numPlaces; }               // Devuelve el número de plazas reservadas
    float obtePreuPagat() const { return preuPagat; }             // Devuelve el precio final pagado
    const DTOExperiencia& obteExperiencia() const { return experiencia; } // Devuelve el DTO de la experiencia reservada

private:
    std::string dataReserva;
    int numPlaces;
    float preuPagat;
    DTOExperiencia experiencia;
};