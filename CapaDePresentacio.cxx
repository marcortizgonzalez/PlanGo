#include "CapaDePresentacio.hxx"
#include "CapaDeDomini.hxx"
#include <iostream>
#include <limits> 
#include <string>

using namespace std;

// --- Helpers ---
void CapaDePresentacio::pausa() {
    cout << "Prem Intro per continuar...";
    // Limpieza robusta del buffer
    // cin.ignore si hay algo que ignorar, pero a veces cin.get() basta si el buffer esta limpio
    // Para simplificar y asegurar:
    string dummy;
    getline(cin, dummy);
}

// --- MENÚS ---
void CapaDePresentacio::inici() {
    int opcio = -1;
    while (opcio != 0) {
        cout << "\n===== MENU PRINCIPAL (No Loggejat) =====" << endl;
        cout << "1. Iniciar sessio" << endl;
        cout << "2. Registrar usuari" << endl;
        cout << "3. Consultes" << endl;
        cout << "0. Sortir" << endl;
        cout << "Opcio: ";
        cin >> opcio;
        cin.ignore(); // Limpiar el \n del buffer después de leer int

        switch (opcio) {
        case 1:
            iniciarSessio();
            if (CapaDeDomini::getInstance().getUsuariLoggejat() != nullptr) menuPrincipal();
            break;
        case 2: registrarUsuari(); break;
        case 3: menuConsultes(); break;
        case 0: cout << "Fins aviat!" << endl; break;
        default: cout << "Opcio no valida." << endl; break;
        }
    }
}

