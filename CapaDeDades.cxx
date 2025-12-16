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

// Buscar por correo electrónico para validaciones
std::shared_ptr<Usuari> CapaDeDades::obtenirUsuariPerCorreu(std::string correu) {
    std::shared_ptr<Usuari> u;
    try {
        odb::transaction t(db->begin());
        typedef odb::query<Usuari> query;
        // Consulta ODB buscando por el campo 'correuElectronic'
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
        for (auto& res : r) {
            llista.push_back(std::make_shared<Reserva>(res));
        }
        t.commit();
    }
    catch (...) {}
    return llista;
}

void CapaDeDades::insertaUsuari(std::shared_ptr<Usuari> u) {
    try {
        odb::transaction t(db->begin());
        db->persist(u);
        t.commit();
    }
    catch (const odb::exception& e) {
        throw std::runtime_error("Error SQL Insert: " + std::string(e.what()));
    }
}

void CapaDeDades::modificaUsuari(std::shared_ptr<Usuari> u) {
    try {
        odb::transaction t(db->begin());
        db->update(u);
        t.commit();
    }
    catch (const odb::exception& e) {
        throw std::runtime_error("Error SQL Update: " + std::string(e.what()));
    }
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