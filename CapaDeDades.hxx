#pragma once
#include <memory>
#include <vector>
#include <odb/database.hxx>
#include "Experiencia.hxx"
#include "Usuari.hxx"
#include "Reserva.hxx"

class CapaDeDades {
public:
    // Obtiene la instancia única de la clase (Patrón Singleton)
    static CapaDeDades& getInstance();

    // --- CONSULTAS AVANZADAS ---

    // Busca experiencias por ciudad y plazas disponibles (filtro)
    std::vector<std::shared_ptr<Experiencia>> cercarExperiencies(std::string ciutat, int places);

    // Obtiene experiencias ordenadas por popularidad (Ranking)
    std::vector<std::shared_ptr<Experiencia>> experienciesMesReservades();

    // --- MÉTODOS GENERALES ---

    // Recupera todas las experiencias de la base de datos
    std::vector<std::shared_ptr<Experiencia>> totesExperiencies();

    // --- BÚSQUEDAS ---

    // Busca un usuario por su apodo (Primary Key)
    std::shared_ptr<Usuari> obtenirUsuari(std::string sobrenom);

    // Busca un usuario por su correo (para validar duplicados)
    std::shared_ptr<Usuari> obtenirUsuariPerCorreu(std::string correu);

    // Busca una experiencia por su nombre
    std::shared_ptr<Experiencia> obtenirExperiencia(std::string nom);

    // --- RESERVAS ---

    // Obtiene la lista de reservas de un usuario específico
    std::vector<std::shared_ptr<Reserva>> obtenirReservesUsuari(std::shared_ptr<Usuari> u);

    // --- MODIFICACIONES (INSERT/UPDATE/DELETE) ---

    // Guarda un nuevo usuario en la BD
    void insertaUsuari(std::shared_ptr<Usuari> u);

    // Actualiza los datos de un usuario existente
    void modificaUsuari(std::shared_ptr<Usuari> u);

    // Guarda una nueva reserva en la BD
    void insertaReserva(std::shared_ptr<Reserva> r);

    // Actualiza una experiencia (ej. contador de reservas)
    void actualitzaExperiencia(std::shared_ptr<Experiencia> e);

    // Elimina una reserva de la BD
    void esborrarReserva(std::shared_ptr<Reserva> r);

    // Elimina un usuario de la BD
    void esborrarUsuari(std::shared_ptr<Usuari> u);

private:
    CapaDeDades();
    std::shared_ptr<odb::database> db;
};