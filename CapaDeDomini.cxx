#include "CapaDeDomini.hxx"
#include "CapaDeDades.hxx"
#include <algorithm> // Para std::sort y std::min
#include <stdexcept> // Para std::runtime_error

// --- Implementación del Singleton ---
CapaDeDomini& CapaDeDomini::getInstance() {
    static CapaDeDomini instance;
    return instance;
}

// --- Gestión de Sesión ---

// CORRECCIÓN: Ahora acepta 2 parámetros
void CapaDeDomini::iniciarSessio(std::string username, std::string contrasenya) {
    // 1. Buscamos el usuario en la BD
    auto usuari = CapaDeDades::getInstance().obtenirUsuari(username);

    // 2. Si no existe, error
    if (!usuari) {
        throw std::runtime_error("L'usuari no existeix.");
    }

    // 3. Comprobamos contraseña
    if (usuari->getContrasenya() != contrasenya) {
        throw std::runtime_error("Contrasenya incorrecta.");
    }

    // 4. Guardamos sesión
    usuariLoggejat = usuari;
}

// --- CASO DE USO: Consultar Novetats ---
std::vector<DTOExperiencia> CapaDeDomini::consultarNovetats() {
    auto totes = CapaDeDades::getInstance().totesExperiencies();

    // Ordenamos por fecha descendente
    std::sort(totes.begin(), totes.end(),
        [](const std::shared_ptr<Experiencia>& a, const std::shared_ptr<Experiencia>& b) {
            return a->getDataAlta() > b->getDataAlta();
        });

    std::vector<DTOExperiencia> resultat;
    int limit = std::min((int)totes.size(), 10);

    for (int i = 0; i < limit; ++i) {
        auto e = totes[i];
        std::vector<std::string> nomsCategories;
        for (const auto& cat : e->getCategories()) {
            nomsCategories.push_back(cat->getNom());
        }

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

// --- BLOQUE A: GESTIÓN DE USUARIOS ---

void CapaDeDomini::registrarUsuari(std::string nom, std::string user, std::string mail,
    std::string pass, std::string dataN) {
    // Validar si ya existe
    auto existe = CapaDeDades::getInstance().obtenirUsuari(user);
    if (existe != nullptr) {
        throw std::runtime_error("L'usuari '" + user + "' ja existeix.");
    }

    // Crear y guardar
    auto nouUsuari = std::make_shared<Usuari>(user, nom, mail, pass, dataN);
    CapaDeDades::getInstance().insertaUsuari(nouUsuari);
}

DTOUsuari CapaDeDomini::consultarUsuari() {
    if (!usuariLoggejat) throw std::runtime_error("No hi ha usuari loggejat.");

    int edat = 0;
    try {
        if (usuariLoggejat->getDataNaixement().size() >= 4) {
            int anyNaix = std::stoi(usuariLoggejat->getDataNaixement().substr(0, 4));
            edat = 2024 - anyNaix;
        }
    }
    catch (...) { edat = 0; }

    // Obtenemos reservas manualmente para evitar error ODB
    auto reserves = CapaDeDades::getInstance().obtenirReservesUsuari(usuariLoggejat);

    return DTOUsuari(
        usuariLoggejat->getNom(),
        usuariLoggejat->getUsername(),
        usuariLoggejat->getEmail(),
        edat,
        (int)reserves.size()
    );
}

void CapaDeDomini::modificarUsuari(std::string nouNom, std::string nouMail,
    std::string novaPass, std::string novaData) {
    if (!usuariLoggejat) throw std::runtime_error("No hi ha usuari loggejat.");

    // Actualizamos si el campo no está vacío
    if (!nouNom.empty()) usuariLoggejat->setNom(nouNom);
    if (!nouMail.empty()) usuariLoggejat->setEmail(nouMail);
    if (!novaPass.empty()) usuariLoggejat->setContrasenya(novaPass);
    if (!novaData.empty()) usuariLoggejat->setDataNaixement(novaData);

    CapaDeDades::getInstance().modificaUsuari(usuariLoggejat);
}

void CapaDeDomini::esborrarUsuari(std::string contrasenya) {
    if (!usuariLoggejat) throw std::runtime_error("No hi ha cap usuari loggejat.");

    if (usuariLoggejat->getContrasenya() != contrasenya) {
        throw std::runtime_error("Contrasenya incorrecta.");
    }

    // Obtenemos las reservas manualmente
    auto reserves = CapaDeDades::getInstance().obtenirReservesUsuari(usuariLoggejat);

    for (auto& r : reserves) {
        CapaDeDades::getInstance().esborrarReserva(r);
    }

    CapaDeDades::getInstance().esborrarUsuari(usuariLoggejat);
    usuariLoggejat = nullptr;
}