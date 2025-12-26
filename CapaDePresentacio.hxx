#pragma once
#include "CapaDeDomini.hxx"

class CapaDePresentacio {
public:
    static CapaDePresentacio& getInstance() {
        static CapaDePresentacio instance;
        return instance;
    }

    void inici();
    void menuPrincipal();
    void menuGestioUsuaris();
    void menuGestioReserves();
    void menuConsultes();

private:
    CapaDePresentacio() = default;

    // --- OPERACIONES ---
    void iniciSessio();
    void tancaSessio();
    void registraUsuari();
    void consultaUsuari();
    void modificaUsuari();
    void esborraUsuari();

    void reservaEscapada();
    void reservaActivitat();
    void consultaReserves();

    void consultaExperiencies();
    void consultarNovetats();
    void consultaMesReservades();

    void pausa();
};