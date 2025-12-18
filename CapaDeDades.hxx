#pragma once
#include <memory>
#include <vector>
#include <odb/database.hxx>
#include "Experiencia.hxx"
#include "Usuari.hxx"
#include "Reserva.hxx"

class CapaDeDades {
public:
    static CapaDeDades& getInstance();

    // Consultes Generals
    std::vector<std::shared_ptr<Experiencia>> totesExperiencies();

    // --- BLOQUE C: Consultas Avanzadas ---
    // Filtra por ciudad y precio máximo, ordenado por precio ascendente
    std::vector<std::shared_ptr<Experiencia>> cercarExperiencies(std::string ciutat, float preuMax);

    // Retorna todas las experiencias ordenadas por número de reservas (Descendente)
    std::vector<std::shared_ptr<Experiencia>> experienciesMesReservades();

    // Búsquedas Simples
    std::shared_ptr<Usuari> obtenirUsuari(std::string sobrenom);
    std::shared_ptr<Usuari> obtenirUsuariPerCorreu(std::string correu);

    // Reservas (Bloque B)
    std::shared_ptr<Experiencia> obtenirExperiencia(std::string nom);
    std::vector<std::shared_ptr<Reserva>> obtenirReservesUsuari(std::shared_ptr<Usuari> u);

    // Modificaciones (Transacciones)
    void insertaUsuari(std::shared_ptr<Usuari> u);
    void modificaUsuari(std::shared_ptr<Usuari> u);
    void insertaReserva(std::shared_ptr<Reserva> r);
    void actualitzaExperiencia(std::shared_ptr<Experiencia> e);

    void esborrarReserva(std::shared_ptr<Reserva> r);
    void esborrarUsuari(std::shared_ptr<Usuari> u);

private:
    CapaDeDades();
    std::shared_ptr<odb::database> db;
};