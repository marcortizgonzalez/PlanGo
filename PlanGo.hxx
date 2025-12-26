#pragma once
#include <memory>

// Clase principal de configuración de la aplicación (Patrón Singleton)
class PlanGo {
public:
    // Obtiene la instancia única de la clase PlanGo
    static PlanGo& getInstance() {
        static PlanGo instance;
        return instance;
    }

    // --- GESTIÓN DE CONFIGURACIÓN ---

    // Devuelve el porcentaje de descuento global actual
    float getDescompte() const { return descompte; }

    // Establece el porcentaje de descuento global para las reservas
    void setDescompte(float d) { descompte = d; }

private:
    PlanGo() : descompte(0.0) {} // Constructor privado para garantizar Singleton

    float descompte;
};