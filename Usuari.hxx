#pragma once
#include <string>
#include <vector>
#include <memory>
#include <odb/core.hxx>

class Reserva; // Declaración anticipada para evitar dependencias circulares

// Clase persistente que representa a un usuario registrado en el sistema
#pragma db object
class Usuari {
public:
    // Constructor: Inicializa un nuevo usuario con sus datos personales
    Usuari(std::string sobrenom, std::string nom, std::string correu, std::string pass, int edat)
        : sobrenom(sobrenom), nom(nom), correuElectronic(correu), contrasenya(pass), edat(edat) {
    }

    // --- MÉTODOS DE ACCESO (Getters) ---

    const std::string& getSobrenom() const { return sobrenom; } // Devuelve el sobrenombre (ID único)
    const std::string& getNom() const { return nom; }           // Devuelve el nombre real
    const std::string& getCorreuElectronic() const { return correuElectronic; } // Devuelve el email
    const std::string& getContrasenya() const { return contrasenya; } // Devuelve la contraseña
    int getEdat() const { return edat; }                        // Devuelve la edad

    // Alias de compatibilidad (Sobrenom = Username)
    const std::string& getUsername() const { return sobrenom; }

    // --- MÉTODOS DE MODIFICACIÓN (Setters) ---

    void setNom(const std::string& n) { nom = n; }
    void setCorreuElectronic(const std::string& c) { correuElectronic = c; }
    void setContrasenya(const std::string& c) { contrasenya = c; }
    void setEdat(int e) { edat = e; }

    // Devuelve la lista de reservas asociadas a este usuario (Relación 1:N)
    std::vector<std::shared_ptr<Reserva>>& getReserves() { return reserves; }

private:
    friend class odb::access;
    Usuari() : edat(0) {} // Constructor vacío para ODB

    // --- ATRIBUTOS PERSISTENTES ---

#pragma db id
    std::string sobrenom;    // Clave primaria (Nickname)

    std::string nom;

#pragma db index unique
    std::string correuElectronic; // Índice único para evitar emails duplicados

    std::string contrasenya;
    int edat;

#pragma db inverse(usuari)
    std::vector<std::shared_ptr<Reserva>> reserves; // Relación inversa con Reserva
};