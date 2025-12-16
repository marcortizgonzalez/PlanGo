#pragma once
#include <memory>
#include <vector>
#include <odb/database.hxx> // Importante para tipos genéricos
#include "Experiencia.hxx"
#include "Usuari.hxx"
#include "Reserva.hxx"

class CapaDeDades {
public:
    static CapaDeDades& getInstance();

    // Métodos (sin cambios)
    std::vector<std::shared_ptr<Experiencia>> totesExperiencies();
    std::shared_ptr<Usuari> obtenirUsuari(std::string username);
    void esborrarReserva(std::shared_ptr<Reserva> r);
    void esborrarUsuari(std::shared_ptr<Usuari> u);

private:
    CapaDeDades();
    std::shared_ptr<odb::database> db;
};