#include "CapaDePresentacio.hxx"
#include "CapaDeDomini.hxx"
#include <iostream>
#include <limits> 
#include <string>
#include <iomanip>

using namespace std;

void CapaDePresentacio::pausa() {
    cout << "Prem Intro per continuar...";
    string dummy;
    getline(cin, dummy);
}

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
        case 1: iniciSessio(); if (CapaDeDomini::getInstance().getUsuariLoggejat()) menuPrincipal(); break;
        case 2: registraUsuari(); break;
        case 3: menuConsultes(); break;
        case 0: cout << "Fins aviat!" << endl; break;
        default: cout << "Opcio no valida." << endl; break;
        }
    }
}

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
        case 4: tancaSessio(); break;
        case 0: cout << "Tancant..." << endl; exit(0); break;
        default: cout << "Opcio no valida." << endl; break;
        }
    }
}

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
        case 1: consultaUsuari(); break;
        case 2: modificaUsuari(); break;
        case 3: esborraUsuari(); if (!CapaDeDomini::getInstance().getUsuariLoggejat()) return; break;
        case 4: tancaSessio(); return;
        case 0: break;
        default: cout << "Opcio no valida." << endl; break;
        }
    }
}

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
        case 1: reservaEscapada(); break;
        case 2: reservaActivitat(); break;
        case 3: consultaReserves(); break;
        case 0: break;
        default: cout << "Opcio no valida." << endl; break;
        }
    }
}

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
        case 1: consultaExperiencies(); break;
        case 2: consultarNovetats(); break;
        case 3: consultaMesReservades(); break;
        case 0: break;
        default: cout << "Opcio no valida." << endl; break;
        }
    }
}

// [3.1.1] Inici Sessio
void CapaDePresentacio::iniciSessio() {
    string user, pwd;
    cout << "\n** Inici sessio **" << endl;
    cout << "Sobrenom: "; cin >> user; cin.ignore();
    cout << "Contrasenya: "; cin >> pwd; cin.ignore();
    try { CapaDeDomini::getInstance().iniciSessio(user, pwd); cout << "Sessio iniciada correctament!" << endl; }
    catch (exception& e) { cout << "Error: " << e.what() << endl; }
    pausa();
}

// [3.1.2] Tanca Sessio
void CapaDePresentacio::tancaSessio() {
    string confirm;
    cout << "\n** Tancar sessio **" << endl;
    cout << "Vols tancar la sessio (S/N): "; cin >> confirm; cin.ignore();
    if (confirm == "S" || confirm == "s") { CapaDeDomini::getInstance().tancaSessio(); cout << "Sessio tancada correctament!" << endl; }
}

// [3.1.3] Registra Usuari
void CapaDePresentacio::registraUsuari() {
    string nom, user, mail, pass; int edat;
    cout << "\n--- REGISTRAR NOU USUARI ---" << endl;
    cout << "Nom complet: "; getline(cin, nom);
    cout << "Sobrenom (ID): "; cin >> user;
    cout << "Correu electronic: "; cin >> mail;
    cout << "Contrasenya: "; cin >> pass;
    cout << "Edat: "; cin >> edat; cin.ignore();
    try { CapaDeDomini::getInstance().registraUsuari(nom, user, mail, pass, edat); cout << "Usuari registrat correctament!" << endl; }
    catch (exception& e) { cout << "Error: " << e.what() << endl; }
    pausa();
}

