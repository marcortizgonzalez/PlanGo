#pragma once
#include "CapaDeDomini.hxx"

class CapaDePresentacio {
public:
    // Obtiene la instancia única (Singleton) de la Capa de Presentación
    static CapaDePresentacio& getInstance() {
        static CapaDePresentacio instance;
        return instance;
    }

    // --- MÉTODOS DE MENÚS (Navegación) ---

    // Menú inicial para usuarios no logueados (Login, Registro, Consultas públicas)
    void inici();

    // Menú principal para usuarios logueados (Gestión cuenta, Reservas, etc.)
    void menuPrincipal();

    // Submenú para operaciones de perfil de usuario
    void menuGestioUsuaris();

    // Submenú para realizar y consultar reservas
    void menuGestioReserves();

    // Submenú para búsquedas y rankings
    void menuConsultes();

private:
    CapaDePresentacio() = default; // Constructor privado

    // --- OPERACIONES DE CASOS DE USO (UI) ---

    // 3.1 GESTIÓN SESIÓN Y USUARIOS
    void iniciarSessio();     // Pide credenciales e inicia sesión
    void tancarSessio();      // Cierra la sesión actual
    void registrarUsuari();   // Formulario de registro de usuario
    void consultarUsuari();   // Muestra datos del usuario actual
    void modificarUsuari();   // Formulario para editar perfil
    void esborrarUsuari();    // Pide confirmación y borra cuenta

    // 3.2 GESTIÓN RESERVAS
    void reservarEscapada();  // Flujo visual para reservar escapada
    void reservarActivitat(); // Flujo visual para reservar actividad
    void consultarReserves(); // Muestra lista de reservas y totales

    // 3.3 CONSULTAS
    void consultarExperiencies(); // Buscador por filtros (ciudad/plazas)
    void consultaNovetats();      // Muestra las últimas experiencias
    void consultarMesReservades(); // Muestra ranking Top 5

    // --- UTILIDADES ---
    void pausa(); // Espera a que el usuario pulse Intro
};