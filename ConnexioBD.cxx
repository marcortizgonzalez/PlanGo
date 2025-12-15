#include "ConnexioBD.hxx"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

using namespace std;

ConnexioBD& ConnexioBD::getInstance() {
    static ConnexioBD instance;
    return instance;
}

shared_ptr<odb::mysql::database> ConnexioBD::getDB() const {
    return _db;
}

map<string, string> ConnexioBD::carregar_env(const string& fitxer) {
    ifstream in(fitxer);
    if (!in.is_open()) throw runtime_error("No s'ha pogut obrir " + fitxer);

    map<string, string> vars;
    string linia;
    while (getline(in, linia)) {
        if (linia.empty() || linia[0] == '#') continue;
        auto pos = linia.find('=');
        if (pos != string::npos) {
            string clau = linia.substr(0, pos);
            string valor = linia.substr(pos + 1);
            // Netejar possibles retorns de carro (\r) si ve de Windows
            if (!valor.empty() && valor.back() == '\r') valor.pop_back();
            vars[clau] = valor;
        }
    }
    return vars;
}

ConnexioBD::ConnexioBD() {
    try {
        auto env = carregar_env("configuracio.env");
        // Convertir port a int
        int port = 0;
        if (env.find("DB_PORT") != env.end()) port = stoi(env["DB_PORT"]);

        _db = make_shared<odb::mysql::database>(
            env["DB_USER"], env["DB_PASS"], env["DB_NAME"], env["DB_HOST"], port);
    }
    catch (const exception& e) {
        cerr << "Error configurant BD: " << e.what() << endl;
        exit(1);
    }
}