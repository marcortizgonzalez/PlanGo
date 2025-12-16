#pragma once
#include <string>
#include <memory>
#include "DTOExperiencia.hxx" // Asegúrate de tener este include

class DTOReserva {
public:
    DTOReserva(std::string data, int places, float preu, DTOExperiencia exp)
        : dataReserva(data), numPlaces(places), preuPagat(preu), experiencia(exp) {
    }

    std::string obteData() const { return dataReserva; }
    int obteNumPlaces() const { return numPlaces; }
    float obtePreuPagat() const { return preuPagat; }
    const DTOExperiencia& obteExperiencia() const { return experiencia; }

private:
    std::string dataReserva;
    int numPlaces;
    float preuPagat;
    DTOExperiencia experiencia;
};