#include <iostream>
#include <unistd.h>
#include <limits.h>
#include <sys/wait.h>
#include <cstdlib>
#include <vector>
#include <sstream>

// Função para dividir a string em um vetor de palavras
std::vector<std::string> split(const std::string& str) {
    std::vector<std::string> comandos;
    std::istringstream iss(str);
    std::string comando;
    while (iss >> comando) {
        comandos.push_back(comando);
    }
    return comandos;
}

void process_command(const std::string& command_line, std::vector<std::string>& history) {
    // Dividir a linha de comando em comandos e argumentos
    std::vector<std::string> comandos = split(command_line);
    if (comandos.empty()) {
        return;
    }

    std::string command = comandos[0];

    // Adicionar o comando ao histórico
    if (command != "history" || (comandos.size() >= 2 && comandos[1] != "-c")) {
        if (history.size() == 10) {
            history.erase(history.begin());
        }
        history.push_back(command_line);
    }

    // Se for comando interno...
    if (command == "exit") {
        exit(0);
    } else if (command == "pwd") {
        char buffer[PATH_MAX];
        if (getcwd(buffer, sizeof(buffer)) != nullptr) {
            std::cout << buffer << std::endl;
        } else {
            perror("Erro");
        }
        return;
    } else if (command == "cd") {
        if (comandos.size() < 2) {
            std::cerr << "cd: falta operando\n";
        } else {
            if (chdir(comandos[1].c_str()) != 0) {
                perror("cd");
            }
        }
        return;
    } else if (command == "history") {
        if (comandos.size() >= 2 && comandos[1] == "-c") {
            history.clear();
        } else {
            for (size_t i = 0; i < history.size(); ++i) {
                std::cout << i + 1 << ": " << history[i] << std::endl;
            }
        }
        return;
    }

    // Se for comando externo
    std::string absolute_path = "/usr/bin/" + command;
    if (access(absolute_path.c_str(), F_OK) == 0) { // Arquivo existe no diretório
        if (access(absolute_path.c_str(), X_OK) == 0) { // Arquivo é executável
            pid_t pid = fork();
            if (pid < 0) { // Erro
                std::cout << "Erro de execução!" << std::endl;
                return;
            } else if (pid == 0) { // Processo filho
                // Preparar argumentos para execve
                std::vector<char*> argv;
                for (auto& comando : comandos) {
                    argv.push_back(&comando[0]);
                }
                argv.push_back(nullptr);

                execve(absolute_path.c_str(), argv.data(), nullptr);
                // Se execve falhar
                perror("execve");
                exit(EXIT_FAILURE);
            } else { // Processo pai
                // Processo pai espera processo filho terminar.
                waitpid(pid, nullptr, 0);
            }
        } else { // Arquivo não é executável
            std::cout << "permission denied: " << command << std::endl;
        }
    } else { // Arquivo não existe
        std::cout << "Command not found: " << command << std::endl;
    }
}

int main() {
    std::vector<std::string> history;
    while (true) {
        std::cout << "$> ";
        std::string command_line;
        getline(std::cin, command_line);
        process_command(command_line, history);
    }
    return 0;
}
