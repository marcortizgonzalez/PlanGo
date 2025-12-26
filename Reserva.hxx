#pragma once
#include <string>
#include <memory>
#include <odb/core.hxx>
#include "Experiencia.hxx"
#include "Usuari.hxx"

// Clase persistente que representa una reserva realizada por un usuario
#pragma db object
class Reserva {
public:
    // Constructor vacío requerido por ODB
    Reserva() : id(0), numPlaces(0), preuPagat(0.0) {}

    // Constructor: Crea una nueva reserva vinculando usuario, experiencia y detalles
    Reserva(int nPlaces, float preu, std::string d,
        std::shared_ptr<Usuari> u, std::shared_ptr<Experiencia> e)
        : numPlaces(nPlaces), preuPagat(preu), data(d), usuari(u), experiencia(e) {
    }

    // --- MÉTODOS DE ACCESO (Getters) ---

    unsigned long getId() const { return id; }            // Devuelve el ID único (autogenerado)
    int getNumPlaces() const { return numPlaces; }        // Devuelve el número de plazas reservadas
    float getPreuPagat() const { return preuPagat; }      // Devuelve el importe total pagado
    const std::string& getData() const { return data; }   // Devuelve la fecha y hora de la reserva

    // Devuelve el usuario titular de la reserva
    std::shared_ptr<Usuari> getUsuari() const { return usuari; }

    // Devuelve la experiencia reservada
    std::shared_ptr<Experiencia> getExperiencia() const { return experiencia; }

private:
    friend class odb::access;

    // --- ATRIBUTOS PERSISTENTES ---

#pragma db id auto
    unsigned long id;        // Clave primaria autoincremental

    std::string data;        // Fecha de la reserva
    int numPlaces;           // Cantidad de personas/plazas
    float preuPagat;         // Precio final con descuentos aplicados

#pragma db not_null
    std::shared_ptr<Usuari> usuari; // Relación con el Usuario (N:1)

#pragma db not_null
    std::shared_ptr<Experiencia> experiencia; // Relación con la Experiencia (N:1)
};