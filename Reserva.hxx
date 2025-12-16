#pragma once
#include <string>
#include <memory>
#include <odb/core.hxx>
#include "Experiencia.hxx"

// CAMBIO 3: Aquí SÍ incluimos Usuari.hxx con total normalidad.
#include "Usuari.hxx"

#pragma db object
class Reserva {
public:
    Reserva() : id(0), numPlaces(0), preuPagat(0.0) {}

    Reserva(int nPlaces, float preu, std::string data,
        std::shared_ptr<Usuari> u, std::shared_ptr<Experiencia> e)
        : numPlaces(nPlaces), preuPagat(preu), dataReserva(data), usuari(u), experiencia(e) {
    }

    unsigned long getId() const { return id; }
    int getNumPlaces() const { return numPlaces; }
    float getPreuPagat() const { return preuPagat; }
    const std::string& getDataReserva() const { return dataReserva; }

    std::shared_ptr<Usuari> getUsuari() const { return usuari; }
    std::shared_ptr<Experiencia> getExperiencia() const { return experiencia; }

private:
    friend class odb::access;

#pragma db id auto
    unsigned long id;

    int numPlaces;
    float preuPagat;
    std::string dataReserva;

#pragma db not_null
    std::shared_ptr<Usuari> usuari;

#pragma db not_null
    std::shared_ptr<Experiencia> experiencia;
};