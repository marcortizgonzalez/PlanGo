#include "CapaDePresentacio.hxx"
#include "CapaDeDomini.hxx"
#include <iostream>
#include <limits> 
#include <string>
#include <iomanip>

using namespace std;

// Pausa la ejecución hasta que el usuario pulse Intro
void CapaDePresentacio::pausa() {
    cout << "Prem Intro per continuar...";
    string dummy;
    getline(cin, dummy);
}

// Menú inicial (Bucle principal para usuario no logueado)
void CapaDePresentacio::inici() {
    int opcio = -1;
    while (opcio != 0) {
        cout << "\n===== MENU PRINCIPAL (No Loggejat) =====" << endl;
        cout << "1. Iniciar sessio" << endl;
        cout << "2. Registrar usuari" << endl;
        cout << "3. Consultes" << endl;
        cout << "0. Sortir" << endl;
        cout << "Opcio: "; cin >> opcio; cin.ignore();
        switch (opcio) {
        case 1: iniciarSessio(); if (CapaDeDomini::getInstance().getUsuariLoggejat()) menuPrincipal(); break;
        case 2: registrarUsuari(); break;
        case 3: menuConsultes(); break;
        case 0: cout << "Fins aviat!" << endl; break;
        default: cout << "Opcio no valida." << endl; break;
        }
    }
}

// Menú principal (Bucle para usuario logueado)
void CapaDePresentacio::menuPrincipal() {
    int opcio = -1;
    while (opcio != 0 && CapaDeDomini::getInstance().getUsuariLoggejat()) {
        cout << "\n===== MENU PRINCIPAL (Loggejat) =====" << endl;
        cout << "1. Gestio usuaris" << endl;
        cout << "2. Gestio reserves" << endl;
        cout << "3. Consultes" << endl;
        cout << "4. Tancar sessio" << endl;
        cout << "0. Sortir" << endl;
        cout << "Opcio: "; cin >> opcio; cin.ignore();
        switch (opcio) {
        case 1: menuGestioUsuaris(); break;
        case 2: menuGestioReserves(); break;
        case 3: menuConsultes(); break;
        case 4: tancarSessio(); break;
        case 0: cout << "Tancant..." << endl; exit(0); break;
        default: cout << "Opcio no valida." << endl; break;
        }
    }
}

// Submenú de gestión de perfil de usuario
void CapaDePresentacio::menuGestioUsuaris() {
    int opcio = -1;
    while (opcio != 0) {
        cout << "\n--- GESTIO USUARI ---" << endl;
        cout << "1. Consulta usuari" << endl;
        cout << "2. Modifica usuari" << endl;
        cout << "3. Esborra usuari" << endl;
        cout << "4. Tancar sessio" << endl;
        cout << "0. Tornar" << endl;
        cout << "Opcio: "; cin >> opcio; cin.ignore();
        switch (opcio) {
        case 1: consultarUsuari(); break;
        case 2: modificarUsuari(); break;
        case 3: esborrarUsuari(); if (!CapaDeDomini::getInstance().getUsuariLoggejat()) return; break;
        case 4: tancarSessio(); return;
        case 0: break;
        default: cout << "Opcio no valida." << endl; break;
        }
    }
}

// Submenú de gestión de reservas
void CapaDePresentacio::menuGestioReserves() {
    int opcio = -1;
    while (opcio != 0) {
        cout << "\n--- GESTIO RESERVES ---" << endl;
        cout << "1. Reservar escapada" << endl;
        cout << "2. Reservar activitat" << endl;
        cout << "3. Visualitzar reserves" << endl;
        cout << "0. Tornar" << endl;
        cout << "Opcio: "; cin >> opcio; cin.ignore();
        switch (opcio) {
        case 1: reservarEscapada(); break;
        case 2: reservarActivitat(); break;
        case 3: consultarReserves(); break;
        case 0: break;
        default: cout << "Opcio no valida." << endl; break;
        }
    }
}

