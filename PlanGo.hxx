#pragma once
#include <memory>

class PlanGo {
public:
    static PlanGo& getInstance() {
        static PlanGo instance;
        return instance;
    }

    // Getter y Setter para el descuento
    float getDescompte() const { return descompte; }
    void setDescompte(float d) { descompte = d; }

private:
    PlanGo() : descompte(0.0) {} // Constructor privado

    float descompte;
};