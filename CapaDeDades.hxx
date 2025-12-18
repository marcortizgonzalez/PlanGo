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

    // Métodos generales
    std::vector<std::shared_ptr<Experiencia>> totesExperiencies();

    // Búsquedas
    std::shared_ptr<Usuari> obtenirUsuari(std::string sobrenom);
    std::shared_ptr<Usuari> obtenirUsuariPerCorreu(std::string correu); // NUEVO

    // Reservas
    std::vector<std::shared_ptr<Reserva>> obtenirReservesUsuari(std::shared_ptr<Usuari> u);
    std::shared_ptr<Experiencia> obtenirExperiencia(std::string nom);
    void actualitzaExperiencia(std::shared_ptr<Experiencia> e);
    void insertaReserva(std::shared_ptr<Reserva> r);

    // Modificación
    void insertaUsuari(std::shared_ptr<Usuari> u);
    void modificaUsuari(std::shared_ptr<Usuari> u);
    void esborrarReserva(std::shared_ptr<Reserva> r);
    void esborrarUsuari(std::shared_ptr<Usuari> u);

private:
    CapaDeDades();
    std::shared_ptr<odb::database> db;
};