// Submenú de consultas generales
void CapaDePresentacio::menuConsultes() {
    int opcio = -1;
    while (opcio != 0) {
        cout << "\n--- CONSULTES ---" << endl;
        cout << "1. Consultar experiencies" << endl;
        cout << "2. Consultar novetats" << endl;
        cout << "3. Consultar mes reservades" << endl;
        cout << "0. Tornar" << endl;
        cout << "Opcio: "; cin >> opcio; cin.ignore();
        switch (opcio) {
        case 1: consultarExperiencies(); break;
        case 2: consultaNovetats(); break;
        case 3: consultarMesReservades(); break;
        case 0: break;
        default: cout << "Opcio no valida." << endl; break;
        }
    }
}

// Solicita credenciales y llama al dominio para login
void CapaDePresentacio::iniciarSessio() {
    string user, pwd;
    cout << "\n** Inici sessio **" << endl;
    cout << "Sobrenom: "; cin >> user; cin.ignore();
    cout << "Contrasenya: "; cin >> pwd; cin.ignore();
    try { CapaDeDomini::getInstance().iniciarSessio(user, pwd); cout << "Sessio iniciada correctament!" << endl; }
    catch (exception& e) { cout << "Error: " << e.what() << endl; }
    pausa();
}

// Cierra la sesión del usuario actual
void CapaDePresentacio::tancarSessio() {
    string confirm;
    cout << "\n** Tancar sessio **" << endl;
    cout << "Vols tancar la sessio (S/N): "; cin >> confirm; cin.ignore();
    if (confirm == "S" || confirm == "s") { CapaDeDomini::getInstance().tancarSessio(); cout << "Sessio tancada correctament!" << endl; }
}

// Formulario de registro de nuevo usuario
void CapaDePresentacio::registrarUsuari() {
    string nom, user, mail, pass; int edat;
    cout << "\n--- REGISTRAR NOU USUARI ---" << endl;
    cout << "Nom complet: "; getline(cin, nom);
    cout << "Sobrenom (ID): "; cin >> user;
    cout << "Correu electronic: "; cin >> mail;
    cout << "Contrasenya: "; cin >> pass;
    cout << "Edat: "; cin >> edat; cin.ignore();
    try { CapaDeDomini::getInstance().registrarUsuari(nom, user, mail, pass, edat); cout << "Usuari registrat correctament!" << endl; }
    catch (exception& e) { cout << "Error: " << e.what() << endl; }
    pausa();
}

// Muestra los datos del perfil del usuario logueado
void CapaDePresentacio::consultarUsuari() {
    try {
        DTOUsuari dto = CapaDeDomini::getInstance().consultarUsuari();
        cout << "\n** Consulta usuari **" << endl;
        cout << "Nom complet: " << dto.obteNom() << endl;
        cout << "Sobrenom: " << dto.obteSobrenom() << endl;
        cout << "Correu: " << dto.obteCorreu() << endl;
        cout << "Edat: " << dto.obteEdat() << " anys" << endl;
        cout << "Nombre total d'experiencies reservades: " << dto.obteNumReserves() << endl;
    }
    catch (exception& e) { cout << "Error: " << e.what() << endl; }
    pausa();
}

// Formulario para modificar datos del usuario (Nombre, Correo, Edad)
void CapaDePresentacio::modificarUsuari() {
    try {
        DTOUsuari actual = CapaDeDomini::getInstance().consultarUsuari();
        cout << "\n** Modifica usuari (" << actual.obteSobrenom() << ") **" << endl;
        cout << "Dades actuals:" << endl;
        cout << "Nom: " << actual.obteNom() << endl;
        cout << "Correu: " << actual.obteCorreu() << endl;
        cout << "Edat: " << actual.obteEdat() << endl;
        cout << "\n(Deixa en blanc i prem Enter si no vols canviar el valor)" << endl;
        string buff, nouNom, nouCorreu; int novaEdat = -1;
        cout << "Nom complet [" << actual.obteNom() << "]: "; getline(cin, nouNom);
        cout << "Correu electronic [" << actual.obteCorreu() << "]: "; getline(cin, nouCorreu);
        cout << "Edat [" << actual.obteEdat() << "]: "; getline(cin, buff);
        if (!buff.empty()) try { novaEdat = stoi(buff); }
        catch (...) { cout << "Format d'edat incorrecte." << endl; }
        CapaDeDomini::getInstance().modificarUsuari(nouNom, nouCorreu, novaEdat);
        cout << "\nModificacio realitzada amb exit. Dades actualitzades (BD):" << endl;
        consultarUsuari(); return;
    }
    catch (exception& e) { cout << "Error: " << e.what() << endl; }
    pausa();
}

