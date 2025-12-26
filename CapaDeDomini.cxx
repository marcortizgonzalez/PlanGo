#define _CRT_SECURE_NO_WARNINGS
#include "CapaDeDomini.hxx"
#include "CapaDeDades.hxx"
#include "PlanGo.hxx"
#include <algorithm>
#include <stdexcept>
#include <ctime>
#include <iomanip>
#include <sstream>

// Patrón Singleton: Garantiza una única instancia del controlador de dominio
CapaDeDomini& CapaDeDomini::getInstance() {
    static CapaDeDomini instance;
    return instance;
}

// --- SESSIÓ I USUARIS ---

// Verifica credenciales y establece el usuario actual en el sistema
void CapaDeDomini::iniciarSessio(std::string username, std::string contrasenya) {
    auto usuari = CapaDeDades::getInstance().obtenirUsuari(username);
    if (!usuari) throw std::runtime_error("L'usuari no existeix.");
    if (usuari->getContrasenya() != contrasenya) throw std::runtime_error("Contrasenya incorrecta.");
    usuariLoggejat = usuari;
}

// Registra un nuevo usuario tras verificar que el nickname y el correo no existan
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

// Devuelve un DTO con los datos actualizados del usuario logueado
DTOUsuari CapaDeDomini::consultarUsuari() {
    if (!usuariLoggejat) throw std::runtime_error("No hi ha usuari loggejat.");
    // Refrescamos la información desde la BD por si hubo cambios
    auto usuariRefrescat = CapaDeDades::getInstance().obtenirUsuari(usuariLoggejat->getSobrenom());
    if (usuariRefrescat) usuariLoggejat = usuariRefrescat;
    auto reserves = CapaDeDades::getInstance().obtenirReservesUsuari(usuariLoggejat);
    return DTOUsuari(usuariLoggejat->getNom(), usuariLoggejat->getSobrenom(),
        usuariLoggejat->getCorreuElectronic(), usuariLoggejat->getEdat(), (int)reserves.size());
}

// Modifica perfil: Valida unicidad de correo y mayoría de edad antes de guardar
void CapaDeDomini::modificarUsuari(std::string nouNom, std::string nouCorreu, int nouEdat) {
    if (!usuariLoggejat) throw std::runtime_error("No hi ha usuari loggejat.");

    // Validación de correo duplicado
    if (!nouCorreu.empty() && nouCorreu != usuariLoggejat->getCorreuElectronic()) {
        auto existe = CapaDeDades::getInstance().obtenirUsuariPerCorreu(nouCorreu);
        if (existe) throw std::runtime_error("El correu electronic ja existeix.");
    }
    // Validación de edad
    if (nouEdat != -1 && nouEdat < 18) throw std::runtime_error("L'usuari es menor d'edat.");

    // Aplicación de cambios
    if (!nouNom.empty()) usuariLoggejat->setNom(nouNom);
    if (!nouCorreu.empty()) usuariLoggejat->setCorreuElectronic(nouCorreu);
    if (nouEdat != -1) usuariLoggejat->setEdat(nouEdat);

    CapaDeDades::getInstance().modificaUsuari(usuariLoggejat);
}

// Borra el usuario y sus reservas, liberando las plazas ocupadas en las experiencias
void CapaDeDomini::esborrarUsuari(std::string contrasenya) {
    if (!usuariLoggejat) throw std::runtime_error("No hi ha usuari.");
    if (usuariLoggejat->getContrasenya() != contrasenya) throw std::runtime_error("Contrasenya incorrecta.");

    auto reserves = CapaDeDades::getInstance().obtenirReservesUsuari(usuariLoggejat);

    // Bucle crítico: Restaura las plazas libres antes de borrar la reserva
    for (auto& r : reserves) {
        auto exp = r->getExperiencia();
        int placesAlliberades = r->getNumPlaces();
        int actuals = exp->getNumReserves();

        int novesReserves = (actuals >= placesAlliberades) ? (actuals - placesAlliberades) : 0;
        exp->setNumReserves(novesReserves);

        CapaDeDades::getInstance().actualitzaExperiencia(exp);
        CapaDeDades::getInstance().esborrarReserva(r);
    }

    CapaDeDades::getInstance().esborrarUsuari(usuariLoggejat);
    usuariLoggejat = nullptr;
}

// --- HELPERS (Utilidades) ---

// Obtiene la fecha/hora actual del sistema formateada como string
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

// Recupera datos de una experiencia para mostrarlos en la UI antes de confirmar
DTOExperiencia CapaDeDomini::obtenirDadesExperiencia(std::string nom) {
    if (!usuariLoggejat) throw std::runtime_error("No hi ha usuari loggejat.");
    auto exp = CapaDeDades::getInstance().obtenirExperiencia(nom);
    if (!exp) throw std::runtime_error("L'experiencia no existeix.");

    // Retorna DTO (incluye numReserves corregido)
    return DTOExperiencia(exp->obteTipus(), exp->getNom(), exp->getDescripcio(),
        exp->getCiutat(), exp->getMaximPlaces(), exp->getPreu(),
        exp->obteDadesEspecifiques(), exp->getNumReserves());
}

// Calcula el precio total con descuento sin efectuar la reserva (Pre-visualización)
float CapaDeDomini::calcularPreuReserva(std::string nom, int persones) {
    auto exp = CapaDeDades::getInstance().obtenirExperiencia(nom);
    if (!exp) throw std::runtime_error("L'experiencia no existeix.");

    float preuBase = exp->getPreu() * persones;
    float descompte = PlanGo::getInstance().getDescompte();
    return preuBase * ((100.0f - descompte) / 100.0f);
}