// [3.1.4] Consulta Usuari
void CapaDePresentacio::consultaUsuari() {
    try {
        DTOUsuari dto = CapaDeDomini::getInstance().consultaUsuari();
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

// [3.1.5] Modifica Usuari
void CapaDePresentacio::modificaUsuari() {
    try {
        DTOUsuari actual = CapaDeDomini::getInstance().consultaUsuari();
        cout << "\n** Modifica usuari **" << endl;
        cout << "Nom complet: " << actual.obteNom() << endl;
        cout << "Sobrenom: " << actual.obteSobrenom() << endl;
        cout << "Correu electronic: " << actual.obteCorreu() << endl;
        cout << "Edat: " << actual.obteEdat() << endl;
        cout << "\n(Deixa en blanc i prem Enter si no vols canviar el valor)" << endl;
        
        string buff, nouNom, nouCorreu; int novaEdat = -1;
        cout << "Nom complet [" << actual.obteNom() << "]: "; getline(cin, nouNom);
        cout << "Correu electronic [" << actual.obteCorreu() << "]: "; getline(cin, nouCorreu);
        cout << "Edat [" << actual.obteEdat() << "]: "; getline(cin, buff);
        if (!buff.empty()) try { novaEdat = stoi(buff); }
        catch (...) { cout << "Format d'edat incorrecte." << endl; }

        CapaDeDomini::getInstance().modificaUsuari(nouNom, nouCorreu, novaEdat);
        cout << "\nModificacio realitzada amb exit. Dades actualitzades (BD):" << endl;
        consultaUsuari(); return;
    }
    catch (exception& e) { cout << "Error: " << e.what() << endl; }
    pausa();
}

// [3.1.6] Esborra Usuari
void CapaDePresentacio::esborraUsuari() {
    string password, confirmacio;
    cout << "\n** Esborrar usuari **" << endl;
    cout << "Per confirmar has d'entrar la contrasenya: "; cin >> password; cin.ignore();
    try {
        cout << "Vols confirmar l'esborrat? (S/N): "; cin >> confirmacio; cin.ignore();
        if (confirmacio == "S" || confirmacio == "s") { CapaDeDomini::getInstance().esborraUsuari(password); cout << "Usuari esborrat correctament!" << endl; }
    }
    catch (const std::exception& e) { cout << "Error: " << e.what() << endl; }
    pausa();
}

// [3.2.1] Reserva Escapada
void CapaDePresentacio::reservaEscapada() {
    try {
        cout << "\n** Reservar escapada **" << endl;
        string nom, confirm;
        cout << "Nom: "; getline(cin, nom);

        DTOExperiencia dto = CapaDeDomini::getInstance().consultaEscapada(nom);

        cout << "Descripcio: " << dto.obteDescripcio() << endl;
        cout << "Ciutat: " << dto.obteCiutat() << endl;
        cout << "Nombre de places: " << dto.obteMaximPlaces() << endl;
        cout << dto.obteDetalls() << endl;
        cout << "Preu: " << dto.obtePreu() << " eur" << endl;

        cout << "\nVols confirmar la reserva? (S/N): "; cin >> confirm; cin.ignore();
        if (confirm == "S" || confirm == "s") {
            CapaDeDomini::getInstance().reservaEscapada(nom);
            cout << "Reserva registrada correctament." << endl;
            cout << "Preu aplicat a la reserva: " << dto.obtePreu() << " eur" << endl;
        }
        else { cout << "Reserva cancel lada." << endl; }
    }
    catch (const std::exception& e) { cout << "Error: " << e.what() << endl; }
    pausa();
}

// [3.2.2] Reserva Activitat
void CapaDePresentacio::reservaActivitat() {
    try {
        cout << "\n** Reservar activitat **" << endl;
        string nom, confirm; int persones;
        cout << "Nom: "; getline(cin, nom);

        DTOExperiencia dto = CapaDeDomini::getInstance().consultaActivitat(nom);

        cout << "Descripcio: " << dto.obteDescripcio() << endl;
        cout << "Ciutat: " << dto.obteCiutat() << endl;
        cout << "Nombre maxim de places: " << dto.obteMaximPlaces() << endl;
        cout << dto.obteDetalls() << endl;
        cout << "Preu per persona: " << dto.obtePreu() << " eur" << endl;

        bool intentar = true;
        while (intentar) {
            cout << "\nNombre de persones: "; cin >> persones; cin.ignore();

            float total = CapaDeDomini::getInstance().calculaPreu(nom, persones);
            cout << "Preu total de la reserva: " << total << " eur" << endl;

            cout << "Vols continuar amb la reserva? (S/N): "; cin >> confirm; cin.ignore();
            if (confirm == "S" || confirm == "s") {
                try {
                    CapaDeDomini::getInstance().reservaActivitat(nom, persones);
                    cout << "Reserva registrada correctament." << endl;
                    intentar = false;
                }
                catch (const std::exception& e) {
                    cout << "\nERROR: " << e.what() << endl;
                    string opcio;
                    cout << "Vols entrar un numero alternatiu? (S/N) (N per finalitzar): ";
                    cin >> opcio; cin.ignore();
                    if (opcio != "S" && opcio != "s") {
                        cout << "Operacio finalitzada per l'usuari." << endl;
                        intentar = false;
                    }
                }
            }
            else {
                cout << "Reserva cancel lada." << endl;
                intentar = false;
            }
        }
    }
    catch (const std::exception& e) { cout << "Error: " << e.what() << endl; }
    pausa();
}

// [3.2.3] Consulta Reserves
void CapaDePresentacio::consultaReserves() {
    try {
        auto llista = CapaDeDomini::getInstance().consultaReserves();
        cout << "\n===== LES MEVES RESERVES =====" << endl;

        if (llista.empty()) {
            cout << "No tens cap reserva." << endl;
        }
        else {
            float totalGlobal = 0.0f;
            float totalEscapades = 0.0f;
            float totalActivitats = 0.0f;

            for (const auto& r : llista) {
                float preu = r.obtePreuPagat();
                totalGlobal += preu;
                if (r.obteExperiencia().obteTipus() == "ACTIVITAT") totalActivitats += preu;
                else totalEscapades += preu;
            }

            cout << ">>> PREU TOTAL PAGAT: " << std::fixed << std::setprecision(2) << totalGlobal << " eur" << endl << endl;

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
    catch (const std::exception& e) { cout << "Error: " << e.what() << endl; }
    pausa();
}

// [3.3.1] Consulta Experiencies
void CapaDePresentacio::consultaExperiencies() {
    try {
        cout << "\n--- CERCAR EXPERIENCIES ---" << endl;
        string ciutat; int persones;
        cout << "Ciutat: "; getline(cin, ciutat);
        cout << "Nombre de persones: "; cin >> persones; cin.ignore();

        auto llista = CapaDeDomini::getInstance().consultaExperiencies(ciutat, persones);

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
    catch (const std::exception& e) { cout << "Error: " << e.what() << endl; }
    pausa();
}

// [3.3.2] Consultar Novetats
void CapaDePresentacio::consultarNovetats() {
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
    catch (const std::exception& e) { cout << "Error: " << e.what() << endl; }
    pausa();
}

// [3.3.3] Consulta Mes Reservades
void CapaDePresentacio::consultaMesReservades() {
    try {
        auto llista = CapaDeDomini::getInstance().consultaMesReservades();
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
                    cout << "#" << count << " " << dto.obteNom() << endl;
                    cout << "   " << dto.obteDescripcio() << ", " << dto.obteCiutat() << endl;
                    cout << "   Places: " << dto.obteMaximPlaces() << ", Preu: " << dto.obtePreu() << endl;
                    cout << "   " << dto.obteDetalls() << endl;
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
                    cout << "   " << dto.obteDescripcio() << ", " << dto.obteCiutat() << endl;
                    cout << "   Places: " << dto.obteMaximPlaces() << ", Preu: " << dto.obtePreu() << endl;
                    cout << "   " << dto.obteDetalls() << endl;
                    if (count == 5) break;
                }
            }
            if (count == 0) cout << "Cap." << endl;
        }
    }
    catch (const std::exception& e) { cout << "Error: " << e.what() << endl; }
    pausa();
}