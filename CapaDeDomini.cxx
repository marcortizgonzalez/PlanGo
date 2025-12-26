#define _CRT_SECURE_NO_WARNINGS
#include "CapaDeDomini.hxx"
#include "CapaDeDades.hxx"
#include "PlanGo.hxx"
#include <algorithm>
#include <stdexcept>
#include <ctime>
#include <iomanip>
#include <sstream>

// Singleton
CapaDeDomini& CapaDeDomini::getInstance() {
    static CapaDeDomini instance;
    return instance;
}

// --- GESTIÓ SESSIÓ I USUARIS ---

// [3.1.1] Inici Sessió
void CapaDeDomini::iniciSessio(std::string username, std::string contrasenya) {
    auto usuari = CapaDeDades::getInstance().obtenirUsuari(username);
    if (!usuari) throw std::runtime_error("L'usuari no existeix.");
    if (usuari->getContrasenya() != contrasenya) throw std::runtime_error("Contrasenya incorrecta.");
    usuariLoggejat = usuari;
}

// [3.1.2] Tanca Sessió
void CapaDeDomini::tancaSessio() {
    usuariLoggejat = nullptr;
}

// [3.1.3] Registra Usuari
void CapaDeDomini::registraUsuari(std::string nom, std::string sobrenom, std::string correu,
    std::string pass, int edat) {

    // Validar unicidad de Nickname [RIT]
    if (CapaDeDades::getInstance().obtenirUsuari(sobrenom) != nullptr) {
        throw std::runtime_error("El sobrenom '" + sobrenom + "' ja existeix.");
    }
    // Validar unicidad de Correo [RIT2]
    if (CapaDeDades::getInstance().obtenirUsuariPerCorreu(correu) != nullptr) {
        throw std::runtime_error("El correu '" + correu + "' ja esta registrat.");
    }
    // Validar Mayoría de Edad [RIT1]
    if (edat < 18) {
        throw std::runtime_error("L'usuari ha de ser major d'edat.");
    }

    auto u = std::make_shared<Usuari>(sobrenom, nom, correu, pass, edat);
    CapaDeDades::getInstance().insertaUsuari(u);
}

// [3.1.4] Consulta Usuari
DTOUsuari CapaDeDomini::consultaUsuari() {
    if (!usuariLoggejat) throw std::runtime_error("No hi ha usuari loggejat.");
    // Refrescamos datos de la BD
    auto usuariRefrescat = CapaDeDades::getInstance().obtenirUsuari(usuariLoggejat->getSobrenom());
    if (usuariRefrescat) usuariLoggejat = usuariRefrescat;

    auto reserves = CapaDeDades::getInstance().obtenirReservesUsuari(usuariLoggejat);
    return DTOUsuari(usuariLoggejat->getNom(), usuariLoggejat->getSobrenom(),
        usuariLoggejat->getCorreuElectronic(), usuariLoggejat->getEdat(), (int)reserves.size());
}

// [3.1.5] Modifica Usuari
void CapaDeDomini::modificaUsuari(std::string nouNom, std::string nouCorreu, int nouEdat) {
    if (!usuariLoggejat) throw std::runtime_error("No hi ha usuari loggejat.");

    // Validar correo nuevo si cambia
    if (!nouCorreu.empty() && nouCorreu != usuariLoggejat->getCorreuElectronic()) {
        auto existe = CapaDeDades::getInstance().obtenirUsuariPerCorreu(nouCorreu);
        if (existe) throw std::runtime_error("El correu electronic ja existeix.");
    }
    // Validar edad
    if (nouEdat != -1 && nouEdat < 18) throw std::runtime_error("No es poden registrar usuaris menors");

    // Aplicar cambios
    if (!nouNom.empty()) usuariLoggejat->setNom(nouNom);
    if (!nouCorreu.empty()) usuariLoggejat->setCorreuElectronic(nouCorreu);
    if (nouEdat != -1) usuariLoggejat->setEdat(nouEdat);

    CapaDeDades::getInstance().modificaUsuari(usuariLoggejat);
}

