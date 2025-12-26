#include "CapaDePresentacio.hxx"
#include "CapaDeDomini.hxx"
#include <iostream>
#include <limits> 
#include <string>
#include <iomanip> // Imprescindible per 'setprecision'

using namespace std;

// --- Helpers ---

// Pausa la ejecución hasta que el usuario pulse Intro
void CapaDePresentacio::pausa() {
    cout << "\nPrem Intro per continuar...";
    string dummy;
    getline(cin, dummy);
}

// --- MENÚS ---

// Menú inicial para usuarios no logueados (Login, Registro, Consultas)
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

// Menú principal para usuarios logueados (Gestión de cuenta, Reservas, Consultas)
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

// Submenú para operaciones sobre el perfil del usuario (Consultar, Modificar, Borrar)
void CapaDePresentacio::menuGestioUsuaris() {
    int opcio = -1;
    while (opcio != 0) {
        cout << "\n--- GESTIO USUARIS ---" << endl;
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

// Submenú para gestionar reservas (Crear y Visualizar)
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

// Submenú para realizar búsquedas y ver rankings
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

// --- GESTIÓ USUARIS ---

// Pide credenciales y llama al dominio para iniciar sesión
void CapaDePresentacio::iniciarSessio() {
    string user, pwd;
    cout << "\n** Inici sessio **" << endl;
    cout << "Sobrenom: "; cin >> user; cin.ignore();
    cout << "Contrasenya: "; cin >> pwd; cin.ignore();
    try { CapaDeDomini::getInstance().iniciarSessio(user, pwd); cout << "Sessio iniciada correctament!" << endl; }
    catch (exception& e) { cout << "Error: " << e.what() << endl; }
    pausa();
}

// Pide confirmación y cierra la sesión actual
void CapaDePresentacio::tancarSessio() {
    string confirm;
    cout << "\n** Tancar sessio **" << endl;
    cout << "Vols tancar la sessio (S/N): "; cin >> confirm; cin.ignore();
    if (confirm == "S" || confirm == "s") { CapaDeDomini::getInstance().tancarSessio(); cout << "Sessio tancada correctament!" << endl; }
}

// Recoge datos personales y registra un nuevo usuario en el sistema
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

// Pide nuevos datos (opcionalmente) y actualiza el perfil del usuario
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

// Pide contraseña y confirmación para borrar la cuenta del usuario
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

// --- GESTIÓ RESERVES ---

// Flujo para reservar Escapada: Nombre -> Info -> Confirmar -> Reservar (Plazas Auto)
void CapaDePresentacio::reservarEscapada() {
    try {
        cout << "\n** Reservar escapada **" << endl;
        string nom, confirm;
        cout << "Nom: "; getline(cin, nom);
        DTOExperiencia dto = CapaDeDomini::getInstance().obtenirDadesExperiencia(nom);

        if (dto.obteTipus() == "ACTIVITAT") throw runtime_error("Aixo es una activitat.");

        // INFORMACIÓ ABANS DE CONFIRMAR
        cout << "\n--- DETALLS ---" << endl;
        cout << "Descripcio: " << dto.obteDescripcio() << endl;
        cout << "Ciutat: " << dto.obteCiutat() << endl;
        cout << "Places totals: " << dto.obteMaximPlaces() << endl;
        cout << dto.obteDetalls() << endl;
        cout << "Preu total: " << dto.obtePreu() << " eur" << endl;

        cout << "\nVols confirmar la reserva? (S/N): "; cin >> confirm; cin.ignore();
        if (confirm == "S" || confirm == "s") {
            CapaDeDomini::getInstance().reservarEscapada(nom);
            cout << "Reserva registrada correctament." << endl;
            cout << "Preu final aplicat: " << dto.obtePreu() << " eur" << endl;
        }
        else { cout << "Reserva cancel·lada." << endl; }
    }
    catch (const std::exception& e) { cout << "Error: " << e.what() << endl; }
    pausa();
}

// Flujo para reservar Actividad: Nombre -> Info -> Personas -> Precio -> Confirmar -> Reservar
void CapaDePresentacio::reservarActivitat() {
    try {
        cout << "\n** Reservar activitat **" << endl;
        string nom, confirm; int persones;
        cout << "Nom: "; getline(cin, nom);
        DTOExperiencia dto = CapaDeDomini::getInstance().obtenirDadesExperiencia(nom);

        if (dto.obteTipus() != "ACTIVITAT") throw runtime_error("Aixo no es una activitat.");

        // INFORMACIÓ
        cout << "\n--- DETALLS ---" << endl;
        cout << "Descripcio: " << dto.obteDescripcio() << endl;
        cout << "Ciutat: " << dto.obteCiutat() << endl;
        cout << "Places max: " << dto.obteMaximPlaces() << endl;
        cout << dto.obteDetalls() << endl;
        cout << "Preu per persona: " << dto.obtePreu() << " eur" << endl;

        cout << "\nNombre de persones: "; cin >> persones; cin.ignore();
        float total = CapaDeDomini::getInstance().calcularPreuReserva(nom, persones);

        cout << "Preu total de la reserva: " << total << " eur" << endl;
        cout << "Vols continuar amb la reserva? (S/N): "; cin >> confirm; cin.ignore();
        if (confirm == "S" || confirm == "s") {
            CapaDeDomini::getInstance().reservarActivitat(nom, persones);
            cout << "Reserva registrada correctament." << endl;
        }
        else { cout << "Reserva cancel·lada." << endl; }
    }
    catch (const std::exception& e) { cout << "Error: " << e.what() << endl; }
    pausa();
}

// --- CONSULTES ---

// Lista reservas del usuario con subtotales por tipo y total global
void CapaDePresentacio::consultarReserves() {
    try {
        auto llista = CapaDeDomini::getInstance().consultarReserves();

        cout << "\n===== LES MEVES RESERVES =====" << endl;

        if (llista.empty()) {
            cout << "No tens cap reserva." << endl;
        }
        else {
            // Variables per totals
            float totalGlobal = 0.0f;
            float totalEscapades = 0.0f;
            float totalActivitats = 0.0f;

            // Calcular primer els totals
            for (const auto& r : llista) {
                float p = r.obtePreuPagat();
                totalGlobal += p;
                if (r.obteExperiencia().obteTipus() == "ACTIVITAT") totalActivitats += p;
                else totalEscapades += p;
            }

            cout << "TOTAL PAGAT GLOBAL: " << std::fixed << std::setprecision(2) << totalGlobal << " eur" << endl << endl;

            // LLISTA 1: ESCAPADES
            cout << "--- ESCAPADES ---" << endl;
            bool hayEsc = false;
            for (const auto& r : llista) {
                const auto& exp = r.obteExperiencia();
                if (exp.obteTipus() != "ACTIVITAT") {
                    hayEsc = true;
                    cout << "Data: " << r.obteData() << " | Places: " << r.obteNumPlaces()
                        << " | Preu: " << r.obtePreuPagat() << " eur" << endl;
                    cout << "   " << exp.obteNom() << " (" << exp.obteCiutat() << ")" << endl;
                    cout << "   " << exp.obteDetalls() << endl;
                    cout << "   ----------------" << endl;
                }
            }
            if (!hayEsc) cout << "(Cap escapada)" << endl;
            cout << ">>> TOTAL ESCAPADES: " << totalEscapades << " eur" << endl << endl;

            // LLISTA 2: ACTIVITATS
            cout << "--- ACTIVITATS ---" << endl;
            bool hayAct = false;
            for (const auto& r : llista) {
                const auto& exp = r.obteExperiencia();
                if (exp.obteTipus() == "ACTIVITAT") {
                    hayAct = true;
                    cout << "Data: " << r.obteData() << " | Places: " << r.obteNumPlaces()
                        << " | Preu: " << r.obtePreuPagat() << " eur" << endl;
                    cout << "   " << exp.obteNom() << " (" << exp.obteCiutat() << ")" << endl;
                    cout << "   " << exp.obteDetalls() << endl;
                    cout << "   ----------------" << endl;
                }
            }
            if (!hayAct) cout << "(Cap activitat)" << endl;
            cout << ">>> TOTAL ACTIVITATS: " << totalActivitats << " eur" << endl;
        }
    }
    catch (const std::exception& e) { cout << "Error: " << e.what() << endl; }
    pausa();
}

// Busca experiencias por ciudad/plazas y muestra resultados en bloques separados
void CapaDePresentacio::consultarExperiencies() {
    try {
        cout << "\n--- CERCAR EXPERIENCIES ---" << endl;
        string ciutat; int persones;
        cout << "Ciutat: "; getline(cin, ciutat);
        cout << "Nombre de persones: "; cin >> persones; cin.ignore();

        auto llista = CapaDeDomini::getInstance().consultarExperiencies(ciutat, persones);

        if (llista.empty()) {
            cout << "No s'han trobat experiencies." << endl;
        }
        else {
            // SEPARAR LLISTES I FORMAT VISUAL
            cout << "\n=== ESCAPADES TROBADES ===" << endl;
            for (const auto& dto : llista) {
                if (dto.obteTipus() != "ACTIVITAT") {
                    cout << "+----------------------------------------+" << endl;
                    cout << "| NOM: " << dto.obteNom() << endl;
                    cout << "| DESC: " << dto.obteDescripcio() << endl;
                    cout << "| PREU: " << dto.obtePreu() << " eur  | MAX PLACES: " << dto.obteMaximPlaces() << endl;
                    cout << "| DETALLS: " << dto.obteDetalls() << endl;
                    cout << "+----------------------------------------+" << endl;
                }
            }

            cout << "\n=== ACTIVITATS TROBADES ===" << endl;
            for (const auto& dto : llista) {
                if (dto.obteTipus() == "ACTIVITAT") {
                    cout << "+----------------------------------------+" << endl;
                    cout << "| NOM: " << dto.obteNom() << endl;
                    cout << "| DESC: " << dto.obteDescripcio() << endl;
                    cout << "| PREU: " << dto.obtePreu() << " eur  | MAX PLACES: " << dto.obteMaximPlaces() << endl;
                    cout << "| DETALLS: " << dto.obteDetalls() << endl;
                    cout << "+----------------------------------------+" << endl;
                }
            }
        }
    }
    catch (const std::exception& e) { cout << "Error: " << e.what() << endl; }
    pausa();
}

// Muestra las 10 últimas experiencias añadidas
void CapaDePresentacio::consultaNovetats() {
    try {
        auto novetats = CapaDeDomini::getInstance().consultarNovetats();
        cout << "\n--- NOVETATS (Ultimes 10) ---" << endl;
        for (const auto& dto : novetats) {
            cout << "[" << dto.obteTipus() << "] " << dto.obteNom()
                << " (" << dto.obteCiutat() << ") - " << dto.obtePreu() << " eur" << endl;
            cout << "   " << dto.obteDetalls() << endl;
        }
    }
    catch (const std::exception& e) { cout << "Error: " << e.what() << endl; }
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
            cout << "\n[ESCAPADES]" << endl;
            int count = 0;
            for (const auto& dto : llista) {
                if (dto.obteTipus() != "ACTIVITAT") {
                    count++;
                    cout << "#" << count << " " << dto.obteNom() << " (" << dto.obteNumReserves() << " reserves)" << endl;
                    cout << "   " << dto.obteDescripcio() << endl;
                    if (count == 5) break;
                }
            }
            if (count == 0) cout << "Cap." << endl;

            cout << "\n[ACTIVITATS]" << endl;
            count = 0;
            for (const auto& dto : llista) {
                if (dto.obteTipus() == "ACTIVITAT") {
                    count++;
                    cout << "#" << count << " " << dto.obteNom() << endl;
                    cout << "   " << dto.obteDescripcio() << endl;
                    if (count == 5) break;
                }
            }
            if (count == 0) cout << "Cap." << endl;
        }
    }
    catch (const std::exception& e) { cout << "Error: " << e.what() << endl; }
    pausa();
}