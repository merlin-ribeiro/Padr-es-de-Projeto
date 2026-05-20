#pragma once
#include <string>
#include <iostream>
#include "IObservador.h"
#include "PoliticoColaborador.h"

// Padrão Observer: ConcreteObserver
class Eleitor : public IObservador {
private:
    std::string           nome;
    PoliticoColaborador*  candidato;  // cada eleitor tem exatamente 1 candidato

public:
    Eleitor(const std::string& nome, PoliticoColaborador* candidato)
        : nome(nome), candidato(candidato) {}

    // Recebe notificação; exibe mensagem apenas se for o seu candidato
    void atualizar(const std::string& nomeCandidato) override {
        if (nomeCandidato == candidato->getNome()) {
            std::cout << "  >>> [ELEITOR: " << nome
                      << "] SEU CANDIDATO ESTA FALANDO ("
                      << nomeCandidato << ") <<<\n";
        }
    }

    std::string getNome()     const { return nome; }
    std::string getCandidato() const { return candidato->getNome(); }
};