// [3.1.6] Esborra Usuari
void CapaDeDomini::esborraUsuari(std::string contrasenya) {
    if (!usuariLoggejat) throw std::runtime_error("No hi ha usuari.");
    if (usuariLoggejat->getContrasenya() != contrasenya) throw std::runtime_error("Contrasenya incorrecta.");

    auto reserves = CapaDeDades::getInstance().obtenirReservesUsuari(usuariLoggejat);

    // Liberamos plazas (sin borrar reservas explícitamente, confiamos en CASCADE para el borrado)
    for (auto& r : reserves) {
        auto exp = r->getExperiencia();
        int placesAlliberades = r->getNumPlaces();
        int actuals = exp->getNumReserves();
        int novesReserves = (actuals >= placesAlliberades) ? (actuals - placesAlliberades) : 0;

        exp->setNumReserves(novesReserves);
        CapaDeDades::getInstance().actualitzaExperiencia(exp);
    }

    // Borrado del usuario (dispara ON DELETE CASCADE para reservas en BD)
    CapaDeDades::getInstance().esborrarUsuari(usuariLoggejat);
    usuariLoggejat = nullptr;
}

// --- RESERVAS ---

std::string CapaDeDomini::_obteDataActual() {
    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);
    std::stringstream ss;
    ss << (now->tm_year + 1900) << "-"
        << std::setw(2) << std::setfill('0') << (now->tm_mon + 1) << "-"
        << std::setw(2) << std::setfill('0') << now->tm_mday << " "
        << std::setw(2) << std::setfill('0') << now->tm_hour << ":"
        << std::setw(2) << std::setfill('0') << now->tm_min;
    return ss.str();
}

// [3.2.1] Consulta Escapada
DTOExperiencia CapaDeDomini::consultaEscapada(std::string nom) {
    if (!usuariLoggejat) throw std::runtime_error("No hi ha usuari loggejat.");

    auto exp = CapaDeDades::getInstance().obtenirExperiencia(nom);

    // Validamos existencia y tipo
    if (!exp) throw std::runtime_error("L'escapada no existeix.");
    if (exp->obteTipus() == "ACTIVITAT") throw std::runtime_error("L'escapada no existeix."); // O "No es una escapada"

    return DTOExperiencia(exp->obteTipus(), exp->getNom(), exp->getDescripcio(),
        exp->getCiutat(), exp->getMaximPlaces(), exp->getPreu(),
        exp->obteDadesEspecifiques());
}

// [3.2.2] Consulta Activitat
DTOExperiencia CapaDeDomini::consultaActivitat(std::string nom) {
    if (!usuariLoggejat) throw std::runtime_error("No hi ha usuari loggejat.");

    auto exp = CapaDeDades::getInstance().obtenirExperiencia(nom);

    // Validamos existencia y tipo
    if (!exp) throw std::runtime_error("L'activitat no existeix.");
    if (exp->obteTipus() != "ACTIVITAT") throw std::runtime_error("L'activitat no existeix.");

    return DTOExperiencia(exp->obteTipus(), exp->getNom(), exp->getDescripcio(),
        exp->getCiutat(), exp->getMaximPlaces(), exp->getPreu(),
        exp->obteDadesEspecifiques());
}

// Calcula precio (Escapada = Total, Activitat = Por persona)
float CapaDeDomini::calculaPreu(std::string nom, int persones) {
    auto exp = CapaDeDades::getInstance().obtenirExperiencia(nom);
    if (!exp) throw std::runtime_error("L'experiencia no existeix.");

    float preuBase = 0.0f;
    if (exp->obteTipus() == "ACTIVITAT") {
        preuBase = exp->getPreu() * persones;
    }
    else {
        preuBase = exp->getPreu();
    }

    float descompte = PlanGo::getInstance().getDescompte();
    return preuBase * ((100.0f - descompte) / 100.0f);
}

