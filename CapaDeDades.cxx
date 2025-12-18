#include "CapaDeDades.hxx"
#include "ConnexioBD.hxx"
#include <iostream>
#include <odb/mysql/database.hxx>
#include <odb/transaction.hxx>

// Includes ODB
#include "Experiencia-odb.hxx"
#include "Usuari-odb.hxx"
#include "Reserva-odb.hxx"
#include "Escapada-odb.hxx"
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
        std::cerr << "Error connexio: " << e.what() << std::endl;
    }
}

// --- CONSULTAS BLOQUE C (CORREGIDAS) ---

std::vector<std::shared_ptr<Experiencia>> CapaDeDades::cercarExperiencies(std::string ciutat, float preuMax) {
    std::vector<std::shared_ptr<Experiencia>> result_list;
    try {
        odb::transaction t(db->begin());
        typedef odb::query<Experiencia> query;
        typedef odb::result<Experiencia> result;

        // CORRECCIÓN: Paréntesis envolviendo la lógica antes de sumar el string "ORDER BY"
        result r(db->query<Experiencia>(
            (query::ciutat == ciutat && query::preu <= preuMax) + "ORDER BY" + query::preu
        ));

        for (result::iterator i(r.begin()); i != r.end(); ++i) {
            result_list.push_back(i.load());
        }
        t.commit();
    }
    catch (const odb::exception& e) {
        std::cerr << "Error cercant experiencies: " << e.what() << std::endl;
    }
    return result_list;
}

std::vector<std::shared_ptr<Experiencia>> CapaDeDades::experienciesMesReservades() {
    std::vector<std::shared_ptr<Experiencia>> result_list;
    try {
        odb::transaction t(db->begin());
        typedef odb::query<Experiencia> query;
        typedef odb::result<Experiencia> result;

        // CORRECCIÓN: Aseguramos que el string sea std::string o usamos sintaxis ODB pura
        result r(db->query<Experiencia>("ORDER BY" + query::numReserves + "DESC"));

        for (result::iterator i(r.begin()); i != r.end(); ++i) {
            result_list.push_back(i.load());
        }
        t.commit();
    }
    catch (const odb::exception& e) {
        std::cerr << "Error ranking experiencies: " << e.what() << std::endl;
    }
    return result_list;
}

// --- RESTO DE MÉTODOS (Bloques A y B) ---

std::vector<std::shared_ptr<Experiencia>> CapaDeDades::totesExperiencies() {
    std::vector<std::shared_ptr<Experiencia>> result_list;
    try {
        odb::transaction t(db->begin());
        typedef odb::result<Experiencia> result;
        result r(db->query<Experiencia>());
        for (result::iterator i(r.begin()); i != r.end(); ++i) {
            result_list.push_back(i.load());
        }
        t.commit();
    }
    catch (...) {}
    return result_list;
}

std::shared_ptr<Usuari> CapaDeDades::obtenirUsuari(std::string sobrenom) {
    std::shared_ptr<Usuari> u;
    try {
        odb::transaction t(db->begin());
        u = db->find<Usuari>(sobrenom);
        t.commit();
    }
    catch (...) {}
    return u;
}

std::shared_ptr<Usuari> CapaDeDades::obtenirUsuariPerCorreu(std::string correu) {
    std::shared_ptr<Usuari> u;
    try {
        odb::transaction t(db->begin());
        typedef odb::query<Usuari> query;
        u = db->query_one<Usuari>(query::correuElectronic == correu);
        t.commit();
    }
    catch (...) {}
    return u;
}

std::vector<std::shared_ptr<Reserva>> CapaDeDades::obtenirReservesUsuari(std::shared_ptr<Usuari> u) {
    std::vector<std::shared_ptr<Reserva>> llista;
    try {
        odb::transaction t(db->begin());
        typedef odb::query<Reserva> query;
        typedef odb::result<Reserva> result;
        result r(db->query<Reserva>(query::usuari == u->getSobrenom()));
        for (auto& res : r) llista.push_back(std::make_shared<Reserva>(res));
        t.commit();
    }
    catch (...) {}
    return llista;
}

std::shared_ptr<Experiencia> CapaDeDades::obtenirExperiencia(std::string nom) {
    std::shared_ptr<Experiencia> e;
    try {
        odb::transaction t(db->begin());
        e = db->find<Experiencia>(nom);
        t.commit();
    }
    catch (...) {}
    return e;
}

void CapaDeDades::insertaUsuari(std::shared_ptr<Usuari> u) {
    try {
        odb::transaction t(db->begin());
        db->persist(u);
        t.commit();
    }
    catch (const odb::exception& e) { throw std::runtime_error(e.what()); }
}

void CapaDeDades::modificaUsuari(std::shared_ptr<Usuari> u) {
    try {
        odb::transaction t(db->begin());
        db->update(u);
        t.commit();
    }
    catch (const odb::exception& e) { throw std::runtime_error(e.what()); }
}

void CapaDeDades::actualitzaExperiencia(std::shared_ptr<Experiencia> e) {
    try {
        odb::transaction t(db->begin());
        db->update(e);
        t.commit();
    }
    catch (const odb::exception& e) { throw std::runtime_error(e.what()); }
}

void CapaDeDades::insertaReserva(std::shared_ptr<Reserva> r) {
    try {
        odb::transaction t(db->begin());
        db->persist(r);
        t.commit();
    }
    catch (const odb::exception& e) { throw std::runtime_error(e.what()); }
}

void CapaDeDades::esborrarReserva(std::shared_ptr<Reserva> r) {
    try {
        odb::transaction t(db->begin());
        db->erase(r);
        t.commit();
    }
    catch (...) {}
}

void CapaDeDades::esborrarUsuari(std::shared_ptr<Usuari> u) {
    try {
        odb::transaction t(db->begin());
        db->erase(u);
        t.commit();
    }
    catch (...) {}
}