#pragma once
#include <string>
#include <vector>
#include <memory>
#include <odb/core.hxx>
// Incluimos Reserva.hxx aquí porque necesitamos el tipo completo para el vector
#include "Reserva.hxx" 

#pragma db object
class Usuari {
public:
    Usuari(std::string u, std::string n, std::string mail, std::string pass, std::string dataN)
        : username(u), nom(n), email(mail), contrasenya(pass), dataNaixement(dataN) {
    }

    const std::string& getContrasenya() const { return contrasenya; }
    const std::string& getUsername() const { return username; }

    // Acceso a las reservas del usuario
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

    // Relación 1:N inversa (Reserva tiene un puntero 'usuari')
    // value_not_null asegura punteros válidos
    // cascade(delete) es opcional, pero útil: si borras usuario, ODB puede borrar reservas (aunque en diseño dijimos de hacerlo manual)
#pragma db value_not_null inverse(usuari) 
    std::vector<std::shared_ptr<Reserva>> reserves;
};