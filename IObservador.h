#pragma once
#include <string>

// Padrão Observer - papel: Observer (abstrato)
class IObservador {
public:
    virtual void atualizar(const std::string& nomeCandidato) = 0;
    virtual ~IObservador() = default;
};
