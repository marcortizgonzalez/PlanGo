#pragma once
#include <string>

class DTOExperiencia {
public:
    // Constructor actualizado
    DTOExperiencia(std::string t, std::string n, std::string d, std::string c,
        int maxP, float p, std::string det)
        : tipus(t), nom(n), descripcio(d), ciutat(c), maximPlaces(maxP), preu(p), detalls(det) {
    }

    std::string obteTipus() const { return tipus; }
    std::string obteNom() const { return nom; }
    std::string obteDescripcio() const { return descripcio; }
    std::string obteCiutat() const { return ciutat; }
    int obteMaximPlaces() const { return maximPlaces; }
    float obtePreu() const { return preu; } // Nuevo
    std::string obteDetalls() const { return detalls; }

private:
    std::string tipus;
    std::string nom;
    std::string descripcio;
    std::string ciutat;
    int maximPlaces;
    float preu; // Nuevo
    std::string detalls;
};