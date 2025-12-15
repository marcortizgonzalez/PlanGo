#pragma once
#include <memory>
#include <vector>
#include <odb/database.hxx>
#include "Experiencia.hxx"
#include "Usuari.hxx"
#include "Reserva.hxx"

class CapaDeDades {
public:
    // Patrón Singleton
    static CapaDeDades& getInstance();

    // Métodos para "Consultar Novetats"
    std::vector<std::shared_ptr<Experiencia>> totesExperiencies();

    // Métodos para "Esborrar Usuari"
    std::shared_ptr<Usuari> obtenirUsuari(std::string username);
    void esborrarReserva(std::shared_ptr<Reserva> r);
    void esborrarUsuari(std::shared_ptr<Usuari> u);

private:
    CapaDeDades(); // Constructor privado
    // std::unique_ptr<odb::database> db;
    std::shared_ptr<odb::database> db;
};