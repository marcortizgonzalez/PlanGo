#include "CapaDeDomini.hxx"
#include "CapaDeDades.hxx"
#include <algorithm>
#include <stdexcept>

CapaDeDomini& CapaDeDomini::getInstance() {
    static CapaDeDomini instance;
    return instance;
}

void CapaDeDomini::iniciarSessio(std::string username, std::string contrasenya) {
    auto usuari = CapaDeDades::getInstance().obtenirUsuari(username);
    if (!usuari) throw std::runtime_error("L'usuari no existeix.");
    if (usuari->getContrasenya() != contrasenya) throw std::runtime_error("Contrasenya incorrecta.");
    usuariLoggejat = usuari;
}

void CapaDeDomini::registrarUsuari(std::string nom, std::string sobrenom, std::string correu,
    std::string pass, int edat) {
    // Validar ID único
    if (CapaDeDades::getInstance().obtenirUsuari(sobrenom) != nullptr) {
        throw std::runtime_error("El sobrenom '" + sobrenom + "' ja existeix.");
    }
    // Validar Correo único
    if (CapaDeDades::getInstance().obtenirUsuariPerCorreu(correu) != nullptr) {
        throw std::runtime_error("El correu '" + correu + "' ja esta registrat.");
    }

    auto u = std::make_shared<Usuari>(sobrenom, nom, correu, pass, edat);
    CapaDeDades::getInstance().insertaUsuari(u);
}

DTOUsuari CapaDeDomini::consultarUsuari() {
    if (!usuariLoggejat) throw std::runtime_error("No hi ha usuari loggejat.");

    // REQUISITO: "Anar a la base de dades a buscar la informació" para confirmar cambios
    auto usuariRefrescat = CapaDeDades::getInstance().obtenirUsuari(usuariLoggejat->getSobrenom());
    if (usuariRefrescat) {
        usuariLoggejat = usuariRefrescat; // Actualizamos memoria con lo real de BD
    }

    auto reserves = CapaDeDades::getInstance().obtenirReservesUsuari(usuariLoggejat);

    return DTOUsuari(
        usuariLoggejat->getNom(),
        usuariLoggejat->getSobrenom(),
        usuariLoggejat->getCorreuElectronic(),
        usuariLoggejat->getEdat(),
        (int)reserves.size()
    );
}

void CapaDeDomini::modificarUsuari(std::string nouNom, std::string nouCorreu, int nouEdat) {
    if (!usuariLoggejat) throw std::runtime_error("No hi ha usuari loggejat.");

    // 1. Validar Correo (Escenario Alternativo)
    if (!nouCorreu.empty() && nouCorreu != usuariLoggejat->getCorreuElectronic()) {
        auto existe = CapaDeDades::getInstance().obtenirUsuariPerCorreu(nouCorreu);
        if (existe) {
            throw std::runtime_error("El correu electronic ja existeix.");
        }
    }

    // 2. Validar Edat (Escenario Alternativo)
    if (nouEdat != -1) { // Si se ha introducido un valor
        if (nouEdat < 18) {
            throw std::runtime_error("L'usuari es menor d'edat.");
        }
    }

    // Aplicar cambios si pasan las validaciones
    if (!nouNom.empty()) usuariLoggejat->setNom(nouNom);
    if (!nouCorreu.empty()) usuariLoggejat->setCorreuElectronic(nouCorreu);
    if (nouEdat != -1) usuariLoggejat->setEdat(nouEdat);

    // Guardar en BD
    CapaDeDades::getInstance().modificaUsuari(usuariLoggejat);
}

void CapaDeDomini::esborrarUsuari(std::string contrasenya) {
    if (!usuariLoggejat) throw std::runtime_error("No hi ha usuari.");
    if (usuariLoggejat->getContrasenya() != contrasenya) throw std::runtime_error("Contrasenya incorrecta.");

    auto reserves = CapaDeDades::getInstance().obtenirReservesUsuari(usuariLoggejat);
    for (auto& r : reserves) CapaDeDades::getInstance().esborrarReserva(r);

    CapaDeDades::getInstance().esborrarUsuari(usuariLoggejat);
    usuariLoggejat = nullptr;
}

std::vector<DTOExperiencia> CapaDeDomini::consultarNovetats() {
    auto totes = CapaDeDades::getInstance().totesExperiencies();

    // Ordenar por dataAlta
    std::sort(totes.begin(), totes.end(), [](const std::shared_ptr<Experiencia>& a, const std::shared_ptr<Experiencia>& b) {
        return a->getDataAlta() > b->getDataAlta();
        });

    std::vector<DTOExperiencia> res;
    int limit = std::min((int)totes.size(), 10);

    for (int i = 0; i < limit; ++i) {
        auto e = totes[i];
        res.emplace_back(
            e->obteTipus(),
            e->getNom(),
            e->getDescripcio(),
            e->getCiutat(),
            e->getMaximPlaces(),
            e->getPreu(), // AHORA SACAMOS EL PRECIO DE LA BASE
            e->obteDadesEspecifiques()
        );
    }
    return res;
}