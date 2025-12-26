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
    // Obtiene la instancia única (Singleton) del controlador de dominio
    static CapaDeDomini& getInstance();

    // --- GESTIÓN DE SESIÓN ---

    // Valida credenciales e inicia sesión
    void iniciarSessio(std::string u, std::string p);

    // Devuelve el puntero al usuario actual
    std::shared_ptr<Usuari> getUsuariLoggejat() const { return usuariLoggejat; }

    // Cierra la sesión actual
    void tancarSessio() { usuariLoggejat = nullptr; }

    // --- GESTIÓN DE USUARIOS (Bloque A) ---

    // Registra un nuevo usuario en el sistema
    void registrarUsuari(std::string n, std::string s, std::string c, std::string p, int e);

    // Consulta los datos del usuario logueado
    DTOUsuari consultarUsuari();

    // Modifica los datos del usuario logueado
    void modificarUsuari(std::string n, std::string c, int e);

    // Elimina el usuario actual y sus reservas
    void esborrarUsuari(std::string p);

    // --- GESTIÓN DE RESERVAS (Bloque B) ---

    // Obtiene datos de una experiencia para mostrar antes de reservar
    DTOExperiencia obtenirDadesExperiencia(std::string nom);

    // Calcula el precio provisional de una reserva (útil para actividades)
    float calcularPreuReserva(std::string nom, int persones);

    // Realiza la reserva de una escapada (plazas automáticas)
    void reservarEscapada(std::string nom);

    // Realiza la reserva de una actividad (plazas manuales)
    void reservarActivitat(std::string nom, int persones);

    // Consulta el historial de reservas del usuario
    std::vector<DTOReserva> consultarReserves();

    // --- CONSULTAS (Bloque C) ---

    // Obtiene las últimas experiencias añadidas
    std::vector<DTOExperiencia> consultarNovetats();

    // Busca experiencias por ciudad y plazas
    std::vector<DTOExperiencia> consultarExperiencies(std::string ciutat, int places);

    // Obtiene el ranking de experiencias más reservadas
    std::vector<DTOExperiencia> consultarMesReservades();

private:
    CapaDeDomini() {} // Constructor privado
    std::shared_ptr<Usuari> usuariLoggejat;

    // Métodos auxiliares internos
    void _processarReserva(std::shared_ptr<Experiencia> exp, int numPlaces);
    std::string _obteDataActual();
};