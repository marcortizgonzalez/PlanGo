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

    // Sessió i Usuaris
    void iniciarSessio(std::string u, std::string p);
    std::shared_ptr<Usuari> getUsuariLoggejat() const { return usuariLoggejat; }
    void tancarSessio() { usuariLoggejat = nullptr; }
    void registrarUsuari(std::string n, std::string s, std::string c, std::string p, int e);
    DTOUsuari consultarUsuari();
    void modificarUsuari(std::string n, std::string c, int e);
    void esborrarUsuari(std::string p);

    // Reserves
    DTOExperiencia obtenirDadesExperiencia(std::string nom);
    float calcularPreuReserva(std::string nom, int persones);
    void reservarEscapada(std::string nom);
    void reservarActivitat(std::string nom, int persones);
    std::vector<DTOReserva> consultarReserves();

    // Consultes
    std::vector<DTOExperiencia> consultarNovetats();

    std::vector<DTOExperiencia> consultarExperiencies(std::string ciutat, int places);

    std::vector<DTOExperiencia> consultarMesReservades();

private:
    CapaDeDomini() {}
    std::shared_ptr<Usuari> usuariLoggejat;
    void _processarReserva(std::shared_ptr<Experiencia> exp, int numPlaces);
    std::string _obteDataActual();
};