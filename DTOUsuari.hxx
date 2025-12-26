#pragma once
#include <string>

// Clase DTO para transferir datos de usuario a la capa de presentación
class DTOUsuari {
public:
    // Constructor: Inicializa el objeto con los datos personales y estadísticas del usuario
    DTOUsuari(std::string n, std::string s, std::string c, int e, int nRes)
        : nom(n), sobrenom(s), correu(c), edat(e), numReserves(nRes) {
    }

    // --- MÉTODOS DE ACCESO (Getters) ---

    std::string obteNom() const { return nom; }           // Devuelve el nombre completo
    std::string obteSobrenom() const { return sobrenom; } // Devuelve el apodo (ID)
    std::string obteCorreu() const { return correu; }     // Devuelve el correo electrónico
    int obteEdat() const { return edat; }                 // Devuelve la edad
    int obteNumReserves() const { return numReserves; }   // Devuelve el total de reservas realizadas

private:
    std::string nom;
    std::string sobrenom;
    std::string correu;
    int edat;
    int numReserves;
};