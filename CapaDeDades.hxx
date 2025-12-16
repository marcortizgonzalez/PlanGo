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
    std::shared_ptr<Usuari> obtenirUsuari(std::string username);

    // Métodos para evitar bloqueos de ODB (Carga manual)
    std::vector<std::shared_ptr<Reserva>> obtenirReservesUsuari(std::shared_ptr<Usuari> u);

    // Métodos de borrado
    void esborrarReserva(std::shared_ptr<Reserva> r);
    void esborrarUsuari(std::shared_ptr<Usuari> u);

    // --- BLOQUE A: Gestión Usuarios ---
    void insertaUsuari(std::shared_ptr<Usuari> u);
    void modificaUsuari(std::shared_ptr<Usuari> u);

private:
    CapaDeDades();
    std::shared_ptr<odb::database> db;
};