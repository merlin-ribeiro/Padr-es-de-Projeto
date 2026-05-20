#pragma once
#include "PoliticoColaborador.h"

// Padrão Mediator: ConcreteColleague - papel de quem pergunta
class InquiridorColaborador : public PoliticoColaborador {
public:
    explicit InquiridorColaborador(const std::string& nome)
        : PoliticoColaborador(nome) {}

    // Delegado ao mediador via chamar_operacao() em extensões reais;
    // aqui a escolha é feita diretamente pela Facade conforme o diagrama
    void escolher_inquirido(PoliticoColaborador* politico);
    // implementação em MediadorDebate.h para evitar dependência circular
};

// Padrão Mediator: ConcreteColleague - papel de quem responde
class InquiridoColaborador : public PoliticoColaborador {
public:
    explicit InquiridoColaborador(const std::string& nome)
        : PoliticoColaborador(nome) {}
};
