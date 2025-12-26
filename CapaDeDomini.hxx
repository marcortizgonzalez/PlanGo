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

    // --- GESTIÓ SESSIÓ I USUARIS ---

    // [3.1.1] Valida credenciales e inicia sesión
    void iniciSessio(std::string u, std::string p);

    // [3.1.2] Cierra la sesión actual
    void tancaSessio();

    // Devuelve el usuario logueado
    std::shared_ptr<Usuari> getUsuariLoggejat() const { return usuariLoggejat; }

    // [3.1.3] Registra un nuevo usuario 
    void registraUsuari(std::string n, std::string s, std::string c, std::string p, int e);

    // [3.1.4] Consulta datos del usuario logueado 
    DTOUsuari consultaUsuari();

    // [3.1.5] Modifica datos del usuario
    void modificaUsuari(std::string n, std::string c, int e);

    // [3.1.6] Borra usuario y sus reservas 
    void esborraUsuari(std::string p);

    // --- GESTIÓ RESERVES ---

    // [3.2.1] Consulta específica para Escapadas
    DTOExperiencia consultaEscapada(std::string nom);

    // [3.2.2] Consulta específica para Actividades
    DTOExperiencia consultaActivitat(std::string nom);

    // Calcula precio provisional 
    float calculaPreu(std::string nom, int persones);

    // [3.2.1] Reserva escapada 
    void reservaEscapada(std::string nom);

    // [3.2.2] Reserva actividad 
    void reservaActivitat(std::string nom, int persones);

    // [3.2.3] Consulta historial reservas 
    std::vector<DTOReserva> consultaReserves();

    // --- CONSULTES ---

    // [3.3.2] Novedades
    std::vector<DTOExperiencia> consultarNovetats();

    // [3.3.1] Buscar experiencias 
    std::vector<DTOExperiencia> consultaExperiencies(std::string ciutat, int places);

    // [3.3.3] Ranking 
    std::vector<DTOExperiencia> consultaMesReservades();

private:
    CapaDeDomini() {}
    std::shared_ptr<Usuari> usuariLoggejat;
    void _processarReserva(std::shared_ptr<Experiencia> exp, int numPlaces);
    std::string _obteDataActual();
};