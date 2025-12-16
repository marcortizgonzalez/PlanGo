#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Usuari.hxx"
#include "DTOExperiencia.hxx"
#include "DTOUsuari.hxx" // Necesario para consultarUsuari

class CapaDeDomini {
public:
    static CapaDeDomini& getInstance();

    // Gestión de Sesión (AHORA CON 2 ARGUMENTOS)
    void iniciarSessio(std::string username, std::string contrasenya);
    std::shared_ptr<Usuari> getUsuariLoggejat() const { return usuariLoggejat; }
    void tancarSessio() { usuariLoggejat = nullptr; }

    // BLOQUE A: Gestión Usuarios
    void registrarUsuari(std::string nom, std::string user, std::string mail,
        std::string pass, std::string dataN);
    DTOUsuari consultarUsuari();
    void modificarUsuari(std::string nouNom, std::string nouMail,
        std::string novaPass, std::string novaData);
    void esborrarUsuari(std::string contrasenya);

    // BLOQUE C: Consultas
    std::vector<DTOExperiencia> consultarNovetats();

private:
    CapaDeDomini() {}
    std::shared_ptr<Usuari> usuariLoggejat;
};