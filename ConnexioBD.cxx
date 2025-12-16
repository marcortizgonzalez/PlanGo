#include "ConnexioBD.hxx"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <cstdlib> // Para system("pause")

using namespace std;

ConnexioBD& ConnexioBD::getInstance() {
    static ConnexioBD instance;
    return instance;
}

shared_ptr<odb::mysql::database> ConnexioBD::getDB() const {
    return _db;
}

// Función auxiliar para leer el fichero .env
map<string, string> ConnexioBD::carregar_env(const string& fitxer) {
    ifstream in(fitxer);
    if (!in.is_open()) {
        throw runtime_error("No s'ha pogut obrir el fitxer: " + fitxer);
    }

    map<string, string> vars;
    string linia;
    while (getline(in, linia)) {
        // Ignorar líneas vacías o comentarios
        if (linia.empty() || linia[0] == '#') continue;

        auto pos = linia.find('=');
        if (pos != string::npos) {
            string clau = linia.substr(0, pos);
            string valor = linia.substr(pos + 1);

            // LIMPIEZA IMPORTANTE: Eliminar el retorno de carro (\r) si existe
            // (Windows a veces deja caracteres invisibles que rompen la contraseña)
            if (!valor.empty() && valor.back() == '\r') {
                valor.pop_back();
            }

            vars[clau] = valor;
        }
    }
    return vars;
}

ConnexioBD::ConnexioBD() {
    try {
        // 1. Cargar variables del fichero
        auto env = carregar_env("configuracio.env");

        // 2. Verificar que existen las claves
        if (env.find("DB_USER") == env.end() || env.find("DB_PASS") == env.end()) {
            throw runtime_error("Falten variables al fitxer configuracio.env");
        }

        int port = 0;
        if (env.find("DB_PORT") != env.end()) {
            port = stoi(env["DB_PORT"]);
        }

        // 3. Crear la conexión ODB
        _db = make_shared<odb::mysql::database>(
            env["DB_USER"],
            env["DB_PASS"],
            env["DB_NAME"],
            env["DB_HOST"],
            port
        );

    }
    catch (const exception& e) {
        cerr << "ERROR FATAL (ConnexioBD): " << e.what() << endl;
        cerr << "Verifica que 'configuracio.env' existeix i te les dades correctes." << endl;
        system("pause"); // Pausa para que veas el error si falla
        exit(1);
    }
}