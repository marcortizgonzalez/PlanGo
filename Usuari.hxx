#pragma once
#include <string>
#include <vector>
#include <memory>
#include <odb/core.hxx>

class Reserva;

#pragma db object
class Usuari {
public:
    // Constructor adaptado a los nuevos tipos (edat es int)
    Usuari(std::string sobrenom, std::string nom, std::string correu, std::string pass, int edat)
        : sobrenom(sobrenom), nom(nom), correuElectronic(correu), contrasenya(pass), edat(edat) {
    }

    // --- GETTERS ---
    const std::string& getSobrenom() const { return sobrenom; } // ID
    const std::string& getNom() const { return nom; }
    const std::string& getCorreuElectronic() const { return correuElectronic; }
    const std::string& getContrasenya() const { return contrasenya; }
    int getEdat() const { return edat; }
    // Getter de compatibilidad por si alguna parte antigua llama a getUsername
    const std::string& getUsername() const { return sobrenom; }

    // --- SETTERS ---
    void setNom(const std::string& n) { nom = n; }
    void setCorreuElectronic(const std::string& c) { correuElectronic = c; }
    void setContrasenya(const std::string& c) { contrasenya = c; }
    void setEdat(int e) { edat = e; }

    std::vector<std::shared_ptr<Reserva>>& getReserves() { return reserves; }

private:
    friend class odb::access;
    Usuari() : edat(0) {}

#pragma db id
    std::string sobrenom; // ID (antes username)

    std::string nom;
    std::string correuElectronic; // Antes email
    std::string contrasenya;
    int edat; // Antes dataNaixement (string) -> Ahora int

#pragma db transient
    std::vector<std::shared_ptr<Reserva>> reserves;
};