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

// --- SESIÓN Y USUARIOS ---

// Verifica las credenciales contra la base de datos e inicia sesión
void CapaDeDomini::iniciarSessio(std::string username, std::string contrasenya) {
    auto usuari = CapaDeDades::getInstance().obtenirUsuari(username);
    if (!usuari) throw std::runtime_error("L'usuari no existeix.");
    if (usuari->getContrasenya() != contrasenya) throw std::runtime_error("Contrasenya incorrecta.");
    usuariLoggejat = usuari;
}

// Registra un nuevo usuario comprobando que el nickname y el correo sean únicos y sea mayor de edad
void CapaDeDomini::registrarUsuari(std::string nom, std::string sobrenom, std::string correu,
    std::string pass, int edat) {

    // 1. Validar unicidad de Nickname
    if (CapaDeDades::getInstance().obtenirUsuari(sobrenom) != nullptr) {
        throw std::runtime_error("El sobrenom '" + sobrenom + "' ja existeix.");
    }
    // 2. Validar unicidad de Correo
    if (CapaDeDades::getInstance().obtenirUsuariPerCorreu(correu) != nullptr) {
        throw std::runtime_error("El correu '" + correu + "' ja esta registrat.");
    }
    // 3. Validar Mayoría de Edad (Restricción de integridad)
    if (edat < 18) {
        throw std::runtime_error("L'usuari ha de ser major d'edat.");
    }

    auto u = std::make_shared<Usuari>(sobrenom, nom, correu, pass, edat);
    CapaDeDades::getInstance().insertaUsuari(u);
}

// Obtiene los datos del usuario actual refrescados desde la BD
DTOUsuari CapaDeDomini::consultarUsuari() {
    if (!usuariLoggejat) throw std::runtime_error("No hi ha usuari loggejat.");
    // Refrescamos objeto por si hubo cambios externos
    auto usuariRefrescat = CapaDeDades::getInstance().obtenirUsuari(usuariLoggejat->getSobrenom());
    if (usuariRefrescat) usuariLoggejat = usuariRefrescat;

    auto reserves = CapaDeDades::getInstance().obtenirReservesUsuari(usuariLoggejat);
    return DTOUsuari(usuariLoggejat->getNom(), usuariLoggejat->getSobrenom(),
        usuariLoggejat->getCorreuElectronic(), usuariLoggejat->getEdat(), (int)reserves.size());
}

// Modifica el perfil validando que el nuevo correo no esté en uso y la mayoría de edad
void CapaDeDomini::modificarUsuari(std::string nouNom, std::string nouCorreu, int nouEdat) {
    if (!usuariLoggejat) throw std::runtime_error("No hi ha usuari loggejat.");

    // Validar correo duplicado solo si ha cambiado
    if (!nouCorreu.empty() && nouCorreu != usuariLoggejat->getCorreuElectronic()) {
        auto existe = CapaDeDades::getInstance().obtenirUsuariPerCorreu(nouCorreu);
        if (existe) throw std::runtime_error("El correu electronic ja existeix.");
    }
    // Validar edad
    if (nouEdat != -1 && nouEdat < 18) throw std::runtime_error("L'usuari es menor d'edat.");

    // Aplicar cambios
    if (!nouNom.empty()) usuariLoggejat->setNom(nouNom);
    if (!nouCorreu.empty()) usuariLoggejat->setCorreuElectronic(nouCorreu);
    if (nouEdat != -1) usuariLoggejat->setEdat(nouEdat);

    CapaDeDades::getInstance().modificaUsuari(usuariLoggejat);
}

// Elimina el usuario y sus reservas
void CapaDeDomini::esborrarUsuari(std::string contrasenya) {
    if (!usuariLoggejat) throw std::runtime_error("No hi ha usuari.");
    if (usuariLoggejat->getContrasenya() != contrasenya) throw std::runtime_error("Contrasenya incorrecta.");

    auto reserves = CapaDeDades::getInstance().obtenirReservesUsuari(usuariLoggejat);

    // Borramos las reservas asociadas de la BD
    for (auto& r : reserves) {
        CapaDeDades::getInstance().esborrarReserva(r);
    }

    // Borramos el usuario
    CapaDeDades::getInstance().esborrarUsuari(usuariLoggejat);
    usuariLoggejat = nullptr;
}

// --- RESERVAS ---

// Helper: Obtiene la fecha/hora actual del sistema formateada
std::string CapaDeDomini::_obteDataActual() {
    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);
    std::stringstream ss;
    // Format: AAAA-MM-DD HH:MM
    ss << (now->tm_year + 1900) << "-"
        << std::setw(2) << std::setfill('0') << (now->tm_mon + 1) << "-"
        << std::setw(2) << std::setfill('0') << now->tm_mday << " "
        << std::setw(2) << std::setfill('0') << now->tm_hour << ":"
        << std::setw(2) << std::setfill('0') << now->tm_min;
    return ss.str();
}

// Recupera información de una experiencia para mostrar antes de reservar
DTOExperiencia CapaDeDomini::obtenirDadesExperiencia(std::string nom) {
    if (!usuariLoggejat) throw std::runtime_error("No hi ha usuari loggejat.");
    auto exp = CapaDeDades::getInstance().obtenirExperiencia(nom);
    if (!exp) throw std::runtime_error("L'experiencia no existeix.");

    return DTOExperiencia(exp->obteTipus(), exp->getNom(), exp->getDescripcio(),
        exp->getCiutat(), exp->getMaximPlaces(), exp->getPreu(),
        exp->obteDadesEspecifiques());
}

