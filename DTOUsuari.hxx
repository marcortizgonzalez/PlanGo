#pragma once
#include <string>

class DTOUsuari {
public:
    DTOUsuari(std::string n, std::string s, std::string c, int e, int nRes)
        : nom(n), sobrenom(s), correu(c), edat(e), numReserves(nRes) {
    }

    std::string obteNom() const { return nom; }
    std::string obteSobrenom() const { return sobrenom; }
    std::string obteCorreu() const { return correu; }
    int obteEdat() const { return edat; }
    int obteNumReserves() const { return numReserves; }

private:
    std::string nom;
    std::string sobrenom;
    std::string correu;
    int edat;
    int numReserves;
};