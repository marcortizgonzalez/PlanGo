#pragma once
#include <string>
#include <vector>
#include <memory>
#include <odb/core.hxx>

// CAMBIO 1: No incluimos Reserva.hxx aquí.
// Usamos Forward Declaration para que C++ sepa que existe el nombre.
class Reserva;

#pragma db object
class Usuari {
public:
    Usuari(std::string u, std::string n, std::string mail, std::string pass, std::string dataN)
        : username(u), nom(n), email(mail), contrasenya(pass), dataNaixement(dataN) {
    }

    const std::string& getContrasenya() const { return contrasenya; }
    const std::string& getUsername() const { return username; }

    std::vector<std::shared_ptr<Reserva>>& getReserves() { return reserves; }

private:
    friend class odb::access;
    Usuari() {}

#pragma db id
    std::string username;

    std::string nom;
    std::string email;
    std::string contrasenya;
    std::string dataNaixement;

    // CAMBIO 2: ¡La Clave! Marcamos esto como 'transient'.
    // ODB ignorará esta lista, rompiendo el ciclo y evitando el error.
    // Como ya las cargas manualmente en CapaDeDades, no hace falta que ODB lo haga.
#pragma db transient
    std::vector<std::shared_ptr<Reserva>> reserves;
};