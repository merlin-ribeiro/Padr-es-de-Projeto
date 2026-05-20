#pragma once

class Configuracao; // forward declaration

// Padrão Mediator - papel: Mediator (abstrato)
class Mediador {
public:
    virtual void debate(Configuracao* config) = 0;
    virtual ~Mediador() = default;
};
