#include "CapaDePresentacio.hxx"
#include "CapaDeDomini.hxx"
#include <iostream>
#include <limits> 
#include <string>

using namespace std;

// --- FUNCIONES AUXILIARES ---

void CapaDePresentacio::pausa() {
    cout << "Prem Intro per continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// --- GESTIÓN DE FLUJO (MENÚS) ---

void CapaDePresentacio::inici() {
    int opcio = -1;
    while (opcio != 0) {
        // Menú inicial sin sesión
        cout << "\n===== MENU PRINCIPAL (No Loggejat) =====" << endl;
        cout << "1. Iniciar sessio" << endl;
        cout << "2. Registrar usuari" << endl;
        cout << "3. Consultes" << endl;
        cout << "0. Sortir" << endl;
        cout << "Opcio: ";
        cin >> opcio;

        switch (opcio) {
        case 1:
            iniciarSessio();
            if (CapaDeDomini::getInstance().getUsuariLoggejat() != nullptr) {
                menuPrincipal();
            }
            break;
        case 2:
            registrarUsuari();
            break;
        case 3:
            menuConsultes();
            break;
        case 0:
            cout << "Fins aviat!" << endl;
            break;
        default:
            cout << "Opcio no valida." << endl;
            pausa();
            break;
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

        switch (opcio) {
        case 1: menuGestioUsuaris(); break;
        case 2: menuGestioReserves(); break;
        case 3: menuConsultes(); break;
        case 4: tancarSessio(); break;
        case 0:
            cout << "Tancant aplicacio..." << endl;
            exit(0);
            break;
        default: cout << "Opcio no valida." << endl; pausa(); break;
        }
    }
}

// --- SUBMENÚS ---

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

        switch (opcio) {
        case 1: consultarUsuari(); break;
        case 2: modificarUsuari(); break;
        case 3:
            esborrarUsuari();
            // Si se borra el usuario, nos echa al menú principal
            if (CapaDeDomini::getInstance().getUsuariLoggejat() == nullptr) return;
            break;
        case 4:
            tancarSessio();
            return; // Salir del submenú
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
        cout << "Opcio: ";
        cin >> opcio;

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
        cout << "Opcio: ";
        cin >> opcio;

        switch (opcio) {
        case 1: consultarExperiencies(); break;
        case 2: consultaNovetats(); break;
        case 3: consultarMesReservades(); break;
        case 0: break;
        default: cout << "Opcio no valida." << endl; break;
        }
    }
}

// --- IMPLEMENTACIÓN DE CASOS DE USO ---

void CapaDePresentacio::iniciarSessio() {
    string user, pwd;
    cout << "\n** Inici sessio **" << endl;
    cout << "Sobrenom: "; cin >> user;
    cout << "Contrasenya: "; cin >> pwd;

    try {
        // Llamada correcta con contraseña
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
    cout << "Vols tancar la sessio (S/N): ";
    cin >> confirm;
    if (confirm == "S" || confirm == "s") {
        CapaDeDomini::getInstance().tancarSessio();
        cout << "Sessio tancada correctament!" << endl;
    }
}

// --- AQUÍ ESTABAN LOS [TODO], AHORA IMPLEMENTADOS ---

void CapaDePresentacio::registrarUsuari() {
    string nom, user, mail, pass, dataN;
    cout << "\n--- REGISTRAR NOU USUARI ---" << endl;
    cout << "Nom complet: "; cin >> nom;
    cout << "Sobrenom: "; cin >> user;
    cout << "Correu: "; cin >> mail;
    cout << "Contrasenya: "; cin >> pass;
    cout << "Data Naixement (AAAA-MM-DD): "; cin >> dataN;

    try {
        CapaDeDomini::getInstance().registrarUsuari(nom, user, mail, pass, dataN);
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

void CapaDePresentacio::modificarUsuari() {
    try {
        DTOUsuari actual = CapaDeDomini::getInstance().consultarUsuari();
        cout << "\n--- MODIFICAR USUARI (" << actual.obteSobrenom() << ") ---" << endl;
        cout << "(Deixa en blanc [posa un guio '-'] si no vols canviar)" << endl;

        string nom, mail, pass, data;
        cout << "Nou Nom [" << actual.obteNom() << "]: "; cin >> nom;
        cout << "Nou Correu [" << actual.obteCorreu() << "]: "; cin >> mail;
        cout << "Nova Contrasenya: "; cin >> pass;
        cout << "Nova Data: "; cin >> data;

        // Limpieza simple: si el usuario pone un guion, lo tratamos como vacío
        if (nom == "-") nom = "";
        if (mail == "-") mail = "";
        if (pass == "-") pass = "";
        if (data == "-") data = "";

        CapaDeDomini::getInstance().modificarUsuari(nom, mail, pass, data);
        cout << "Dades actualitzades correctament." << endl;
    }
    catch (exception& e) {
        cout << "Error: " << e.what() << endl;
    }
    pausa();
}

void CapaDePresentacio::esborrarUsuari() {
    string password, confirmacio;
    cout << "\n** Esborrar usuari **" << endl;
    cout << "Per confirmar has d'entrar la contrasenya:" << endl;
    cout << "Contrasenya: ";
    cin >> password;

    try {
        cout << "Vols confirmar l'esborrat? (S/N): ";
        cin >> confirmacio;

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

// --- ESTOS SIGUEN PENDIENTES (Bloque B y C) ---
void CapaDePresentacio::reservarEscapada() {
    cout << "[TODO] Reservar Escapada - En construccio" << endl;
    pausa();
}

void CapaDePresentacio::reservarActivitat() {
    cout << "[TODO] Reservar Activitat - En construccio" << endl;
    pausa();
}

void CapaDePresentacio::consultarReserves() {
    cout << "[TODO] Visualitzar Reserves - En construccio" << endl;
    pausa();
}

void CapaDePresentacio::consultarExperiencies() {
    cout << "[TODO] Consultar Experiencies - En construccio" << endl;
    pausa();
}

void CapaDePresentacio::consultaNovetats() {
    try {
        auto novetats = CapaDeDomini::getInstance().consultarNovetats();
        cout << "\n** Consultar novetats **" << endl;
        for (const auto& dto : novetats) {
            cout << "[" << dto.obteTipus() << "] " << dto.obteNom()
                << " (" << dto.obteCiutat() << ")" << endl;
            cout << "   " << dto.obteDetalls() << endl;
            cout << "   Categories: ";
            for (const auto& cat : dto.obteCategories()) cout << cat << " ";
            cout << endl;
        }
    }
    catch (exception& e) {
        cout << "Error: " << e.what() << endl;
    }
    pausa();
}

void CapaDePresentacio::consultarMesReservades() {
    cout << "[TODO] Consultar Mes Reservades - En construccio" << endl;
    pausa();
}