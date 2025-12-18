#include "CapaDeDomini.hxx"
#include "CapaDeDades.hxx"
#include "PlanGo.hxx" // Necesario para el descuento
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
    if (CapaDeDades::getInstance().obtenirUsuari(sobrenom) != nullptr) {
        throw std::runtime_error("El sobrenom '" + sobrenom + "' ja existeix.");
    }
    if (CapaDeDades::getInstance().obtenirUsuariPerCorreu(correu) != nullptr) {
        throw std::runtime_error("El correu '" + correu + "' ja esta registrat.");
    }

    auto u = std::make_shared<Usuari>(sobrenom, nom, correu, pass, edat);
    CapaDeDades::getInstance().insertaUsuari(u);
}

DTOUsuari CapaDeDomini::consultarUsuari() {
    if (!usuariLoggejat) throw std::runtime_error("No hi ha usuari loggejat.");

    auto usuariRefrescat = CapaDeDades::getInstance().obtenirUsuari(usuariLoggejat->getSobrenom());
    if (usuariRefrescat) usuariLoggejat = usuariRefrescat;

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

    if (!nouCorreu.empty() && nouCorreu != usuariLoggejat->getCorreuElectronic()) {
        auto existe = CapaDeDades::getInstance().obtenirUsuariPerCorreu(nouCorreu);
        if (existe) throw std::runtime_error("El correu electronic ja existeix.");
    }
    if (nouEdat != -1 && nouEdat < 18) throw std::runtime_error("L'usuari es menor d'edat.");

    if (!nouNom.empty()) usuariLoggejat->setNom(nouNom);
    if (!nouCorreu.empty()) usuariLoggejat->setCorreuElectronic(nouCorreu);
    if (nouEdat != -1) usuariLoggejat->setEdat(nouEdat);

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
    std::sort(totes.begin(), totes.end(), [](const std::shared_ptr<Experiencia>& a, const std::shared_ptr<Experiencia>& b) {
        return a->getDataAlta() > b->getDataAlta();
        });

    std::vector<DTOExperiencia> res;
    int limit = std::min((int)totes.size(), 10);
    for (int i = 0; i < limit; ++i) {
        auto e = totes[i];
        res.emplace_back(e->obteTipus(), e->getNom(), e->getDescripcio(), e->getCiutat(),
            e->getMaximPlaces(), e->getPreu(), e->obteDadesEspecifiques());
    }
    return res;
}

// --- BLOQUE B: IMPLEMENTACIÓN RESERVAS ---

void CapaDeDomini::_processarReserva(std::shared_ptr<Experiencia> exp, int numPlaces, std::string data) {
    // 1. Validar plazas
    if (exp->getNumReserves() + numPlaces > exp->getMaximPlaces()) {
        int disponibles = exp->getMaximPlaces() - exp->getNumReserves();
        throw std::runtime_error("No hi ha places suficients. Disponibles: " + std::to_string(disponibles));
    }

    // 2. Calcular precio
    float preuBase = exp->getPreu() * numPlaces;
    float descompte = PlanGo::getInstance().getDescompte();
    float preuFinal = preuBase * ((100.0f - descompte) / 100.0f);

    // 3. Crear y guardar
    auto novaReserva = std::make_shared<Reserva>(numPlaces, preuFinal, data, usuariLoggejat, exp);
    exp->setNumReserves(exp->getNumReserves() + numPlaces);

    CapaDeDades::getInstance().insertaReserva(novaReserva);
    CapaDeDades::getInstance().actualitzaExperiencia(exp);
}

void CapaDeDomini::reservarEscapada(std::string nomEscapada, int numPersones, std::string data) {
    if (!usuariLoggejat) throw std::runtime_error("No hi ha usuari loggejat.");
    auto exp = CapaDeDades::getInstance().obtenirExperiencia(nomEscapada);
    if (!exp) throw std::runtime_error("L'escapada no existeix.");
    if (exp->obteTipus() == "ACTIVITAT") throw std::runtime_error("No es una escapada.");

    _processarReserva(exp, numPersones, data);
}

void CapaDeDomini::reservarActivitat(std::string nomActivitat, int numPersones, std::string data) {
    if (!usuariLoggejat) throw std::runtime_error("No hi ha usuari loggejat.");
    auto exp = CapaDeDades::getInstance().obtenirExperiencia(nomActivitat);
    if (!exp) throw std::runtime_error("L'activitat no existeix.");
    if (exp->obteTipus() != "ACTIVITAT") throw std::runtime_error("No es una activitat.");

    _processarReserva(exp, numPersones, data);
}

std::vector<DTOReserva> CapaDeDomini::consultarReserves() {
    if (!usuariLoggejat) throw std::runtime_error("No hi ha usuari loggejat.");
    auto reserves = CapaDeDades::getInstance().obtenirReservesUsuari(usuariLoggejat);

    std::vector<DTOReserva> result;
    for (const auto& r : reserves) {
        auto e = r->getExperiencia();
        DTOExperiencia dtoExp(e->obteTipus(), e->getNom(), e->getDescripcio(), e->getCiutat(),
            e->getMaximPlaces(), e->getPreu(), e->obteDadesEspecifiques());
        result.emplace_back(r->getData(), r->getNumPlaces(), r->getPreuPagat(), dtoExp);
    }
    return result;
}