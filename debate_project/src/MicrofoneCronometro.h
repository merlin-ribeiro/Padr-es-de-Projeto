#pragma once
#include <iostream>
#include <thread>
#include <chrono>

class MicrofoneCronometro {
private:
    bool microfoneAtivo = false;

public:
    void ativar() {
        microfoneAtivo = true;
        std::cout << "      [microfone ABERTO]" << std::endl;
    }

    void desativar() {
        microfoneAtivo = false;
        std::cout << "      [microfone FECHADO]" << std::endl;
    }

    // Simula espera: usa 1 segundo por unidade para demonstração no terminal
    void esperar_tempo(int tempo) {
        std::cout << "      [cronometro: " << tempo << "s]" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(tempo));
    }
};
