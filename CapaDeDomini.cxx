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
// Método auxiliar para simular el login (necesario para saber quién borra el usuario)
void CapaDeDomini::iniciarSessio(std::string username) {
    // Pedimos a la capa de datos que busque el usuario
    usuariLoggejat = CapaDeDades::getInstance().obtenirUsuari(username);

    if (!usuariLoggejat) {
        throw std::runtime_error("L'usuari no existeix.");
    }
}

// --- CASO DE USO: Consultar Novetats ---
std::vector<DTOExperiencia> CapaDeDomini::consultarNovetats() {
    // 1. Obtenemos TODAS las experiencias de la Capa de Datos
    // (Recuerda: recupera tanto Escapadas como Actividades gracias a ODB)
    auto totes = CapaDeDades::getInstance().totesExperiencies();

    // 2. Ordenamos en memoria por fecha de alta (descendente)
    // Utilizamos una función lambda para comparar
    std::sort(totes.begin(), totes.end(),
        [](const std::shared_ptr<Experiencia>& a, const std::shared_ptr<Experiencia>& b) {
            // Comparamos strings YYYY-MM-DD (funciona alfabéticamente)
            return a->getDataAlta() > b->getDataAlta();
        });

    // 3. Seleccionamos solo las 10 primeras (o menos si no hay tantas)
    std::vector<DTOExperiencia> resultat;
    int limit = std::min((int)totes.size(), 10);

    for (int i = 0; i < limit; ++i) {
        auto e = totes[i];

        // 4. Convertimos las Categorías (objetos) a Strings para el DTO
        std::vector<std::string> nomsCategories;
        for (const auto& cat : e->getCategories()) {
            nomsCategories.push_back(cat->getNom());
        }

        // 5. Creamos el DTO usando POLIMORFISMO
        // e->obteTipus() devolverá "EXPERIÈNCIA" o "ACTIVITAT" según la subclase
        // e->obteDadesEspecifiques() devolverá el string formateado correcto
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

// --- CASO DE USO: Esborrar Usuari ---
void CapaDeDomini::esborrarUsuari(std::string contrasenya) {
    // Precondición: Debe haber un usuario loggeado
    if (!usuariLoggejat) {
        throw std::runtime_error("No hi ha cap usuari loggejat.");
    }

    // 1. Verificar la contraseña (Lógica de negocio)
    if (usuariLoggejat->getContrasenya() != contrasenya) {
        throw std::runtime_error("Contrasenya incorrecta.");
    }

    // 2. Obtener las reservas del usuario para borrarlas primero
    // (Importante: trabajamos sobre una copia del vector para evitar problemas al iterar y borrar)
    auto reserves = usuariLoggejat->getReserves();

    // 3. Borrar cada reserva individualmente
    for (auto& r : reserves) {
        CapaDeDades::getInstance().esborrarReserva(r);
    }

    // 4. Borrar el usuario
    CapaDeDades::getInstance().esborrarUsuari(usuariLoggejat);

    // 5. Cerrar la sesión (Postcondición)
    usuariLoggejat = nullptr;
}