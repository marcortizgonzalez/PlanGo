#pragma once
#include <string>
#include <vector>
#include <memory>
#include <odb/core.hxx>

// Forward declaration
class Reserva;

#pragma db object
class Usuari {
public:
    // Constructor completo
    Usuari(std::string u, std::string n, std::string mail, std::string pass, std::string dataN)
        : username(u), nom(n), email(mail), contrasenya(pass), dataNaixement(dataN) {
    }

    // --- GETTERS (Lectura) ---
    const std::string& getUsername() const { return username; }
    // Alias para compatibilidad con tu código de Dominio:
    const std::string& getSobrenom() const { return username; }

    const std::string& getNom() const { return nom; }
    const std::string& getEmail() const { return email; }
    const std::string& getContrasenya() const { return contrasenya; }
    const std::string& getDataNaixement() const { return dataNaixement; }

    // --- SETTERS (Escritura - Para ModificarUsuari) ---
    void setNom(const std::string& n) { nom = n; }
    void setEmail(const std::string& e) { email = e; }
    void setContrasenya(const std::string& c) { contrasenya = c; }
    void setDataNaixement(const std::string& d) { dataNaixement = d; }

    // Acceso al vector (aunque ODB no lo guarde automáticamente)
    std::vector<std::shared_ptr<Reserva>>& getReserves() { return reserves; }

private:
    friend class odb::access;
    Usuari() {} // Constructor vacío para ODB

#pragma db id
    std::string username;

    std::string nom;
    std::string email;
    std::string contrasenya;
    std::string dataNaixement;

    // IMPORTANTE: 'transient' para que ODB ignore esta lista y no cree bucles infinitos.
    // La cargaremos manualmente en CapaDeDades.
#pragma db transient
    std::vector<std::shared_ptr<Reserva>> reserves;
};