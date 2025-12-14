#pragma once
#include <string>
#include <odb/core.hxx>

#pragma db object
class Categoria {
public:
    Categoria(const std::string& nom) : nom(nom) {}
    const std::string& getNom() const { return nom; }

private:
    friend class odb::access;
    Categoria() {} // Constructor por defecto necesario para ODB

#pragma db id
    std::string nom;
};