// --- RESERVES (Lógica interna) ---

// Método centralizado: Valida plazas, aplica descuento, crea reserva y actualiza BD
void CapaDeDomini::_processarReserva(std::shared_ptr<Experiencia> exp, int numPlaces) {
    // 1. Validar disponibilidad
    int disponibles = exp->getMaximPlaces() - exp->getNumReserves();

    if (numPlaces > disponibles) {
        std::string error = "No hi ha places suficients. Sol·licitades: " + std::to_string(numPlaces) +
            ", Disponibles: " + std::to_string(disponibles);
        throw std::runtime_error(error);
    }

    // 2. Cálculo económico
    float preuBase = exp->getPreu() * numPlaces;
    float descompte = PlanGo::getInstance().getDescompte();
    float preuFinal = preuBase * ((100.0f - descompte) / 100.0f);

    // 3. Fecha y Persistencia
    std::string dataAra = _obteDataActual();
    auto novaReserva = std::make_shared<Reserva>(numPlaces, preuFinal, dataAra, usuariLoggejat, exp);

    exp->setNumReserves(exp->getNumReserves() + numPlaces);

    CapaDeDades::getInstance().insertaReserva(novaReserva);
    CapaDeDades::getInstance().actualitzaExperiencia(exp);
}

// Gestiona reserva de Escapada: Asigna automáticamente el aforo máximo
void CapaDeDomini::reservarEscapada(std::string nomEscapada) {
    if (!usuariLoggejat) throw std::runtime_error("No hi ha usuari loggejat.");

    auto exp = CapaDeDades::getInstance().obtenirExperiencia(nomEscapada);
    if (!exp) throw std::runtime_error("L'escapada no existeix.");
    if (exp->obteTipus() == "ACTIVITAT") throw std::runtime_error("No es una escapada.");

    int placesAReservar = exp->getMaximPlaces();
    _processarReserva(exp, placesAReservar);
}

// Gestiona reserva de Actividad: Valida tipo y cantidad de personas > 0
void CapaDeDomini::reservarActivitat(std::string nomActivitat, int numPersones) {
    if (!usuariLoggejat) throw std::runtime_error("No hi ha usuari loggejat.");

    auto exp = CapaDeDades::getInstance().obtenirExperiencia(nomActivitat);
    if (!exp) throw std::runtime_error("L'activitat no existeix.");
    if (exp->obteTipus() != "ACTIVITAT") throw std::runtime_error("No es una activitat.");

    if (numPersones < 1) throw std::runtime_error("El nombre de persones ha de ser major que 0.");

    _processarReserva(exp, numPersones);
}

// Obtiene historial de reservas del usuario y lo convierte a DTOs
std::vector<DTOReserva> CapaDeDomini::consultarReserves() {
    if (!usuariLoggejat) throw std::runtime_error("No hi ha usuari loggejat.");
    auto reserves = CapaDeDades::getInstance().obtenirReservesUsuari(usuariLoggejat);
    std::vector<DTOReserva> result;
    for (const auto& r : reserves) {
        auto e = r->getExperiencia();
        DTOExperiencia dtoExp(e->obteTipus(), e->getNom(), e->getDescripcio(), e->getCiutat(),
            e->getMaximPlaces(), e->getPreu(), e->obteDadesEspecifiques(), e->getNumReserves());
        result.emplace_back(r->getData(), r->getNumPlaces(), r->getPreuPagat(), dtoExp);
    }
    return result;
}

// --- CONSULTES ---

// Obtiene las 10 últimas experiencias dadas de alta
std::vector<DTOExperiencia> CapaDeDomini::consultarNovetats() {
    auto totes = CapaDeDades::getInstance().totesExperiencies();
    // Ordenar por fecha descendente
    std::sort(totes.begin(), totes.end(), [](const std::shared_ptr<Experiencia>& a, const std::shared_ptr<Experiencia>& b) {
        return a->getDataAlta() > b->getDataAlta();
        });
    std::vector<DTOExperiencia> res;
    int limit = std::min((int)totes.size(), 10);
    for (int i = 0; i < limit; ++i) {
        auto e = totes[i];
        res.emplace_back(e->obteTipus(), e->getNom(), e->getDescripcio(), e->getCiutat(),
            e->getMaximPlaces(), e->getPreu(), e->obteDadesEspecifiques(), e->getNumReserves());
    }
    return res;
}

// Busca experiencias filtrando por ciudad y plazas disponibles
std::vector<DTOExperiencia> CapaDeDomini::consultarExperiencies(std::string ciutat, int places) {
    if (places < 1) throw std::runtime_error("El nombre de persones ha de ser major que 0.");
    auto llista = CapaDeDades::getInstance().cercarExperiencies(ciutat, places);
    std::vector<DTOExperiencia> res;
    for (const auto& e : llista) {
        res.emplace_back(e->obteTipus(), e->getNom(), e->getDescripcio(), e->getCiutat(),
            e->getMaximPlaces(), e->getPreu(), e->obteDadesEspecifiques(), e->getNumReserves());
    }
    return res;
}

// Obtiene ranking de populares (Top 10) para visualización
std::vector<DTOExperiencia> CapaDeDomini::consultarMesReservades() {
    auto llista = CapaDeDades::getInstance().experienciesMesReservades();
    std::vector<DTOExperiencia> res;
    for (const auto& e : llista) {
        res.emplace_back(e->obteTipus(), e->getNom(), e->getDescripcio(), e->getCiutat(),
            e->getMaximPlaces(), e->getPreu(), e->obteDadesEspecifiques(), e->getNumReserves());
    }
    return res;
}