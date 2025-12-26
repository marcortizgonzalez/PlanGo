#pragma once
#include <string>

// Clase DTO para transferir datos de experiencias a la capa de presentación
class DTOExperiencia {
public:
    // Constructor: Inicializa el objeto con todos los datos de la experiencia
    DTOExperiencia(std::string t, std::string n, std::string d, std::string c,
        int maxP, float p, std::string det, int nRes)
        : tipus(t), nom(n), descripcio(d), ciutat(c), maximPlaces(maxP), preu(p), detalls(det), numReserves(nRes) {
    }

    // --- MÉTODOS DE ACCESO (Getters) ---

    std::string obteTipus() const { return tipus; }       // Devuelve el tipo (ESCAPADA o ACTIVITAT)
    std::string obteNom() const { return nom; }           // Devuelve el nombre de la experiencia
    std::string obteDescripcio() const { return descripcio; } // Devuelve la descripción
    std::string obteCiutat() const { return ciutat; }     // Devuelve la ciudad
    int obteMaximPlaces() const { return maximPlaces; }   // Devuelve el aforo máximo
    float obtePreu() const { return preu; }               // Devuelve el precio
    std::string obteDetalls() const { return detalls; }   // Devuelve datos específicos (Hotel/Noches o Duración)
	int obteNumReserves() const { return numReserves; }   // Devuelve el número de reservas

private:
    std::string tipus;
    std::string nom;
    std::string descripcio;
    std::string ciutat;
    int maximPlaces;
    float preu;
    std::string detalls;
    int numReserves;
};