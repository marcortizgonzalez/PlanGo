#pragma once
#include <string>

// Clase DTO (Data Transfer Object) para enviar datos de experiencias desde Dominio a Presentación
// Evita exponer la entidad 'Experiencia' completa a la interfaz gráfica.
class DTOExperiencia {
public:
    // Inicializa todos los datos necesarios para la UI
    DTOExperiencia(std::string t, std::string n, std::string d, std::string c,
        int maxP, float p, std::string det)
        : tipus(t), nom(n), descripcio(d), ciutat(c), maximPlaces(maxP), preu(p), detalls(det) {
    }

    // --- MÉTODOS DE ACCESO (Getters) ---

    std::string obteTipus() const { return tipus; }       // Retorna "ACTIVITAT" o "ESCAPADA"
    std::string obteNom() const { return nom; }           // Nombre de la experiencia
    std::string obteDescripcio() const { return descripcio; } // Descripción corta
    std::string obteCiutat() const { return ciutat; }     // Ubicación
    int obteMaximPlaces() const { return maximPlaces; }   // Aforo máximo
    float obtePreu() const { return preu; }               // Precio base (por persona o total según tipo)
    std::string obteDetalls() const { return detalls; }   // Info específica (Duración o Hotel/Noches)

private:
    std::string tipus;
    std::string nom;
    std::string descripcio;
    std::string ciutat;
    int maximPlaces;
    float preu;
    std::string detalls;
};