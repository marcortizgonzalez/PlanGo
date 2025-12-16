#include "CapaDeDades.hxx"
#include <odb/mysql/database.hxx>
#include <odb/transaction.hxx>
#include <odb/schema-catalog.hxx>
#include <iostream>

#include "ConnexioBD.hxx"

// Includes de los archivos generados por ODB (¡Importante!)
#include "Experiencia-odb.hxx"
#include "Usuari-odb.hxx"
#include "Reserva-odb.hxx"
#include "Escapada-odb.hxx" // Necesario para polimorfismo
#include "Activitat-odb.hxx"

CapaDeDades& CapaDeDades::getInstance() {
    static CapaDeDades instance;
    return instance;
}

CapaDeDades::CapaDeDades() {
    try {

        this->db = ConnexioBD::getInstance().getDB();
    }
    catch (const odb::exception& e) {
        std::cerr << "Error de connexió: " << e.what() << std::endl;
    }
}

std::vector<std::shared_ptr<Experiencia>> CapaDeDades::totesExperiencies() {
    std::vector<std::shared_ptr<Experiencia>> result_list;

    try {
        odb::transaction t(db->begin());

        // CORRECTE: Consultem la classe base 'Experiencia'.
        // ODB ja sap que ha de retornar punteros perquè ho vam configurar amb --default-pointer
        typedef odb::result<Experiencia> result;

        // Executem la consulta
        result r(db->query<Experiencia>());

        // Iterem. Com que és polimòrfic, 'i.load()' ens donarà el shared_ptr correcte
        for (result::iterator i(r.begin()); i != r.end(); ++i) {
            result_list.push_back(i.load());
        }

        t.commit();
    }
    catch (const odb::exception& e) {
        std::cerr << "Error ODB: " << e.what() << std::endl;
    }
    return result_list;
}

std::shared_ptr<Usuari> CapaDeDades::obtenirUsuari(std::string username) {
    std::shared_ptr<Usuari> u;
    try {
        odb::transaction t(db->begin());
        u = db->load<Usuari>(username);
        t.commit();
    }
    catch (...) {} // Si no existe, devuelve null
    return u;
}

void CapaDeDades::esborrarReserva(std::shared_ptr<Reserva> r) {
    try {
        odb::transaction t(db->begin());
        db->erase(r);
        t.commit();
    }
    catch (const odb::exception& e) {
        std::cerr << "Error esborrant reserva: " << e.what() << std::endl;
    }
}

void CapaDeDades::esborrarUsuari(std::shared_ptr<Usuari> u) {
    try {
        odb::transaction t(db->begin());
        db->erase(u);
        t.commit();
    }
    catch (const odb::exception& e) {
        std::cerr << "Error esborrant usuari: " << e.what() << std::endl;
    }
}