#pragma once
#include <string>
#include <vector>
#include <memory>
#include <odb/core.hxx>

class Reserva; // Declaración anticipada

// Clase persistente que representa a un usuario
#pragma db object
class Usuari {
public:
    // Constructor: Crea usuario con sus datos personales
    Usuari(std::string sobrenom, std::string nom, std::string correu, std::string pass, int edat)
        : sobrenom(sobrenom), nom(nom), correuElectronic(correu), contrasenya(pass), edat(edat) {
    }

    // --- GETTERS ---
    const std::string& getSobrenom() const { return sobrenom; } // ID único
    const std::string& getNom() const { return nom; }
    const std::string& getCorreuElectronic() const { return correuElectronic; }
    const std::string& getContrasenya() const { return contrasenya; }
    int getEdat() const { return edat; }
    const std::string& getUsername() const { return sobrenom; } // Alias

    // --- SETTERS ---
    void setNom(const std::string& n) { nom = n; }
    void setCorreuElectronic(const std::string& c) { correuElectronic = c; }
    void setContrasenya(const std::string& c) { contrasenya = c; }
    void setEdat(int e) { edat = e; }

    // Devuelve el vector de reservas (se llena manualmente en CapaDeDades)
    std::vector<std::shared_ptr<Reserva>>& getReserves() { return reserves; }

private:
    friend class odb::access;
    Usuari() : edat(0) {} // Constructor vacío para ODB

    // --- ATRIBUTOS PERSISTENTES ---

#pragma db id
    std::string sobrenom;

    std::string nom;

#pragma db index unique
    std::string correuElectronic;

    std::string contrasenya;
    int edat;

    // 'transient': ODB ignora esto para evitar ciclos, lo gestionamos por código
#pragma db transient
    std::vector<std::shared_ptr<Reserva>> reserves;
};