#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "MediadorDebate.h"
#include "Configuracao.h"
#include "GerenciadorPoliticos.h"
#include "Logger.h"
#include "Eleitor.h"

// Padrão Facade + Singleton
class Facade {
private:
    static Facade*        instance;
    MediadorDebate*       mediador_debate;
    Configuracao*         config;
    GerenciadorPoliticos* gerencia_politicos;
    Logger*               logger;
    std::vector<Eleitor*> eleitores;  // ciclo de vida gerenciado pela Facade

    Facade() {
        mediador_debate    = new MediadorDebate();
        config             = new Configuracao();
        gerencia_politicos = new GerenciadorPoliticos();
        logger             = Logger::get_instance();
        logger->register_log("Fachada iniciada");
    }

public:
    Facade(const Facade&)            = delete;
    Facade& operator=(const Facade&) = delete;

    ~Facade() {
        delete mediador_debate;
        delete config;
        delete gerencia_politicos;
        for (auto* e : eleitores) delete e;
    }

    static Facade* get_instance() {
        if (!instance) instance = new Facade();
        return instance;
    }

    // ---- Configuração de tempos ----
    void configuracao(int pergunta, int resposta, int replica, int treplica) {
        config->set_pergunta_tempo(pergunta);
        config->set_resposta_tempo(resposta);
        config->set_replica_tempo(replica);
        config->set_treplica_tempo(treplica);
        logger->register_log("Tempos configurados: P=" + std::to_string(pergunta)
            + "s R=" + std::to_string(resposta)
            + "s Re=" + std::to_string(replica)
            + "s Tr=" + std::to_string(treplica) + "s");
    }

    // ---- Cadastro de candidato ----
    void cadastrar_politico(const std::string& nome) {
        gerencia_politicos->criar_politico(nome, mediador_debate);
        logger->register_log("Candidato cadastrado: " + nome);
    }

    // ---- Cadastro de eleitor (Observer) ----
    void cadastrar_eleitor(const std::string& nomeEleitor,
                           const std::string& nomeCandidato) {
        auto* politico = gerencia_politicos->obter_politico(nomeCandidato);
        auto* eleitor  = new Eleitor(nomeEleitor, politico);
        politico->registrar(eleitor);   // registra no Subject
        eleitores.push_back(eleitor);
        logger->register_log("Eleitor cadastrado: " + nomeEleitor
                              + " -> " + nomeCandidato);
    }

    // ---- Sorteia inquiridor ----
    void sortear_inquiridor() {
        auto* politico = gerencia_politicos->sortear();
        mediador_debate->set_inquiridor(politico);

        // Propaga os observadores do PoliticoColaborador base
        // para o InquiridorColaborador interno do mediador
        std::vector<IObservador*> obs;
        for (auto* e : eleitores)
            if (e->getCandidato() == politico->getNome())
                obs.push_back(e);
        mediador_debate->registrar_observers_inquiridor(obs);

        logger->register_log("Inquiridor sorteado: " + politico->getNome());
        std::cout << "\n  >> Inquiridor sorteado: " << politico->getNome() << "\n";
    }

    // ---- Escolhe inquirido ----
    void escolher_inquirido(const std::string& nome) {
        auto* politico = gerencia_politicos->obter_politico(nome);
        mediador_debate->set_inquirido(politico);

        // Propaga observadores para o InquiridoColaborador interno
        std::vector<IObservador*> obs;
        for (auto* e : eleitores)
            if (e->getCandidato() == politico->getNome())
                obs.push_back(e);
        mediador_debate->registrar_observers_inquirido(obs);

        logger->register_log("Inquirido escolhido: " + nome);
        std::cout << "  >> Inquirido escolhido: " << nome << "\n";
    }

    // ---- Inicia debate ----
    void iniciar_debate() {
        logger->register_log("Debate iniciado");
        mediador_debate->debate(config);
    }

    // ---- Relatório ----
    void get_logs() const { logger->get_all_logs(); }

    // ---- Helpers para a interface CLI ----
    void listar_candidatos() const {
        std::cout << "\n  Candidatos cadastrados:\n";
        for (auto* p : gerencia_politicos->listar()) {
            std::cout << "    - " << p->getNome()
                      << (p->get_sorteado() ? "  [ja foi inquiridor]" : "")
                      << "\n";
        }
    }

    bool todos_sorteados() const {
        return gerencia_politicos->sorteados() >= gerencia_politicos->total();
    }

    int total_candidatos()  const { return gerencia_politicos->total(); }
    int total_sorteados()   const { return gerencia_politicos->sorteados(); }

    std::vector<std::string> candidatos_disponiveis() const {
        std::vector<std::string> v;
        for (auto* p : gerencia_politicos->listar())
            if (!p->get_sorteado()) v.push_back(p->getNome());
        return v;
    }

    std::vector<std::string> todos_candidatos() const {
        std::vector<std::string> v;
        for (auto* p : gerencia_politicos->listar())
            v.push_back(p->getNome());
        return v;
    }
};

Facade* Facade::instance = nullptr;
