#pragma once
#include <vector>
#include <memory>
#include "Usuari.hxx"
#include "DTOExperiencia.hxx"

class CapaDeDomini {
public:
    static CapaDeDomini& getInstance();

    // Gestión de sesión (simulada para probar "Esborrar Usuari")
    void iniciarSessio(std::string username);
    std::shared_ptr<Usuari> getUsuariLoggejat() const { return usuariLoggejat; }

    // Casos de uso
    std::vector<DTOExperiencia> consultarNovetats();
    void esborrarUsuari(std::string contrasenya);

private:
    CapaDeDomini() {}
    std::shared_ptr<Usuari> usuariLoggejat;
};