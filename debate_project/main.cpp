#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <cstdlib>
#include <ctime>
#include "src/Facade.h"

// -------------------------------------------------------
// Helpers de UI
// -------------------------------------------------------
void linha()  { std::cout << "\n" << std::string(55, '=') << "\n"; }
void separa() { std::cout << std::string(55, '-') << "\n"; }

void cabecalho() {
    linha();
    std::cout << "   SISTEMA DE DEBATE POLITICO - SHELL INTERFACE\n";
    linha();
}

void pausar() {
    std::cout << "\n  [Pressione ENTER para continuar...]";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

std::string ler_linha(const std::string& prompt) {
    std::string s;
    std::cout << prompt;
    std::getline(std::cin, s);
    return s;
}

int ler_int(const std::string& prompt, int minimo = 1, int maximo = 9999) {
    int v;
    while (true) {
        std::cout << prompt;
        if (std::cin >> v && v >= minimo && v <= maximo) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return v;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "  Valor invalido. Tente novamente.\n";
    }
}

// -------------------------------------------------------
// Telas do menu
// -------------------------------------------------------
void tela_cadastrar_candidatos(Facade* f) {
    linha();
    std::cout << "  CADASTRO DE CANDIDATOS\n";
    separa();
    while (true) {
        std::string nome = ler_linha("  Nome do candidato (ou ENTER para finalizar): ");
        if (nome.empty()) break;
        f->cadastrar_politico(nome);
        std::cout << "  Candidato \"" << nome << "\" cadastrado.\n";
    }
}

void tela_cadastrar_eleitores(Facade* f) {
    linha();
    std::cout << "  CADASTRO DE ELEITORES\n";
    separa();
    f->listar_candidatos();
    while (true) {
        std::string nomeE = ler_linha("\n  Nome do eleitor (ou ENTER para finalizar): ");
        if (nomeE.empty()) break;
        std::string nomeC = ler_linha("  Candidato preferido: ");
        try {
            f->cadastrar_eleitor(nomeE, nomeC);
            std::cout << "  Eleitor \"" << nomeE
                      << "\" cadastrado para o candidato \"" << nomeC << "\".\n";
        } catch (const std::exception& e) {
            std::cout << "  [ERRO] " << e.what() << "\n";
        }
    }
}

void tela_configurar_tempos(Facade* f) {
    linha();
    std::cout << "  CONFIGURACAO DOS TEMPOS (em segundos)\n";
    separa();
    int p  = ler_int("  Pergunta  : ", 1, 60);
    int r  = ler_int("  Resposta  : ", 1, 60);
    int re = ler_int("  Replica   : ", 1, 60);
    int tr = ler_int("  Treplica  : ", 1, 60);
    f->configuracao(p, r, re, tr);
    std::cout << "  Tempos configurados.\n";
}

void tela_rodada(Facade* f, int num_rodada) {
    linha();
    std::cout << "  RODADA " << num_rodada << "\n";
    separa();

    // 1. Sorteia inquiridor
    std::cout << "\n  [1/3] Sorteando inquiridor...\n";
    f->sortear_inquiridor();

    // 2. Lista candidatos disponíveis para inquirido
    std::cout << "\n  [2/3] Escolha o inquirido:\n";
    auto todos = f->todos_candidatos();
    int idx = 1;
    for (const auto& c : todos) {
        std::cout << "    " << idx++ << ". " << c << "\n";
    }
    std::string inquirido = ler_linha("\n  Nome do inquirido: ");
    try {
        f->escolher_inquirido(inquirido);
    } catch (const std::exception& e) {
        std::cout << "  [ERRO] " << e.what() << " - rodada cancelada.\n";
        return;
    }

    // 3. Inicia debate
    std::cout << "\n  [3/3] Iniciando rodada...\n";
    separa();
    f->iniciar_debate();
    separa();
    std::cout << "  Rodada " << num_rodada << " concluida.\n";
    pausar();
}

// -------------------------------------------------------
// Fluxo principal
// -------------------------------------------------------
int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    cabecalho();

    Facade* facade = Facade::get_instance();

    // ---- Fase 1: Cadastro de candidatos ----
    tela_cadastrar_candidatos(facade);

    if (facade->total_candidatos() < 2) {
        std::cout << "\n  [AVISO] Sao necessarios pelo menos 2 candidatos.\n";
        // Demo automático com 3 candidatos se o usuário não cadastrou
        std::cout << "  Cadastrando candidatos de demonstracao...\n";
        facade->cadastrar_politico("Alice");
        facade->cadastrar_politico("Bruno");
        facade->cadastrar_politico("Carla");
    }

    // ---- Fase 2: Cadastro de eleitores ----
    std::cout << "\n  Deseja cadastrar eleitores? (s/n): ";
    char resp;
    std::cin >> resp;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (resp == 's' || resp == 'S') {
        tela_cadastrar_eleitores(facade);
    } else {
        // Demo: cadastra eleitores automaticamente
        std::cout << "\n  Cadastrando eleitores de demonstracao...\n";
        auto cands = facade->todos_candidatos();
        if (cands.size() >= 1) {
            facade->cadastrar_eleitor("Eleitor_A1", cands[0]);
            facade->cadastrar_eleitor("Eleitor_A2", cands[0]);
        }
        if (cands.size() >= 2) {
            facade->cadastrar_eleitor("Eleitor_B1", cands[1]);
        }
        if (cands.size() >= 3) {
            facade->cadastrar_eleitor("Eleitor_C1", cands[2]);
            facade->cadastrar_eleitor("Eleitor_C2", cands[2]);
        }
    }

    // ---- Fase 3: Configuração dos tempos ----
    std::cout << "\n  Deseja configurar os tempos? (s/n): ";
    std::cin >> resp;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (resp == 's' || resp == 'S') {
        tela_configurar_tempos(facade);
    } else {
        facade->configuracao(2, 2, 1, 1);
        std::cout << "  Tempos padrao aplicados: P=2s R=2s Re=1s Tr=1s\n";
    }

    // ---- Fase 4: Rodadas do debate ----
    linha();
    std::cout << "  INICIO DO DEBATE\n";
    std::cout << "  Total de candidatos: " << facade->total_candidatos() << "\n";
    std::cout << "  Serao realizadas " << facade->total_candidatos()
              << " rodadas (cada candidato como inquiridor uma vez).\n";
    pausar();

    int rodada = 1;
    while (!facade->todos_sorteados()) {
        tela_rodada(facade, rodada++);
    }

    // ---- Fase 5: Relatório final ----
    linha();
    std::cout << "  DEBATE ENCERRADO\n";
    separa();
    facade->get_logs();

    linha();
    std::cout << "  Obrigado por usar o Sistema de Debate Politico.\n";
    linha();

    return 0;
}
