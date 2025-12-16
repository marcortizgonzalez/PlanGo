#pragma once
#include <string>
#include <vector>

class DTOExperiencia {
public:
    // Constructor completo que usa tu CapaDeDomini
    DTOExperiencia(std::string t, std::string n, std::string d, std::string c,
        int maxP, std::string det, std::vector<std::string> cats)
        : tipus(t), nom(n), descripcio(d), ciutat(c), maximPlaces(maxP), detalls(det), categories(cats) {
    }

    // Getters utilizados en tu CapaDePresentacio antigua
    std::string obteTipus() const { return tipus; }
    std::string obteNom() const { return nom; }
    std::string obteDescripcio() const { return descripcio; }
    std::string obteCiutat() const { return ciutat; }
    int obteMaximPlaces() const { return maximPlaces; }
    std::string obteDetalls() const { return detalls; } // Para mostrar "Hotel: X" o "Durada: Y"
    std::vector<std::string> obteCategories() const { return categories; }

private:
    std::string tipus;
    std::string nom;
    std::string descripcio;
    std::string ciutat;
    int maximPlaces;
    std::string detalls;
    std::vector<std::string> categories;
};