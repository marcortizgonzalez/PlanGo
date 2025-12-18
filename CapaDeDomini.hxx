#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Usuari.hxx"
#include "DTOExperiencia.hxx"
#include "DTOUsuari.hxx"
#include "DTOReserva.hxx"
#include "Experiencia.hxx"

class CapaDeDomini {
public:
    static CapaDeDomini& getInstance();

    // Sessió
    void iniciarSessio(std::string username, std::string contrasenya);
    std::shared_ptr<Usuari> getUsuariLoggejat() const { return usuariLoggejat; }
    void tancarSessio() { usuariLoggejat = nullptr; }

    // Bloque A: Usuaris
    void registrarUsuari(std::string nom, std::string sobrenom, std::string correu, std::string pass, int edat);
    DTOUsuari consultarUsuari();
    void modificarUsuari(std::string nouNom, std::string nouCorreu, int nouEdat);
    void esborrarUsuari(std::string contrasenya);

    // Bloque B: Reserves
    void reservarEscapada(std::string nomEscapada, int numPersones, std::string data);
    void reservarActivitat(std::string nomActivitat, int numPersones, std::string data);
    std::vector<DTOReserva> consultarReserves();

    // Bloque C: Consultes
    std::vector<DTOExperiencia> consultarNovetats();
    std::vector<DTOExperiencia> consultarExperiencies(std::string ciutat, float preuMax);
    std::vector<DTOExperiencia> consultarMesReservades();

private:
    CapaDeDomini() {}
    std::shared_ptr<Usuari> usuariLoggejat;
    void _processarReserva(std::shared_ptr<Experiencia> exp, int numPlaces, std::string data);
};