#pragma once
#include <memory>
#include <string>
#include <map>
#include <odb/mysql/database.hxx>

class ConnexioBD {
public:
    static ConnexioBD& getInstance();
    std::shared_ptr<odb::mysql::database> getDB() const;

private:
    ConnexioBD();
    std::map<std::string, std::string> carregar_env(const std::string& fitxer);
    std::shared_ptr<odb::mysql::database> _db;
};