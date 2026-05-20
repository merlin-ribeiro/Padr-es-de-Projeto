#pragma once
#include <string>
#include <vector>
#include <iostream>

// Padrão Singleton
class Logger {
private:
    static Logger* instance;
    std::vector<std::string> logs;
    Logger() = default;

public:
    Logger(const Logger&)            = delete;
    Logger& operator=(const Logger&) = delete;

    static Logger* get_instance() {
        if (!instance) instance = new Logger();
        return instance;
    }

    void register_log(const std::string& msg) {
        logs.push_back(msg);
        std::cout << "  [LOG] " << msg << std::endl;
    }

    void get_all_logs() const {
        std::cout << "\n========== RELATORIO DO DEBATE ==========\n";
        for (size_t i = 0; i < logs.size(); ++i) {
            std::cout << "  " << (i + 1) << ". " << logs[i] << "\n";
        }
        std::cout << "=========================================\n";
    }
};

Logger* Logger::instance = nullptr;