// Lógica interna reserva
void CapaDeDomini::_processarReserva(std::shared_ptr<Experiencia> exp, int numPlaces) {
    int disponibles = exp->getMaximPlaces() - exp->getNumReserves();

    if (numPlaces > disponibles) {
        std::string error = "No hi ha places suficients. Sol·licitades: " + std::to_string(numPlaces) +
            ", Disponibles: " + std::to_string(disponibles);
        throw std::runtime_error(error);
    }

    float preuBase = 0.0f;
    if (exp->obteTipus() == "ACTIVITAT") preuBase = exp->getPreu() * numPlaces;
    else preuBase = exp->getPreu();

    float descompte = PlanGo::getInstance().getDescompte();
    float preuFinal = preuBase * ((100.0f - descompte) / 100.0f);

    std::string dataAra = _obteDataActual();
    auto novaReserva = std::make_shared<Reserva>(numPlaces, preuFinal, dataAra, usuariLoggejat, exp);

    exp->setNumReserves(exp->getNumReserves() + numPlaces);

    CapaDeDades::getInstance().insertaReserva(novaReserva);
    CapaDeDades::getInstance().actualitzaExperiencia(exp);
}
// [3.2.1] Reserva Escapada
void CapaDeDomini::reservaEscapada(std::string nomEscapada) {
    if (!usuariLoggejat) throw std::runtime_error("No hi ha usuari loggejat.");

    auto exp = CapaDeDades::getInstance().obtenirExperiencia(nomEscapada);
    if (!exp) throw std::runtime_error("L'escapada no existeix.");
    if (exp->obteTipus() == "ACTIVITAT") throw std::runtime_error("No es una escapada.");

    int placesAReservar = exp->getMaximPlaces();
    _processarReserva(exp, placesAReservar);
}

// [3.2.2] Reserva Activitat
void CapaDeDomini::reservaActivitat(std::string nomActivitat, int numPersones) {
    if (!usuariLoggejat) throw std::runtime_error("No hi ha usuari loggejat.");

    auto exp = CapaDeDades::getInstance().obtenirExperiencia(nomActivitat);
    if (!exp) throw std::runtime_error("L'activitat no existeix.");
    if (exp->obteTipus() != "ACTIVITAT") throw std::runtime_error("No es una activitat.");

    if (numPersones < 1) throw std::runtime_error("El nombre de persones ha de ser major que 0.");

    _processarReserva(exp, numPersones);
}

// [3.2.3] Consulta Reserves
std::vector<DTOReserva> CapaDeDomini::consultaReserves() {
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

// --- CONSULTES ---

// [3.3.2] Consultar Novetats
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

// [3.3.1] Consulta Experiencies (por ciudad)
std::vector<DTOExperiencia> CapaDeDomini::consultaExperiencies(std::string ciutat, int places) {
    if (places < 1) throw std::runtime_error("El nombre de persones ha de ser major que 0.");
    auto llista = CapaDeDades::getInstance().cercarExperiencies(ciutat, places);
    std::vector<DTOExperiencia> res;
    for (const auto& e : llista) {
        res.emplace_back(e->obteTipus(), e->getNom(), e->getDescripcio(), e->getCiutat(),
            e->getMaximPlaces(), e->getPreu(), e->obteDadesEspecifiques());
    }
    return res;
}

// [3.3.3] Consulta Més Reservades (Ranking)
std::vector<DTOExperiencia> CapaDeDomini::consultaMesReservades() {
    auto llista = CapaDeDades::getInstance().experienciesMesReservades();
    std::vector<DTOExperiencia> res;
    for (const auto& e : llista) {
        res.emplace_back(e->obteTipus(), e->getNom(), e->getDescripcio(), e->getCiutat(),
            e->getMaximPlaces(), e->getPreu(), e->obteDadesEspecifiques());
    }
    return res;
}