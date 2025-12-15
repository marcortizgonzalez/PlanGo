#include "CapaDeDomini.hxx"
#include "CapaDeDades.hxx"
#include <algorithm> // Para sort
#include <stdexcept>

CapaDeDomini& CapaDeDomini::getInstance() {
    static CapaDeDomini instance;
    return instance;
}

void CapaDeDomini::iniciarSessio(std::string username) {
    usuariLoggejat = CapaDeDades::getInstance().obtenirUsuari(username);
}

// CASO DE USO 1: Consultar Novetats
std::vector<DTOExperiencia> CapaDeDomini::consultarNovetats() {
    // 1. Obtener todas
    auto totes = CapaDeDades::getInstance().totesExperiencies();

    // 2. Ordenar por fecha (descendente). Asumimos formato string YYYY-MM-DD
    std::sort(totes.begin(), totes.end(), [](const std::shared_ptr<Experiencia>& a, const std::shared_ptr<Experiencia>& b) {
        return a->getDataAlta() > b->getDataAlta();
        });

    // 3. Seleccionar top 10
    std::vector<DTOExperiencia> resultat;
    int limit = std::min((int)totes.size(), 10);

    for (int i = 0; i < limit; ++i) {
        auto e = totes[i];

        // Convertim les categories (objectes) a strings per al DTO
        std::vector<std::string> nomsCategories;
        for (const auto& cat : e->getCategories()) {
            nomsCategories.push_back(cat->getNom());
        }

        // Creem el DTO amb TOTS els camps
        resultat.emplace_back(
            e->obteTipus(),
            e->getNom(),
            e->getDescripcio(),
            e->getCiutat(),
            e->getMaximPlaces(),
            e->obteDadesEspecifiques(),
            nomsCategories
        );
    }
    return resultat;
}

// CASO DE USO 2: Esborrar Usuari
void CapaDeDomini::esborrarUsuari(std::string contrasenya) {
    if (!usuariLoggejat) throw std::runtime_error("No hi ha usuari loggejat.");

    if (usuariLoggejat->getContrasenya() != contrasenya) {
        throw std::runtime_error("Contrasenya incorrecta.");
    }

    // Borrar reservas (necesitamos cargar la relación primero o confiar en ODB)
    // Para simplificar, asumimos que 'getReserves' tiene los datos
    // Nota: En ODB real, si la relación es lazy, necesitamos una transacción activa para leer.
    // Aquí confiamos en que CapaDeDades gestione las transacciones internas de borrado.

    // Iteramos sobre una copia porque vamos a borrar
    auto reserves = usuariLoggejat->getReserves();
    for (auto& r : reserves) {
        CapaDeDades::getInstance().esborrarReserva(r);
    }

    CapaDeDades::getInstance().esborrarUsuari(usuariLoggejat);
    usuariLoggejat = nullptr;
}