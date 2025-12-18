#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Usuari.hxx"
#include "DTOExperiencia.hxx"
#include "DTOUsuari.hxx"
#include "DTOReserva.hxx" // Necesario para consultarReserves
#include "Experiencia.hxx" // Necesario para el helper

class CapaDeDomini {
public:
    static CapaDeDomini& getInstance();

    void iniciarSessio(std::string username, std::string contrasenya);
    std::shared_ptr<Usuari> getUsuariLoggejat() const { return usuariLoggejat; }
    void tancarSessio() { usuariLoggejat = nullptr; }

    // BLOQUE A: Gestión Usuarios
    void registrarUsuari(std::string nom, std::string sobrenom, std::string correu,
        std::string pass, int edat);

    DTOUsuari consultarUsuari();
    void modificarUsuari(std::string nouNom, std::string nouCorreu, int nouEdat);
    void esborrarUsuari(std::string contrasenya);

    // BLOQUE B: Reservas
    void reservarEscapada(std::string nomEscapada, int numPersones, std::string data);
    void reservarActivitat(std::string nomActivitat, int numPersones, std::string data);
    std::vector<DTOReserva> consultarReserves();

    // BLOQUE C: Consultas
    std::vector<DTOExperiencia> consultarNovetats();

private:
    CapaDeDomini() {}
    std::shared_ptr<Usuari> usuariLoggejat;

    // --- ESTA ES LA LÍNEA QUE FALTABA ---
    void _processarReserva(std::shared_ptr<Experiencia> exp, int numPlaces, std::string data);
};