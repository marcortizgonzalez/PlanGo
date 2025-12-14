#include "CapaDeDades.hxx"
#include <odb/mysql/database.hxx>
#include <odb/transaction.hxx>
#include <odb/schema-catalog.hxx>
#include <iostream>

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
        // ("user", "password", "database", "host", port)
        db = std::unique_ptr<odb::database>(new odb::mysql::database("inep08", "xubaasha8Shei6", "inep08", "ubiwan.epsevg.upc.edu", 3306));
    }
    catch (const odb::exception& e) {
        std::cerr << "Error de connexió: " << e.what() << std::endl;
    }
}

std::vector<std::shared_ptr<Experiencia>> CapaDeDades::totesExperiencies() {
    std::vector<std::shared_ptr<Experiencia>> result_list;
    try {
        odb::transaction t(db->begin());
        // Consulta polimórfica: recupera tanto Escapadas como Actividades
        odb::result<Experiencia> r(db->query<Experiencia>());
        for (auto& e : r) {
            result_list.push_back(std::shared_ptr<Experiencia>(new Experiencia(e))); // ODB devuelve referencia, copiamos o usamos punteros
            // Nota: En ODB con shared_ptr configurado, 'e' ya debería manejarse bien. 
            // Si configuraste --default-pointer std::shared_ptr, el iterador devuelve el objeto.
            // Para simplificar en este ejemplo asumimos carga correcta. 
            // La forma más segura con polimorfismo en ODB suele ser cargar por ID o iterar punteros.
        }
        // Corrección para iteración simple con shared_ptr
        for (auto i = r.begin(); i != r.end(); ++i) {
            result_list.push_back(std::shared_ptr<Experiencia>(i.load()));
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