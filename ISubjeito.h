#pragma once
#include "IObservador.h"

// Padrão Observer - papel: Subject (abstrato)
class ISubjeito {
public:
    virtual void registrar(IObservador* obs) = 0;
    virtual void remover(IObservador* obs)   = 0;
    virtual void notificar()                  = 0;
    virtual ~ISubjeito() = default;
};
