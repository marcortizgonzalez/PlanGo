#include "CapaDeDades.hxx"
#include "ConnexioBD.hxx"
#include <iostream>

// ODB Includes
#include <odb/mysql/database.hxx>
#include <odb/transaction.hxx>
#include <odb/schema-catalog.hxx>

// Includes Generados
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

// --- CONSULTA POLIMÓRFICA CORREGIDA ---
std::vector<std::shared_ptr<Experiencia>> CapaDeDades::totesExperiencies() {
    std::vector<std::shared_ptr<Experiencia>> result_list;
    try {
        odb::transaction t(db->begin());
        typedef odb::result<Experiencia> result;

        result r(db->query<Experiencia>());

        // CORRECCIÓN CLAVE: Usamos iterador explícito 'i'
        // 'i.load()' devuelve el shared_ptr polimórfico correcto (Escapada o Activitat)
        for (result::iterator i(r.begin()); i != r.end(); ++i) {
            result_list.push_back(i.load());
        }

        t.commit();
    }
    catch (const odb::exception& e) {
        std::cerr << "Error ODB a totesExperiencies: " << e.what() << std::endl;
    }
    return result_list;
}

std::shared_ptr<Usuari> CapaDeDades::obtenirUsuari(std::string username) {
    std::shared_ptr<Usuari> u;
    try {
        odb::transaction t(db->begin());
        u = db->find<Usuari>(username);
        t.commit();
    }
    catch (...) {}
    return u;
}

// --- FUNCIÓN QUE FALTABA ---
std::vector<std::shared_ptr<Reserva>> CapaDeDades::obtenirReservesUsuari(std::shared_ptr<Usuari> u) {
    std::vector<std::shared_ptr<Reserva>> llista;
    try {
        odb::transaction t(db->begin());

        typedef odb::query<Reserva> query;
        typedef odb::result<Reserva> result;

        // Buscamos reservas por nombre de usuario
        result r(db->query<Reserva>(query::usuari == u->getUsername()));

        for (auto& res : r) {
            // Aquí hacemos una copia del objeto en memoria
            llista.push_back(std::make_shared<Reserva>(res));
        }
        t.commit();
    }
    catch (const odb::exception& e) {
        std::cerr << "Error carregant reserves: " << e.what() << std::endl;
    }
    return llista;
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

void CapaDeDades::insertaUsuari(std::shared_ptr<Usuari> u) {
    try {
        odb::transaction t(db->begin());
        db->persist(u);
        t.commit();
    }
    catch (const odb::exception& e) {
        throw std::runtime_error("Error Insert: " + std::string(e.what()));
    }
}

void CapaDeDades::modificaUsuari(std::shared_ptr<Usuari> u) {
    try {
        odb::transaction t(db->begin());
        db->update(u);
        t.commit();
    }
    catch (const odb::exception& e) {
        throw std::runtime_error("Error Update: " + std::string(e.what()));
    }
}