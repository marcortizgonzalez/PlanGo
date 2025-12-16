#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Usuari.hxx"
#include "DTOExperiencia.hxx"
#include "DTOUsuari.hxx"

class CapaDeDomini {
public:
    static CapaDeDomini& getInstance();

    void iniciarSessio(std::string username, std::string contrasenya);
    std::shared_ptr<Usuari> getUsuariLoggejat() const { return usuariLoggejat; }
    void tancarSessio() { usuariLoggejat = nullptr; }

    // BLOQUE A: Gestión Usuarios
    // DataN ahora es 'int edat'
    void registrarUsuari(std::string nom, std::string sobrenom, std::string correu,
        std::string pass, int edat);

    DTOUsuari consultarUsuari();

    // Modificar: recibe strings. Si están vacíos, no se cambian. 'nouEdat' es int (-1 si no cambia).
    void modificarUsuari(std::string nouNom, std::string nouCorreu, int nouEdat);

    void esborrarUsuari(std::string contrasenya);

    std::vector<DTOExperiencia> consultarNovetats();

private:
    CapaDeDomini() {}
    std::shared_ptr<Usuari> usuariLoggejat;
};