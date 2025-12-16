#pragma once
#include "CapaDeDomini.hxx"

class CapaDePresentacio {
public:
    // Patrón Singleton
    static CapaDePresentacio& getInstance() {
        static CapaDePresentacio instance;
        return instance;
    }

    // --- MÉTODOS DE MENÚS (Gestión de flujo) ---
    void inici();             // Menú inicial (Sin loggear) [cite: 94-97]
    void menuPrincipal();     // Menú principal (Loggeado) [cite: 98-103]

    // Submenús [cite: 109]
    void menuGestioUsuaris();
    void menuGestioReserves();
    void menuConsultes();

private:
    CapaDePresentacio() = default;

    // --- OPERACIONES DE CASOS DE ÚS (Sección 3) ---

    // 3.1 Gestió sessió i usuaris
    void iniciarSessio();     // 3.1.1
    void tancarSessio();      // 3.1.2
    void registrarUsuari();   // 3.1.3
    void consultarUsuari();   // 3.1.4
    void modificarUsuari();   // 3.1.5
    void esborrarUsuari();    // 3.1.6 (Ya lo tenías)

    // 3.2 Gestió reserves
    void reservarEscapada();  // 3.2.1
    void reservarActivitat(); // 3.2.2
    void consultarReserves(); // 3.2.3

    // 3.3 Consultes
    void consultarExperiencies();   // 3.3.1
    void consultaNovetats();        // 3.3.2 (Ya lo tenías)
    void consultarMesReservades();  // 3.3.3

    // Helpers
    void pausa(); // Para el "Prem Enter..."
};