void CapaDePresentacio::menuPrincipal() {
    int opcio = -1;
    while (opcio != 0 && CapaDeDomini::getInstance().getUsuariLoggejat() != nullptr) {
        cout << "\n===== MENU PRINCIPAL (Loggejat) =====" << endl;
        cout << "1. Gestio usuaris" << endl;
        cout << "2. Gestio reserves" << endl;
        cout << "3. Consultes" << endl;
        cout << "4. Tancar sessio" << endl;
        cout << "0. Sortir" << endl;
        cout << "Opcio: ";
        cin >> opcio;
        cin.ignore();

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

void CapaDePresentacio::menuGestioUsuaris() {
    int opcio = -1;
    while (opcio != 0) {
        cout << "\n--- GESTIO USUARIS ---" << endl;
        cout << "1. Consulta usuari" << endl;
        cout << "2. Modifica usuari" << endl;
        cout << "3. Esborra usuari" << endl;
        cout << "4. Tancar sessio" << endl;
        cout << "0. Tornar" << endl;
        cout << "Opcio: ";
        cin >> opcio;
        cin.ignore();

        switch (opcio) {
        case 1: consultarUsuari(); break;
        case 2: modificarUsuari(); break;
        case 3:
            esborrarUsuari();
            if (CapaDeDomini::getInstance().getUsuariLoggejat() == nullptr) return;
            break;
        case 4: tancarSessio(); return;
        case 0: break;
        default: cout << "Opcio no valida." << endl; break;
        }
    }
}
// (Mantén los otros submenús igual, menuGestioReserves, menuConsultes...)
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

// --- CASOS DE USO ---

void CapaDePresentacio::iniciarSessio() {
    string user, pwd;
    cout << "\n** Inici sessio **" << endl;
    cout << "Sobrenom: "; cin >> user;
    cin.ignore(); // Importante limpiar buffer antes de leer pass si fuera getline, aunque cin>>pwd va bien
    cout << "Contrasenya: "; cin >> pwd;
    cin.ignore();

    try {
        CapaDeDomini::getInstance().iniciarSessio(user, pwd);
        cout << "Sessio iniciada correctament!" << endl;
    }
    catch (exception& e) {
        cout << "Error: " << e.what() << endl;
    }
    pausa();
}

void CapaDePresentacio::tancarSessio() {
    string confirm;
    cout << "\n** Tancar sessio **" << endl;
    cout << "Vols tancar la sessio (S/N): "; cin >> confirm;
    cin.ignore();
    if (confirm == "S" || confirm == "s") {
        CapaDeDomini::getInstance().tancarSessio();
        cout << "Sessio tancada correctament!" << endl;
    }
}

void CapaDePresentacio::registrarUsuari() {
    string nom, sobrenom, mail, pass;
    int edat;

    cout << "\n--- REGISTRAR NOU USUARI ---" << endl;

    // CORRECCIÓN ERROR 2: Usar getline para nombre completo
    cout << "Nom complet: ";
    // cin.ignore() ya se hizo en el menú antes de llamar a esta función
    getline(cin, nom);

    cout << "Sobrenom (ID): "; cin >> sobrenom;
    cout << "Correu electronic: "; cin >> mail;
    cout << "Contrasenya: "; cin >> pass;
    cout << "Edat: "; cin >> edat;
    cin.ignore(); // Limpiar tras leer int

    try {
        CapaDeDomini::getInstance().registrarUsuari(nom, sobrenom, mail, pass, edat);
        cout << "Usuari registrat correctament!" << endl;
    }
    catch (exception& e) {
        cout << "Error: " << e.what() << endl;
    }
    pausa();
}

void CapaDePresentacio::consultarUsuari() {
    try {
        DTOUsuari dto = CapaDeDomini::getInstance().consultarUsuari();
        cout << "\n--- DADES USUARI ---" << endl;
        cout << "Sobrenom: " << dto.obteSobrenom() << endl;
        cout << "Nom: " << dto.obteNom() << endl;
        cout << "Correu: " << dto.obteCorreu() << endl;
        cout << "Edat: " << dto.obteEdat() << " anys" << endl;
        cout << "Reserves fetes: " << dto.obteNumReserves() << endl;
    }
    catch (exception& e) {
        cout << "Error: " << e.what() << endl;
    }
    pausa();
}

// CORRECCIÓN ERROR 3: Implementación estricta del caso de uso
void CapaDePresentacio::modificarUsuari() {
    try {
        // 1. Mostrar información actual
        DTOUsuari actual = CapaDeDomini::getInstance().consultarUsuari();
        cout << "\n--- MODIFICAR USUARI (" << actual.obteSobrenom() << ") ---" << endl;
        // Se muestra todo menos contraseña (ya lo hace consultarUsuari por DTO)
        cout << "Dades actuals:" << endl;
        cout << "Nom: " << actual.obteNom() << endl;
        cout << "Correu: " << actual.obteCorreu() << endl;
        cout << "Edat: " << actual.obteEdat() << endl;

        cout << "\n(Deixa en blanc i prem Enter si no vols canviar el valor)" << endl;

        string buff;
        string nouNom, nouCorreu;
        int novaEdat = -1;

        // 2. Pedir campos sucesivamente
        cout << "Nom complet [" << actual.obteNom() << "]: ";
        getline(cin, nouNom);

        cout << "Correu electronic [" << actual.obteCorreu() << "]: ";
        getline(cin, nouCorreu);

        cout << "Edat [" << actual.obteEdat() << "]: ";
        getline(cin, buff);
        if (!buff.empty()) {
            try {
                novaEdat = stoi(buff);
            }
            catch (...) {
                cout << "Format d'edat incorrecte. S'ignorara aquest camp." << endl;
            }
        }

        // 3. Llamar al sistema para modificar
        CapaDeDomini::getInstance().modificarUsuari(nouNom, nouCorreu, novaEdat);

        // 4. Mostrar información actualizada DESDE BASE DE DATOS
        // La llamada a consultarUsuari dentro de CapaDeDomini ya fuerza la recarga (reload)
        cout << "\nModificacio realitzada amb exit. Dades actualitzades (BD):" << endl;
        consultarUsuari(); // Reutilizamos la función de visualización

    }
    catch (exception& e) {
        cout << "Error: " << e.what() << endl;
    }
    pausa();
}

void CapaDePresentacio::esborrarUsuari() {
    string password, confirmacio;
    cout << "\n** Esborrar usuari **" << endl;
    cout << "Contrasenya per confirmar: "; cin >> password;
    cin.ignore();

    try {
        cout << "Segur? (S/N): "; cin >> confirmacio; cin.ignore();
        if (confirmacio == "S" || confirmacio == "s") {
            CapaDeDomini::getInstance().esborrarUsuari(password);
            cout << "Usuari esborrat correctament!" << endl;
        }
    }
    catch (const std::exception& e) {
        cout << "Error: " << e.what() << endl;
    }
    pausa();
}

// Stubs del resto
void CapaDePresentacio::reservarEscapada() { cout << "En construccio" << endl; pausa(); }
void CapaDePresentacio::reservarActivitat() { cout << "En construccio" << endl; pausa(); }
void CapaDePresentacio::consultarReserves() { cout << "En construccio" << endl; pausa(); }
void CapaDePresentacio::consultarExperiencies() { cout << "En construccio" << endl; pausa(); }
void CapaDePresentacio::consultaNovetats() {
    try {
        auto novetats = CapaDeDomini::getInstance().consultarNovetats();
        cout << "\n** Consultar novetats **" << endl;
        for (const auto& dto : novetats) {
            cout << "[" << dto.obteTipus() << "] " << dto.obteNom() << " (" << dto.obteCiutat() << ")" << endl;
            // Mostramos precio
            cout << "   Preu: " << dto.obtePreu() << " eur" << endl;
            cout << "   " << dto.obteDetalls() << endl;
        }
    }
    catch (exception& e) { cout << "Error: " << e.what() << endl; }
    pausa();
}
void CapaDePresentacio::consultarMesReservades() { cout << "En construccio" << endl; pausa(); }