// Elimina el usuario actual tras confirmar contraseña
void CapaDePresentacio::esborrarUsuari() {
    string password, confirmacio;
    cout << "\n** Esborrar usuari **" << endl;
    cout << "Per confirmar has d'entrar la contrasenya: "; cin >> password; cin.ignore();
    try {
        cout << "Vols confirmar l'esborrat? (S/N): "; cin >> confirmacio; cin.ignore();
        if (confirmacio == "S" || confirmacio == "s") { CapaDeDomini::getInstance().esborrarUsuari(password); cout << "Usuari esborrat correctament!" << endl; }
    }
    catch (const std::exception& e) { cout << "Error: " << e.what() << endl; }
    pausa();
}

// Flujo visual para reservar una Escapada
void CapaDePresentacio::reservarEscapada() {
    try {
        cout << "\n** Reservar escapada **" << endl;
        string nom, confirm;

        // 1. Pedir nombre
        cout << "Nom: ";
        getline(cin, nom);

        // 2. Mostrar info y validar tipo
        DTOExperiencia dto = CapaDeDomini::getInstance().obtenirDadesExperiencia(nom);

        if (dto.obteTipus() == "ACTIVITAT") throw runtime_error("Aixo es una activitat.");

        cout << "Descripcio: " << dto.obteDescripcio() << endl;
        cout << "Ciutat: " << dto.obteCiutat() << endl;
        cout << "Nombre de places: " << dto.obteMaximPlaces() << endl;
        cout << dto.obteDetalls() << endl; // Hotel y Nits
        cout << "Preu: " << dto.obtePreu() << " eur" << endl;

        // 3. Confirmar
        cout << "\nVols confirmar la reserva? (S/N): ";
        cin >> confirm;
        cin.ignore();

        if (confirm == "S" || confirm == "s") {
            CapaDeDomini::getInstance().reservarEscapada(nom);
            cout << "Reserva registrada correctament." << endl;
            cout << "Preu aplicat a la reserva: " << dto.obtePreu() << " eur" << endl;
        }
        else {
            cout << "Reserva cancel lada." << endl;
        }
    }
    catch (const std::exception& e) {
        cout << "Error: " << e.what() << endl;
    }
    pausa();
}

// Flujo visual para reservar una Actividad
// Bucle de reintento si se supera el aforo
void CapaDePresentacio::reservarActivitat() {
    try {
        cout << "\n** Reservar activitat **" << endl;
        string nom, confirm;
        int persones;

        // 1. Pedir nombre
        cout << "Nom: ";
        getline(cin, nom);

        // 2. Mostrar info y validar tipo
        DTOExperiencia dto = CapaDeDomini::getInstance().obtenirDadesExperiencia(nom);

        if (dto.obteTipus() != "ACTIVITAT") throw runtime_error("Aixo no es una activitat.");

        cout << "Descripcio: " << dto.obteDescripcio() << endl;
        cout << "Ciutat: " << dto.obteCiutat() << endl;
        cout << "Nombre maxim de places: " << dto.obteMaximPlaces() << endl;
        cout << dto.obteDetalls() << endl; // Durada
        cout << "Preu per persona: " << dto.obtePreu() << " eur" << endl;

        // --- INICIO BUCLE DE REINTENTO ---
        bool intentar = true;
        while (intentar) {
            // 3. Pedir personas
            cout << "\nNombre de persones: ";
            cin >> persones;
            cin.ignore();

            // 4. Mostrar precio total calculado
            float total = CapaDeDomini::getInstance().calcularPreuReserva(nom, persones);
            cout << "Preu total de la reserva: " << total << " eur" << endl;

            // 5. Confirmar
            cout << "Vols continuar amb la reserva? (S/N): ";
            cin >> confirm;
            cin.ignore();

            if (confirm == "S" || confirm == "s") {
                try {
                    // Intentamos realizar la reserva
                    CapaDeDomini::getInstance().reservarActivitat(nom, persones);
                    cout << "Reserva registrada correctament." << endl;
                    intentar = false; // Éxito, salimos del bucle
                }
                catch (const std::exception& e) {
                    // ERROR: Aforo superado
                    // La excepción 'e' ya contiene el mensaje con las plazas disponibles (viene de CapaDeDomini)
                    cout << "\nERROR: " << e.what() << endl;

                    // Preguntamos si quiere reintentar o salir
                    string opcio;
                    cout << "Vols entrar un numero alternatiu? (S/N) (N per finalitzar): ";
                    cin >> opcio;
                    cin.ignore();

                    if (opcio != "S" && opcio != "s") {
                        cout << "Operacio finalitzada per l'usuari." << endl;
                        intentar = false; // Salimos del bucle sin reservar
                    }
                    // Si es 'S', el bucle se repite y vuelve a pedir "Nombre de persones"
                }
            }
            else {
                cout << "Reserva cancel lada." << endl;
                intentar = false;
            }
        }
        // --- FIN BUCLE DE REINTENTO ---
    }
    catch (const std::exception& e) {
        cout << "Error: " << e.what() << endl;
    }
    pausa();
}

