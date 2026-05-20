#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include "ISubjeito.h"
#include "IObservador.h"
#include "MicrofoneCronometro.h"

class Mediador; // forward declaration

// Padrão Mediator: Colleague base
// Padrão Observer: ConcreteSubject
class PoliticoColaborador : public ISubjeito {
private:
    std::string name;
    Mediador*   mediador  = nullptr;
    bool        sorteado  = false;
    MicrofoneCronometro microfone;

    // Observer: lista de eleitores observando este político
    std::vector<IObservador*> observadores;

public:
    explicit PoliticoColaborador(const std::string& nome) : name(nome) {}

    // ---------- ISubjeito ----------
    void registrar(IObservador* obs) override {
        observadores.push_back(obs);
    }

    void remover(IObservador* obs) override {
        observadores.erase(
            std::remove(observadores.begin(), observadores.end(), obs),
            observadores.end()
        );
    }

    void notificar() override {
        for (auto* obs : observadores) {
            obs->atualizar(name);
        }
    }

    // ---------- Falar (modificado para notificar ANTES de abrir microfone) ----------
    void falar(int tempo) {
        notificar();                        // eleitores são avisados primeiro
        microfone.ativar();
        microfone.esperar_tempo(tempo);
        microfone.desativar();
    }

    // ---------- Mediator ----------
    void chamar_operacao() {
        // ponto de extensão para subclasses notificarem o mediador
    }

    void set_mediador(Mediador* m) { mediador = m; }
    Mediador* get_mediador() const { return mediador; }

    // ---------- Estado sorteio ----------
    void set_sorteado(bool op) { sorteado = op; }
    bool get_sorteado()  const { return sorteado; }

    // ---------- Identidade ----------
    std::string getNome() const { return name; }
};
