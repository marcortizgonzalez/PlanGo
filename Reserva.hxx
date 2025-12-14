#pragma once
#include <string>
#include <memory>
#include <odb/core.hxx>

// --- INCLUDES OBLIGATORIOS PARA ODB ---
// Incluimos las cabeceras completas para que ODB detecte el '#pragma db object'
// de las clases a las que apuntamos. Sin esto, ODB no sabe cómo mapearlas.
#include "Experiencia.hxx"
#include "Usuari.hxx" 

// --- FORWARD DECLARATION ---
// Mantenemos esto para ayudar al compilador de C++ con la dependencia circular
class Usuari;

#pragma db object
class Reserva {
public:
    // Constructor vacío (Requerido por ODB para instanciar objetos desde la BD)
    Reserva() {}

    // Constructor con parámetros (Para crear reservas nuevas desde tu código)
    Reserva(int nPlaces, float preu, std::string data,
        std::shared_ptr<Usuari> u, std::shared_ptr<Experiencia> e)
        : numPlaces(nPlaces), preuPagat(preu), dataReserva(data), usuari(u), experiencia(e) {
    }

    // Getters básicos
    unsigned long getId() const { return id; }
    int getNumPlaces() const { return numPlaces; }
    float getPreuPagat() const { return preuPagat; }
    const std::string& getDataReserva() const { return dataReserva; }

    std::shared_ptr<Usuari> getUsuari() const { return usuari; }
    std::shared_ptr<Experiencia> getExperiencia() const { return experiencia; }

private:
    friend class odb::access;

    // ID autogenerado por la base de datos (AUTO_INCREMENT)
#pragma db id auto
    unsigned long id;

    int numPlaces;
    float preuPagat;
    std::string dataReserva;

    // Relación Muchos-a-Uno: Muchas reservas pertenecen a un Usuario
#pragma db not_null
    std::shared_ptr<Usuari> usuari;

    // Relación Muchos-a-Uno: Muchas reservas son de una Experiencia
#pragma db not_null
    std::shared_ptr<Experiencia> experiencia;
};