// Lista las reservas del usuario y muestra totales
void CapaDePresentacio::consultarReserves() {
    try {
        auto llista = CapaDeDomini::getInstance().consultarReserves();

        cout << "\n===== LES MEVES RESERVES =====" << endl;

        if (llista.empty()) {
            cout << "No tens cap reserva." << endl;
        }
        else {
            // Cálculo de totales
            float totalGlobal = 0.0f;
            float totalEscapades = 0.0f;
            float totalActivitats = 0.0f;

            for (const auto& r : llista) {
                float preu = r.obtePreuPagat();
                totalGlobal += preu;
                if (r.obteExperiencia().obteTipus() == "ACTIVITAT") totalActivitats += preu;
                else totalEscapades += preu;
            }

            // TOTAL GLOBAL
            cout << ">>> PREU TOTAL PAGAT: " << std::fixed << std::setprecision(2) << totalGlobal << " eur" << endl << endl;

            // LISTA ESCAPADES
            cout << "--- ESCAPADES ---" << endl;
            bool hiHaEscapades = false;
            for (const auto& r : llista) {
                const auto& exp = r.obteExperiencia();
                if (exp.obteTipus() != "ACTIVITAT") {
                    hiHaEscapades = true;
                    cout << "Data: " << r.obteData() << " | Places: " << r.obteNumPlaces()
                        << " | Preu: " << r.obtePreuPagat() << " eur" << endl;
                    cout << "   " << exp.obteNom() << " (" << exp.obteCiutat() << ")" << endl;
                    cout << "   " << exp.obteDetalls() << endl;
                    cout << "--------------------------------" << endl;
                }
            }
            if (!hiHaEscapades) cout << "(Cap escapada reservada)" << endl;
            cout << "TOTAL ESCAPADES: " << totalEscapades << " eur" << endl << endl;

            // LISTA ACTIVIDADES
            cout << "--- ACTIVITATS ---" << endl;
            bool hiHaActivitats = false;
            for (const auto& r : llista) {
                const auto& exp = r.obteExperiencia();
                if (exp.obteTipus() == "ACTIVITAT") {
                    hiHaActivitats = true;
                    cout << "Data: " << r.obteData() << " | Places: " << r.obteNumPlaces()
                        << " | Preu: " << r.obtePreuPagat() << " eur" << endl;
                    cout << "   " << exp.obteNom() << " (" << exp.obteCiutat() << ")" << endl;
                    cout << "   " << exp.obteDetalls() << endl;
                    cout << "--------------------------------" << endl;
                }
            }
            if (!hiHaActivitats) cout << "(Cap activitat reservada)" << endl;
            cout << "TOTAL ACTIVITATS: " << totalActivitats << " eur" << endl;
        }
    }
    catch (const std::exception& e) {
        cout << "Error: " << e.what() << endl;
    }
    pausa();
}

