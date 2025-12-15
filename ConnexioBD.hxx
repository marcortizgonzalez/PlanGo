#pragma once
#include <memory>
#include <string>
#include <map>
#include <odb/mysql/database.hxx>

class ConnexioBD {
public:
    // Singleton: Instancia única
    static ConnexioBD& getInstance();

    // Método para obtener la conexión
    std::shared_ptr<odb::mysql::database> getDB() const;

private:
    ConnexioBD(); // Constructor privado
    std::map<std::string, std::string> carregar_env(const std::string& fitxer);

    std::shared_ptr<odb::mysql::database> _db;
};