// Calcula el precio total aplicando el porcentaje de descuento global
// CORRECCIÓN: Si es Escapada es precio fijo, si es Actividad es por persona.
float CapaDeDomini::calcularPreuReserva(std::string nom, int persones) {
    auto exp = CapaDeDades::getInstance().obtenirExperiencia(nom);
    if (!exp) throw std::runtime_error("L'experiencia no existeix.");

    float preuBase = 0.0f;
    if (exp->obteTipus() == "ACTIVITAT") {
        preuBase = exp->getPreu() * persones; // Por persona
    }
    else {
        preuBase = exp->getPreu(); // Total (Escapada)
    }

    float descompte = PlanGo::getInstance().getDescompte();
    return preuBase * ((100.0f - descompte) / 100.0f);
}

// Lógica interna: Valida aforo, calcula precio, crea reserva y actualiza plazas ocupadas
void CapaDeDomini::_processarReserva(std::shared_ptr<Experiencia> exp, int numPlaces) {
    // 1. Validar disponibilidad
    int disponibles = exp->getMaximPlaces() - exp->getNumReserves();

    if (numPlaces > disponibles) {
        std::string error = "No hi ha places suficients. Sol·licitades: " + std::to_string(numPlaces) +
            ", Disponibles: " + std::to_string(disponibles);
        throw std::runtime_error(error);
    }

    // 2. Calcular precio base según tipo (igual que en calcularPreuReserva)
    float preuBase = 0.0f;
    if (exp->obteTipus() == "ACTIVITAT") {
        preuBase = exp->getPreu() * numPlaces;
    }
    else {
        preuBase = exp->getPreu(); // Precio paquete
    }

    // Aplicar descuento
    float descompte = PlanGo::getInstance().getDescompte();
    float preuFinal = preuBase * ((100.0f - descompte) / 100.0f);

    // 3. Obtener fecha sistema
    std::string dataAra = _obteDataActual();

    // 4. Crear reserva y actualizar aforo experiencia
    auto novaReserva = std::make_shared<Reserva>(numPlaces, preuFinal, dataAra, usuariLoggejat, exp);
    exp->setNumReserves(exp->getNumReserves() + numPlaces);

    CapaDeDades::getInstance().insertaReserva(novaReserva);
    CapaDeDades::getInstance().actualitzaExperiencia(exp);
}

// Gestiona reserva de Escapada (se ocupa siempre el máximo de plazas)
void CapaDeDomini::reservarEscapada(std::string nomEscapada) {
    if (!usuariLoggejat) throw std::runtime_error("No hi ha usuari loggejat.");

    auto exp = CapaDeDades::getInstance().obtenirExperiencia(nomEscapada);
    if (!exp) throw std::runtime_error("L'escapada no existeix.");
    if (exp->obteTipus() == "ACTIVITAT") throw std::runtime_error("No es una escapada.");

    // Escapada: Se reservan TODAS las plazas
    int placesAReservar = exp->getMaximPlaces();
    _processarReserva(exp, placesAReservar);
}

// Gestiona reserva de Actividad (número de personas variable)
void CapaDeDomini::reservarActivitat(std::string nomActivitat, int numPersones) {
    if (!usuariLoggejat) throw std::runtime_error("No hi ha usuari loggejat.");

    auto exp = CapaDeDades::getInstance().obtenirExperiencia(nomActivitat);
    if (!exp) throw std::runtime_error("L'activitat no existeix.");
    if (exp->obteTipus() != "ACTIVITAT") throw std::runtime_error("No es una activitat.");

    if (numPersones < 1) throw std::runtime_error("El nombre de persones ha de ser major que 0.");

    _processarReserva(exp, numPersones);
}

// Obtiene el historial de reservas del usuario actual
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

// --- CONSULTAS ---

// Retorna las 10 experiencias más recientes
std::vector<DTOExperiencia> CapaDeDomini::consultarNovetats() {
    auto totes = CapaDeDades::getInstance().totesExperiencies();
    // Ordenar por fecha de alta descendente
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

// Filtra experiencias por ciudad y plazas disponibles
std::vector<DTOExperiencia> CapaDeDomini::consultarExperiencies(std::string ciutat, int places) {
    if (places < 1) throw std::runtime_error("El nombre de persones ha de ser major que 0.");

    auto llista = CapaDeDades::getInstance().cercarExperiencies(ciutat, places);

    std::vector<DTOExperiencia> res;
    for (const auto& e : llista) {
        res.emplace_back(e->obteTipus(), e->getNom(), e->getDescripcio(), e->getCiutat(),
            e->getMaximPlaces(), e->getPreu(), e->obteDadesEspecifiques());
    }
    return res;
}

// Ranking de experiencias más reservadas
std::vector<DTOExperiencia> CapaDeDomini::consultarMesReservades() {
    auto llista = CapaDeDades::getInstance().experienciesMesReservades();
    std::vector<DTOExperiencia> res;
    for (const auto& e : llista) {
        res.emplace_back(e->obteTipus(), e->getNom(), e->getDescripcio(), e->getCiutat(),
            e->getMaximPlaces(), e->getPreu(), e->obteDadesEspecifiques());
    }
    return res;
}