// Búsqueda de experiencias por ciudad y plazas
void CapaDePresentacio::consultarExperiencies() {
    try {
        cout << "\n--- CERCAR EXPERIENCIES ---" << endl;
        string ciutat;
        int persones;

        cout << "Ciutat: ";
        getline(cin, ciutat);

        cout << "Nombre de persones: ";
        cin >> persones;
        cin.ignore();

        auto llista = CapaDeDomini::getInstance().consultarExperiencies(ciutat, persones);

        if (llista.empty()) {
            cout << "No s'han trobat experiencies." << endl;
        }
        else {
            cout << "\nResultats:" << endl;
            for (const auto& dto : llista) {
                cout << "****************************************" << endl;
                cout << " NOM: " << dto.obteNom() << endl;
                cout << " TIPUS: " << dto.obteTipus() << endl;
                cout << " DESCRIPCIO: " << dto.obteDescripcio() << endl;
                cout << " CIUTAT: " << dto.obteCiutat() << endl;
                cout << " PREU: " << dto.obtePreu() << " eur" << endl;
                cout << " PLACES MAX: " << dto.obteMaximPlaces() << endl;
                cout << " DETALLS: " << dto.obteDetalls() << endl;
                cout << "****************************************" << endl;
            }
        }
    }
    catch (const std::exception& e) {
        cout << "Error: " << e.what() << endl;
    }
    pausa();
}

// Muestra las 10 últimas experiencias añadidas
void CapaDePresentacio::consultaNovetats() {
    try {
        auto novetats = CapaDeDomini::getInstance().consultarNovetats();
        cout << "\n--- NOVETATS (Ultimes 10) ---" << endl;
        if (novetats.empty()) cout << "Cap experiencia disponible." << endl;

        for (const auto& dto : novetats) {
            cout << "Nom: " << dto.obteNom() << endl;
            cout << "Descripcio: " << dto.obteDescripcio() << endl;
            cout << "Ciutat: " << dto.obteCiutat() << endl;
            cout << "Places: " << dto.obteMaximPlaces() << endl;
            cout << "Preu: " << dto.obtePreu() << endl;
            cout << "Detalls: " << dto.obteDetalls() << endl;
            cout << "Tipus: " << dto.obteTipus() << endl;
            cout << "-----------------" << endl;
        }
    }
    catch (const std::exception& e) {
        cout << "Error: " << e.what() << endl;
    }
    pausa();
}

// Muestra el Top 5 de Escapadas y Actividades más reservadas
void CapaDePresentacio::consultarMesReservades() {
    try {
        auto llista = CapaDeDomini::getInstance().consultarMesReservades();
        cout << "\n--- TOP 5 MES RESERVADES ---" << endl;

        if (llista.empty()) {
            cout << "Encara no hi ha reserves." << endl;
        }
        else {
            // Top 5 Escapadas
            cout << "\n[ESCAPADES]" << endl;
            int count = 0;
            for (const auto& dto : llista) {
                if (dto.obteTipus() != "ACTIVITAT") {
                    count++;
                    cout << "#" << count << " " << dto.obteNom() << endl;
                    cout << "   " << dto.obteDescripcio() << ", " << dto.obteCiutat() << endl;
                    cout << "   Places: " << dto.obteMaximPlaces() << ", Preu: " << dto.obtePreu() << endl;
                    cout << "   " << dto.obteDetalls() << endl;
                    if (count == 5) break; // Límite 5
                }
            }
            if (count == 0) cout << "Cap." << endl;

            // Top 5 Actividades
            cout << "\n[ACTIVITATS]" << endl;
            count = 0;
            for (const auto& dto : llista) {
                if (dto.obteTipus() == "ACTIVITAT") {
                    count++;
                    cout << "#" << count << " " << dto.obteNom() << endl;
                    cout << "   " << dto.obteDescripcio() << ", " << dto.obteCiutat() << endl;
                    cout << "   Places: " << dto.obteMaximPlaces() << ", Preu: " << dto.obtePreu() << endl;
                    cout << "   " << dto.obteDetalls() << endl;
                    if (count == 5) break; // Límite 5
                }
            }
            if (count == 0) cout << "Cap." << endl;
        }
    }
    catch (const std::exception& e) {
        cout << "Error: " << e.what() << endl;
    }
    pausa();
}