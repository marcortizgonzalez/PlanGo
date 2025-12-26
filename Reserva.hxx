#pragma once
#include <string>
#include <memory>
#include <odb/core.hxx>
#include "Experiencia.hxx"
#include "Usuari.hxx"

// Clase persistente que representa una reserva
#pragma db object
class Reserva {
public:
    // Constructor vacío para ODB
    Reserva() : id(0), numPlaces(0), preuPagat(0.0) {}

    // Constructor principal: Vincula datos, usuario y experiencia
    Reserva(int nPlaces, float preu, std::string d,
        std::shared_ptr<Usuari> u, std::shared_ptr<Experiencia> e)
        : id(0), numPlaces(nPlaces), preuPagat(preu), data(d), usuari(u), experiencia(e) {
    }

    // --- GETTERS ---
    unsigned long getId() const { return id; }
    int getNumPlaces() const { return numPlaces; }
    float getPreuPagat() const { return preuPagat; }
    const std::string& getData() const { return data; } // Fecha reserva

    std::shared_ptr<Usuari> getUsuari() const { return usuari; }
    std::shared_ptr<Experiencia> getExperiencia() const { return experiencia; }

private:
    friend class odb::access;

    // --- ATRIBUTOS PERSISTENTES ---

#pragma db id auto
    unsigned long id; // Autoincremental

    std::string data;
    int numPlaces;
    float preuPagat;

#pragma db not_null
    std::shared_ptr<Usuari> usuari;

#pragma db not_null
    std::shared_ptr<Experiencia> experiencia;
};