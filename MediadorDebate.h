#pragma once
#include <iostream>
#include <vector>
#include "Mediador.h"
#include "Configuracao.h"
#include "Colaboradores.h"
#include "Logger.h"

// Padrão Mediator: ConcreteMediator
class MediadorDebate : public Mediador {
private:
    InquiridorColaborador* inquiridor = nullptr;
    InquiridoColaborador*  inquirido  = nullptr;

public:
    ~MediadorDebate() {
        delete inquiridor;
        delete inquirido;
    }

    // Cria InquiridorColaborador copiando nome e observadores do politico base
    void set_inquiridor(PoliticoColaborador* politico) {
        delete inquiridor;
        inquiridor = new InquiridorColaborador(politico->getNome());
        inquiridor->set_mediador(politico->get_mediador());
    }

    // Cria InquiridoColaborador copiando nome e observadores do politico base
    void set_inquirido(PoliticoColaborador* politico) {
        delete inquirido;
        inquirido = new InquiridoColaborador(politico->getNome());
        inquirido->set_mediador(politico->get_mediador());
    }

    InquiridorColaborador* get_inquiridor() { return inquiridor; }
    InquiridoColaborador*  get_inquirido()  { return inquirido;  }

    // Propaga lista de observadores para o colaborador interno (inquiridor ou inquirido)
    void registrar_observers_inquiridor(const std::vector<IObservador*>& obs) {
        if (inquiridor)
            for (auto* o : obs) inquiridor->registrar(o);
    }

    void registrar_observers_inquirido(const std::vector<IObservador*>& obs) {
        if (inquirido)
            for (auto* o : obs) inquirido->registrar(o);
    }

    // Orquestra o debate: pergunta -> resposta -> réplica -> tréplica
    void debate(Configuracao* config) override {
        if (!inquiridor || !inquirido) {
            std::cout << "[ERRO] Inquiridor ou inquirido nao definido.\n";
            return;
        }

        std::cout << "\n--- PERGUNTA (" << config->get_pergunta_tempo() << "s) ---\n";
        Logger::get_instance()->register_log("Pergunta: " + inquiridor->getNome());
        inquiridor->falar(config->get_pergunta_tempo());

        std::cout << "\n--- RESPOSTA (" << config->get_resposta_tempo() << "s) ---\n";
        Logger::get_instance()->register_log("Resposta: " + inquirido->getNome());
        inquirido->falar(config->get_resposta_tempo());

        std::cout << "\n--- REPLICA (" << config->get_replica_tempo() << "s) ---\n";
        Logger::get_instance()->register_log("Replica: " + inquiridor->getNome());
        inquiridor->falar(config->get_replica_tempo());

        std::cout << "\n--- TREPLICA (" << config->get_treplica_tempo() << "s) ---\n";
        Logger::get_instance()->register_log("Treplica: " + inquirido->getNome());
        inquirido->falar(config->get_treplica_tempo());
    }
};
