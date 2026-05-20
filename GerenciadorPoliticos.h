#pragma once
#include <vector>
#include <string>
#include <stdexcept>
#include <cstdlib>
#include "PoliticoColaborador.h"
#include "Mediador.h"

class GerenciadorPoliticos {
private:
    std::vector<PoliticoColaborador*> politicos;
    int total_sorteados = 0;

public:
    ~GerenciadorPoliticos() {
        for (auto* p : politicos) delete p;
    }

    PoliticoColaborador* criar_politico(const std::string& nome,
                                        Mediador* mediador) {
        auto* p = new PoliticoColaborador(nome);
        p->set_mediador(mediador);
        politicos.push_back(p);
        return p;
    }

    // Sorteia um político que ainda não foi inquiridor
    PoliticoColaborador* sortear() {
        if (total_sorteados >= (int)politicos.size())
            throw std::runtime_error("Todos os candidatos ja foram inquiridores.");

        PoliticoColaborador* escolhido = nullptr;
        do {
            int idx = std::rand() % politicos.size();
            escolhido = politicos[idx];
        } while (escolhido->get_sorteado());

        escolhido->set_sorteado(true);
        total_sorteados++;
        return escolhido;
    }

    PoliticoColaborador* obter_politico(const std::string& nome) {
        for (auto* p : politicos) {
            if (p->getNome() == nome) return p;
        }
        throw std::runtime_error("Politico nao encontrado: " + nome);
    }

    const std::vector<PoliticoColaborador*>& listar() const {
        return politicos;
    }

    int total() const { return (int)politicos.size(); }
    int sorteados() const { return total_sorteados; }
};
