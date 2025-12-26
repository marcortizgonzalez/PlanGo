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

    // Consultes Avançades
    std::vector<std::shared_ptr<Experiencia>> cercarExperiencies(std::string ciutat, int places);
    std::vector<std::shared_ptr<Experiencia>> experienciesMesReservades();

    // Métodos generales
    std::vector<std::shared_ptr<Experiencia>> totesExperiencies();

    // Búsquedas
    std::shared_ptr<Usuari> obtenirUsuari(std::string sobrenom);
    std::shared_ptr<Usuari> obtenirUsuariPerCorreu(std::string correu);
    std::shared_ptr<Experiencia> obtenirExperiencia(std::string nom);

    // Reservas
    std::vector<std::shared_ptr<Reserva>> obtenirReservesUsuari(std::shared_ptr<Usuari> u);

    // Modificaciones
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